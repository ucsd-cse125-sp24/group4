#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 40000 // this gives 25 fps (fps = 1M / TICK_US)

ServerCore::ServerCore() : running(false) {}

ServerCore::~ServerCore() {
    shutdown();
}

void ServerCore::initialize() {
    // Initialize network components, game state, graphics, etc.
    printf("initializing\n");

    while (server.get_num_clients() < NUM_CLIENTS)
        server.sock_listen();

    running = true;
}

void ServerCore::run() {
    while (isRunning()) {
        auto start = std::chrono::high_resolution_clock::now();

        while (server.get_num_clients() < NUM_CLIENTS)
        {
            server.sock_listen();
            //maybe display some waiting for players screen?
            for (int i = int(this->clients_data.size()); i < server.get_num_clients(); i++) {
                // for each new connected client, initialize ClientData
                this->accept_new_clients(i);
            }
        }
        //printf("server connected to %i clients\n", server.get_num_clients());
        receive_data();
        process_input();
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

void ServerCore::shutdown() {
    for (ClientData client : clients_data) {
        server.close_client(client.sock);
    }
    clients_data.clear(); // Clear the client data vector
    server.sock_shutdown();
    running = false;
}

bool ServerCore::isRunning() const {
    return running;
}

void ServerCore::receive_data() {
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    for (ClientData client : clients_data) {
        FD_SET(client.sock, &readFdSet);
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
            char* buf = server.sock_receive(client.sock);
            if (buf && buf[0]){
                client.messages.push_back(std::string(buf));
                printf("server got \"%s\" from client\n", buf);
            }
        }
    }
}

void ServerCore::process_input(){}

void ServerCore::update_game_state(){}

void ServerCore::send_updates(){
    const char* teststr = "Goodbye, world!";
    
    auto i = std::begin(clients_data);
    while (i != std::end(clients_data)) {
        if (!server.sock_send((*i).sock, int(strlen(teststr)) + 1, teststr))
            i = clients_data.erase(i);
        else
            i++;
    }
}

void ServerCore::accept_new_clients(int i) {
    SOCKET clientSock = server.get_client_sock(i);
    ClientData client;
    client.sock = clientSock;
    clients_data.push_back(client);
    printf("added new client data\n");
}
