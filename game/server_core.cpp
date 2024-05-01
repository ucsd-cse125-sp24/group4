#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 10000 // this gives 100 fps (fps = 1M / TICK_US)

ServerCore::ServerCore() : running(false) {}

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
        //printf("server connected to %i clients\n", server.get_num_clients());
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
                process_input(packet);

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

void ServerCore::process_input(InputPacket packet) {
    // For now operate on first player by default. TODO: Identify by socket num? Client ID?
    glm::mat4 world = serverState.players[0].world;

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
    clients_data.push_back(client);

    PlayerState p_state;
    p_state.world = glm::mat4(1.0f);

    p_state.score = 0;

    serverState.players.push_back(p_state);

    printf("added new client data\n");
}