#include "server_core.h"

ServerCore::ServerCore() : running(false) {}

ServerCore::~ServerCore() {
    shutdown();
}

void ServerCore::initialize() {
    // Initialize network components, game state, graphics, etc.
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
    // Cleanup code
    running = false;
}

bool ServerCore::isRunning() const {
    return running;
}

int ServerCore::get_num_clients() {
    return 4;
}

void ServerCore::receive_data() {
    Server server = Server();
    for (int i = 0; i < this->get_num_clients(); i){
        // add timer to break if needed? so server won't be stuck on waiting for one client
        char* buf = server.sock_receive(server.get_client_sock(i));
        if (buf){
            this->data.push_back(std::string(buf));
        }
    }
}

void ServerCore::process_input(){}

void ServerCore::update_game_state(){}

void ServerCore::send_updates(){
    
    Server server = Server();
    for (int i = 0; i < this->get_num_clients(); i){
        // add timer to break if needed? so server won't be stuck on waiting for one client
        char* buf = server.sock_receive(server.get_client_sock(i));
        if (buf){
            this->data.push_back(std::string(buf));
        }
    }
}