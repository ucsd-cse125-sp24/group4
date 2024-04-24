#include "../include/client_core.h"
#include <iostream>

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
        send_input();
        receive_updates();
        process_server_data();
        render();
    }
}

void ClientCore::send_input() {}

void ClientCore::receive_updates() {
    // Receive data from the server
    char* received_data = client.sock_receive();
    for (int i = 0; i < strlen(received_data); i += sizeof(ServerPacket)) {
        char one_packet[sizeof(ServerPacket)];
        memcpy(one_packet, received_data + i, sizeof(ServerPacket));
        ServerPacket* s_pkt = new ServerPacket;
        deserialize_server_packet(s_pkt, one_packet);
        server_updates.spacket_buffer.push_back(s_pkt);
    }
}

void ClientCore::process_server_data() {
    // Process received updates
    // Update the game state based on received data
    while (server_updates.spacket_buffer.size() > 0) {
        ServerPacket* s_pkt = server_updates.spacket_buffer.front();
        server_updates.spacket_buffer.pop_front();
        gameState = s_pkt->state;
        
        printf("Packet #%d: %s\n", s_pkt->id, s_pkt->message.c_str());
        //printf("Num players: %d\n", gameState.players.size());
        
        delete s_pkt;
    }
}

void ClientCore::render() {
    // Render the current state of the game to the screen
}