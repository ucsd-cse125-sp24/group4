#include "../include/client_core.h"

ClientCore::ClientCore() : connected(false) {}

ClientCore::~ClientCore() {
    shutdown();
}


void ClientCore::initialize() {
    // Initialize graphics, connect client
    printf("initializing client\n");
    while (client.is_connected() == false);
    connected = true;
}

void ClientCore::shutdown() {
    // Clean up all resources
    connected = false;
    client.close_conn();
}

void ClientCore::run() {
    while (connected) {
        receive_updates();
        process_server_data();
        update_local_game_state();
        render();
    }
}

void ClientCore::send_input(){}

void ClientCore::receive_updates() {
    // Receive data from the server
    char * received_data = client.sock_receive();
    server_updates.messages.push_back(received_data);
}

void ClientCore::process_server_data() {
    // Process received updates
    // Update the game state based on received data
}

void ClientCore::update_local_game_state() {
    // Update local representations of game state
}

void ClientCore::render() {
    // Render the current state of the game to the screen
}