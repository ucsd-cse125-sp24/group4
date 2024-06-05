#include "../include/client_core.h"
#include <iostream>

#pragma comment(lib, "Winmm.lib")

ClientCore::ClientCore() {
    this->connected = false;
    this->server_state = LOBBY;
}

ClientCore::~ClientCore()
{
    if (connected) // only shutdown if it hasn't been called already (else double-closes)
        shutdown();
}

void ClientCore::initialize()
{
    while (!client.is_connected()) {
        client.connect_to_server();
    }
    
    // recv id from server
    char* buffer = client.sock_receive();
    while (!buffer || !buffer[0]){
        buffer = client.sock_receive();
    }
    this->id = *buffer - 1;
    connected = true;
    printf("client connected with id %d\n", this->id);

    // Initialize graphics
    window = Graphics::set_up_window(this->id);
}

bool ClientCore::is_connected() {
    return connected;
}

void ClientCore::shutdown()
{
    // Clean up all resources
    connected = false;
    Window::clean_up();
    client.close_conn();
}

void ClientCore::run()
{
    PlaySound((LPCSTR)"../audio/sneaky_background.wav", GetModuleHandle(NULL), SND_LOOP | SND_ASYNC);
    
    printf("Successfully joined game! Be sure to vote to start once you're ready.");
    while (this->server_state == LOBBY) {
        // check if player has voted or rescinded vote to start; if either, send vote packet w deets
        // TODO: get actual input lmao
        // send_vote(); // hard-coded to just vote READY immediately

        // check for progression to MAIN_LOOP
        receive_updates();

        // maybe display some 'waiting for players' screen if there's time?
    }

    while (connected && this->server_state == MAIN_LOOP)
    {
        receive_updates();
        process_server_data();
        renderGameState();
        send_input(); // moving to bottom bc only send can shutdown
    }

    while (this->server_state == END_WIN) {
        printf("\nyalls won!\n"); // TODO: actual win logic
        return;
    }
    while (this->server_state == END_LOSE) {
        printf("\na loser is u\n"); // TODO: actual loss logic
        break;
    }
}

void ClientCore::send_vote() {
    VotePacket packet;
    packet.vote = READY; // TODO
    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[SERVER_RECV_BUFLEN];

    VotePacket::serialize(packet, buffer);
    if (!client.sock_send((int)bufferSize, buffer)) {
        shutdown();
    }

    delete[] buffer;
}

void ClientCore::send_input()
{
    InputPacket packet;
    packet.cam_angle = Window::get_cam_angle_radians();
    std::vector<int> tmp = Window::get_input_actions();
    for (int event : tmp)
        packet.events.push_back(event);
 
    if (glfwWindowShouldClose(window)) {
        shutdown();
        return;
    }

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[SERVER_RECV_BUFLEN];

    InputPacket::serialize(packet, buffer);
    if (!client.sock_send((int)bufferSize, buffer)) {
        shutdown();
    }

    delete[] buffer;
}

void ClientCore::receive_updates() {
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10;

    char * received_data = NULL;
    ServerHeartbeatPacket hb;
    GameStatePacket packet;

    FD_SET(client.conn_sock, &readFdSet);
    while (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
        received_data = client.sock_receive();
        if (received_data == NULL) {
            printf("receive failed. exiting from client\n");
            shutdown();
            return;
        }
        if (received_data && received_data[0]){
            PacketType type = Packet::get_packet_type(received_data);
            switch(type) {
                case SERVER_HEARTBEAT:
                    ServerHeartbeatPacket::deserialize(received_data, hb);
                    this->server_state = hb.state;
                    break;
                case GAME_STATE:
                    GameStatePacket::deserialize(received_data, packet);
                    this->world_state = packet.state;
                    break;
                default: // shouldn't reach this
                    printf("Error: unexpected receipt of packet type %d\n", type);
                    shutdown(); // not ideal but ehhh
            }

            //printf("client got \"%s\" from server\n", received_data);
        }
        timeout.tv_sec = 0;
        timeout.tv_usec = 10;
    }
}
void ClientCore::process_server_data() {
    // Processed in Window
    Window::update_state(world_state);
    //printf("%d\n", this->world_state.score);
}

void ClientCore::renderGameState()
{
    // Render
    if (server_state == END_WIN) {
        // render win screen
    }
    else if (server_state == END_LOSE) {
        // render lose screen
        Window::display_callback(window);
        Window::idle_callback();
    }
    else {
        Window::display_callback(window);
        Window::idle_callback();
    }
}