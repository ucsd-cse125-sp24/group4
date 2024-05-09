#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 20000 // this gives 50 fps (fps = 1M / TICK_US)

ServerCore::ServerCore() {
    this->running = false;
    for (short i = 0; i < MAX_CLIENTS; i++) // setup available ids to include 1-n
        this->available_ids.push(i);
}

ServerCore::~ServerCore()
{
    shutdown();
}

void ServerCore::listen() {
    // Initialize network components, game state, graphics, etc.
    server.sock_listen();
    //maybe display some waiting for players screen?
    for (int i = int(this->clients_data.size()); i < server.get_num_clients(); i++) {
        // for each new connected client, initialize ClientData
        this->accept_new_clients(i);
    }
    running = true;
}

void ServerCore::run() {
    running = true;
    while (isRunning()) {
        auto start = std::chrono::high_resolution_clock::now();

        while (server.get_num_clients() < NUM_CLIENTS)
        {
            this->listen();
        }
        this->listen(); // comment to disallow joining mid-game
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

    InputPacket packet;
    char *buf;

    for (ClientData* client : clients_data)
    {
        FD_SET(client->sock, &readFdSet);
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0)
        {
            buf = server.sock_receive(client->sock);
            if (buf && buf[0])
            {
                InputPacket::deserialize(buf, packet);
                process_input(packet, client->id);

                // Print for testing
                printf("\nEvents: ");
                for (const auto &event : packet.events)
                    printf("%d ", event);
                printf("\n");
                printf("Camera angle: %f\n\n", packet.cam_angle);
            }
        }
    }
}

void ServerCore::process_input(InputPacket packet, int packet_id) {
    // For now operate on first player by default. TODO: Identify by socket num? Client ID?
    glm::mat4 world = serverState.players[0].world;
    PlayerObject* client_player = pWorld.findPlayer(packet_id);

    float SCALE = 0.05f; // TODO: Define this somewhere else. Maybe in a constants folder?

    // Process input events
    for (int event : packet.events) {
        glm::vec3 dir;
        switch (event) {
        case MOVE_FORWARD:
            //client_player->moveForward();
            dir = glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        case MOVE_BACKWARD:
            //client_player->moveBackward();
            dir = glm::vec3(0.0f, 0.0f, 1.0f);
			break;
        case MOVE_LEFT:
            //client_player->moveLeft();
			dir = glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
        case MOVE_RIGHT:
            //client_player->moveRight();
            dir = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        }
        playerObject->minBound += dir;
        playerObject->maxBound += dir;
        pWorld.step();
        // Rotate dir by camera angle
        dir = glm::normalize(glm::rotateY(dir, packet.cam_angle));

        world = glm::translate(world, dir * SCALE);

    }

    serverState.players[0].world = world;
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

void ServerCore::send_updates()
{
    GameStatePacket packet;
    packet.state = serverState;

    size_t bufferSize = packet.calculateSize();

    char *buffer = new char[bufferSize];
    GameStatePacket::serialize(packet, buffer);

    for (auto i = 0; i < (int)clients_data.size(); i++) {
        bool send_success = server.sock_send(clients_data[i]->sock, (int)bufferSize, buffer);
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

    delete[] buffer;
}

void ServerCore::accept_new_clients(int i) {
    SOCKET clientSock = server.get_client_sock(i);
    ClientData* client = new ClientData;
    client->sock = clientSock;
    client->id = this->available_ids.front(); // assign next avail id to client
    char* buffer = new char[sizeof(short)];
    *((short*)buffer) = client->id;
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

    PlayerObject* newPlayerObject;
    newPlayerObject->force = glm::vec3(0.0f, 0.0f, 0.0f);
    newPlayerObject->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    newPlayerObject->position = glm::vec3(0.0f, 0.0f, 0.0f);
    newPlayerObject->mass = 10;
    newPlayerObject->playerId = client->id;
    newPlayerObject->world = p_state.world;
    newPlayerObject->minBound = glm::vec3(-1, -1, -1);
    newPlayerObject->maxBound = glm::vec3(1, 1, 1);
    
    pWorld.addObject(newPlayerObject);
    pWorld.addPlayer(newPlayerObject);

    printf("added new client data\n");
}