#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 20000 // this gives 50 fps (fps = 1M / TICK_US)

ServerCore::ServerCore() {
    this->running = false;
    this->ready_players = 0;
    this->state = LOBBY;
    for (short i = 0; i < MAX_CLIENTS; i++) // setup available ids to include 1-n
        this->available_ids.push(i);
}

ServerCore::~ServerCore()
{
    shutdown();
}

void ServerCore::listen() {
    // get new connection and, if found, add associated client_data
    server.sock_listen();
    for (int i = int(this->clients_data.size()); i < server.get_num_clients(); i++)
        this->accept_new_clients(i);

    // check for votes
    this->receive_data();

    running = true; // this might be superfluous now, we'll see
}

void ServerCore::run() {
    running = true;
    send_heartbeat();

    // start once max players is reached OR all (nonzero) players are ready anyway
    while (server.get_num_clients() < NUM_CLIENTS && 
          (this->ready_players < server.get_num_clients() || this->ready_players < 1)) {
            this->listen();
    }
    printf("All players have joined or are ready! Game beginning...\n");
    this->state = MAIN_LOOP;
    send_heartbeat();

    while (isRunning()) {
        auto start = std::chrono::high_resolution_clock::now();

        //this->listen(); // uncomment to allow joining mid-game
        receive_data();
        update_game_state();
        send_updates();
        
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        while (duration_us.count() < TICK_MICROSECS) {
            stop = std::chrono::high_resolution_clock::now();
            duration_us = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        }
    }

    this->state = END_WIN; // future todo to implement logic handling win and lose states
    send_heartbeat();
}

void ServerCore::shutdown()
{
    for (ClientData* c : clients_data)
        free(c);
    clients_data.clear(); // Clear the client data vector
    server.sock_shutdown();
    running = false;
}

bool ServerCore::isRunning() const
{
    return running;
}

void ServerCore::receive_data()
{
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10;

    InputPacket input_packet;
    VotePacket vote_packet;
    char *buf;

    for (ClientData* client : clients_data)
    {
        FD_SET(client->sock, &readFdSet);
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0)
        {
            buf = server.sock_receive(client->sock);
            if (buf && buf[0])
            {
                PacketType type = Packet::get_packet_type(buf);
                switch(type) {
                    // handle diff kinds of packets in diff ways depending on game state
                    case PLAYER_INPUT:
                        if (state != MAIN_LOOP) // only accept input during core gameplay loop
                            return;
                        InputPacket::deserialize(buf, input_packet);
                        process_input(input_packet, client->id);
                        // Print for testing
                        printf("\nEvents: ");
                        for (const auto &event : input_packet.events)
                            printf("%d ", event);
                        printf("\n");
                        printf("Camera angle: %f\n\n", input_packet.cam_angle);
                        break;

                    case VOTE:
                        if (state != LOBBY) // only accept votes while in lobby
                            return;
                        VotePacket::deserialize(buf, vote_packet);
                        printf("received vote %d\n", *(buf + sizeof(Vote)));
                        // check that vote is actually state-changing and, if it is, update readiness
                        if (vote_packet.vote == READY && client->ready_to_start == NOT_READY)
                            ready_players++;
                        else if (vote_packet.vote == NOT_READY && client->ready_to_start == READY)
                            ready_players--;

                        client->ready_to_start = vote_packet.vote;
                        break;

                    default: // shouldn't reach this
                        printf("Error: unexpected receipt of packet type %d", type);
                }   
            }
        }
    }
}

void ServerCore::process_input(InputPacket packet, short id) {
    // For now operate on first player by default. TODO: Identify by socket num? Client ID?
    glm::mat4 world = serverState.players[id].world;

    float SCALE = 0.05f; // TODO: Define this somewhere else. Maybe in a constants folder?

    // Process input events
    for (int event : packet.events) {
        glm::vec3 dir;
        switch (event) {
        case MOVE_FORWARD:
            dir = glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        case MOVE_BACKWARD:
            dir = glm::vec3(0.0f, 0.0f, 1.0f);
			break;
        case MOVE_LEFT:
			dir = glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
        case MOVE_RIGHT:
            dir = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        }

        // Rotate dir by camera angle
        dir = glm::normalize(glm::rotateY(dir, packet.cam_angle));

        world = glm::translate(world, dir * SCALE);

    }

    serverState.players[id].world = world;
}

void ServerCore::update_game_state() {

    // Update parts of the game that don't depend on player input.

    // Enemy AI etc
    while (serverState.students.size() < 5) {
        StudentState s_state;

        s_state.world = glm::mat4(1.0f); // TEMP

        serverState.students.push_back(s_state);
    }
    serverState.level = 5;
}

void ServerCore::send_heartbeat() {
    ServerHeartbeatPacket packet;
    packet.state = this->state;

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];
    ServerHeartbeatPacket::serialize(packet, buffer);

    this->send_serial(buffer, bufferSize);
    delete[] buffer;
}

void ServerCore::send_updates() {
    GameStatePacket packet;
    packet.state = serverState;

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];
    GameStatePacket::serialize(packet, buffer);

    this->send_serial(buffer, bufferSize);
    delete[] buffer;
}

void ServerCore::send_serial(char* to_send, size_t length)
{
    for (int i = 0; i < (int)clients_data.size(); i++) {
        bool send_success = server.sock_send(clients_data[i]->sock, (int)length, to_send);
        // if client shutdown, tear down this client/player
        if (!send_success) {
            this->available_ids.push(clients_data[i]->id); // reclaim id as available
            server.close_client(clients_data[i]->sock);
            free(clients_data[i]);
            clients_data.erase(clients_data.begin() + i);
            serverState.players.erase(serverState.players.begin() + i);
            i--;
        }
    }
}

void ServerCore::accept_new_clients(int i) {
    SOCKET clientSock = server.get_client_sock(i);
    ClientData* client = new ClientData;
    client->sock = clientSock;
    client->ready_to_start = NOT_READY;
    client->id = this->available_ids.front(); // assign next avail id to client
    char* buffer = new char[sizeof(short)];
    *((short*)buffer) = client->id + 1; // add 1 bc we can't send 0 (null); clientcore subs 1 to correct
    bool send_success = server.sock_send(client->sock, sizeof(short), buffer);
    if (!send_success) {
        server.close_client(clientSock); // abort conn
        free(client);
        return;
    }
    delete[] buffer;
    this->available_ids.pop(); // on success, id is no longer available, client is added
    clients_data.push_back(client);

    PlayerState p_state;
    p_state.world = glm::mat4(1.0f);

    p_state.score = 0;

    serverState.players.push_back(p_state);

    printf("added new client data\n");
}