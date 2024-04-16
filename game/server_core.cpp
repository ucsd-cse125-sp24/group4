#include "../include/server_core.h"

ServerCore::ServerCore() : running(false) {}

ServerCore::~ServerCore() {
    shutdown();
}

void ServerCore::initialize() {
    // Initialize network components, game state, graphics, etc.
    unsigned long threadID = 0U;
    printf("initializing\n");

    while (server.get_num_clients() < 1)
        server.sock_listen();

    running = true;
}

void ServerCore::run() {
    while (isRunning()) {
        while (server.get_num_clients() < 1)
        {
            server.sock_listen();
            //maybe display some waiting for players screen?
            if (server.get_num_clients() == 1){
                this->accept_new_clients();
            }
        }
        printf("server connection %i\n", server.get_num_clients());
        receive_data();
        process_input();
        update_game_state();
        send_updates();
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
    
    auto i = std::begin(clients_data);
    while (i != std::end(clients_data)) {
        if (!server.sock_send((*i).sock, strlen(teststr) + 1, teststr))
            i = clients_data.erase(i);
        else
            i++;
    }
}

void ServerCore::accept_new_clients() {
    SOCKET clientSock = server.get_client_sock(0);
    ClientData client;
    client.sock = clientSock;
    clients_data.push_back(client);
    printf("added new client data\n");
}
