#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 40000 // this gives 25 fps (fps = 1M / TICK_US)

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
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    InputPacket packet;
    char *buf;

    for (ClientData* client : clients_data)
    {
        FD_SET(client->sock, &readFdSet);
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0)
        {
            buf = server.sock_receive(client->sock);
            if (buf)
            {
                InputPacket::deserialize(buf, packet);

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

void ServerCore::update_game_state() {
    while (serverState.students.size() < 5) { // TODO: change "5"s in this func to constants defined elsewhere
        StudentState s_state;
        s_state.x = serverState.students.size();
        s_state.y = serverState.students.size();
        s_state.z = serverState.students.size();
        s_state.orientation = serverState.students.size();

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

    for (auto i = 0; i < clients_data.size(); i++) {
        bool send_success = server.sock_send(clients_data[i]->sock, bufferSize, buffer);
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
    p_state.x = 0.0f;
    p_state.y = 0.0f;
    p_state.z = 0.0f;
    p_state.orientation = 0.0f;
    p_state.score = 0;

    serverState.players.push_back(p_state);

    printf("added new client data\n");
}