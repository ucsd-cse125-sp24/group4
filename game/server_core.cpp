#include "../include/server_core.h"

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
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &server, 0, &threadID);
    printf("initializing\n");

    while (server.get_num_clients() < 1) // change 1 to however many we want; remove while loop once some player-controlled start functionality is added
        ;
    running = true;
}

void ServerCore::run() {
    while (isRunning()) {
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
