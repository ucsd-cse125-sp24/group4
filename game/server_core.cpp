#include "../include/server_core.h"

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
    int prev = 0;
    while (isRunning()) {
        while (server.get_num_clients() < NUM_CLIENTS)
        {
            prev = server.get_num_clients();
            server.sock_listen();
            //maybe display some waiting for players screen?
            if (server.get_num_clients() > prev){ // for each new connected client, initialize ClientData
                this->accept_new_clients();
                printf("server connection %i\n", server.get_num_clients());
            }
        }
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

void ServerCore::update_game_state() {
    // while (gameState.students.size() < 5) {
    //     StudentState newStudent;
    //     newStudent.x = 4.0f;
    //     newStudent.y = 5.0f;
    //     newStudent.z = 6.0f;
    //     newStudent.orientation = 7.0f;

    //     gameState.students.push_back(newStudent);
    // }
}

void ServerCore::send_updates(){
    ServerPacket pkt;
    pkt.id = curr_id;
    curr_id += 1;
    pkt.message = "Hi client!";
    pkt.state = gameState;

    char send_buffer[sizeof(ServerPacket)];
    auto i = std::begin(clients_data);
    while (i != std::end(clients_data)) {
        serialize_server_packet(&pkt, send_buffer);
        bool send_success = server.sock_send((*i).sock, sizeof(send_buffer), (char*)send_buffer);

        if (!send_success) {
            i = clients_data.erase(i);
        }
        else
            i++;
    }
}

void ServerCore::accept_new_clients() {
    SOCKET clientSock = server.get_client_sock(0);
    ClientData client;
    client.sock = clientSock;
    clients_data.push_back(client);
    
    PlayerState newPlayer;
    newPlayer.x = 0.0f;
    newPlayer.y = 1.0f;
    newPlayer.z = 2.0f;
    newPlayer.orientation = 3.0f;
    newPlayer.score = 0;
    //gameState.players.push_back(newPlayer);

    printf("added new client data\n");
}