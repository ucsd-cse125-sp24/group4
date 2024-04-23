#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 40000 // this gives 25 fps (fps = 1M / TICK_US)

ServerCore::ServerCore() : running(false) {}

ServerCore::~ServerCore() {
    shutdown();
}

// Non-member function to allow threading for server listening
DWORD __stdcall call_listen(void* server){
    auto object = reinterpret_cast<Server*>(server);
    if (object){
        object->sock_listen();
    }
    return 0U;
} 

void ServerCore::initialize() {
    // Initialize network components, game state, graphics, etc.
    unsigned long threadID = 0U;
    server.listener_thread = CreateThread(nullptr, 0U, &call_listen, &server, 0, &threadID);
    printf("initializing\n");

    while (server.get_num_clients() < 1) // change 1 to however many we want; remove while loop once some player-controlled start functionality is added
        ;
    running = true;
}

void ServerCore::run() {
    while (isRunning()) {
        auto start = std::chrono::high_resolution_clock::now();

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
            if (buf){
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
    
    for (auto& client : clients_data) {
        // add timer to break if needed? so server won't be stuck on waiting for one client
        server.sock_send(client.sock, strlen(teststr) + 1, teststr);
    }
}

void ServerCore::accept_new_clients() {
    SOCKET clientSock = server.get_client_sock(0);
    ClientData client;
    client.sock = clientSock;
    clients_data.push_back(client);
    printf("added new client data\n");
}
