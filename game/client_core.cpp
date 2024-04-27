#include "../include/client_core.h"
#include <iostream>

ClientCore::ClientCore() : connected(false) {}

ClientCore::~ClientCore()
{
    shutdown();
}

void ClientCore::initialize()
{
    // Initialize graphics, connect client
    printf("initializing client\n");
    while (client.is_connected() == false)
        ;
    connected = true;
}

void ClientCore::shutdown()
{
    // Clean up all resources
    connected = false;
    client.close_conn();
}

void ClientCore::run()
{
    while (connected)
    {
        send_input();
        receive_updates();
        process_server_data();
        renderGameState();
    }
}

void ClientCore::send_input()
{
    InputPacket packet;
    packet.events.push_back(0);
    packet.events.push_back(1);
    packet.cam_angle = 2.0f;

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];

    InputPacket::serialize(packet, buffer);
    if (!client.sock_send(bufferSize, buffer)) {
        delete[] buffer;
        shutdown();
    }

    delete[] buffer;
}

void ClientCore::receive_updates() {
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    char * received_data;
    GameStatePacket packet;

    FD_SET(client.conn_sock, &readFdSet);
    while (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
        received_data = client.sock_receive();
        if (received_data && received_data[0]){
            GameStatePacket::deserialize(received_data, packet);
            clientState = packet.state;
            //server_updates.messages.push_back(received_data);
            printf("client got \"%s\" from server\n", received_data);
        }
    }
}
void ClientCore::process_server_data() {
    // Process received updates
    // Update the game state based on received data
}

void ClientCore::renderGameState()
{
    // Print
    printf("\n\n");
    std::cout << "Level: " << clientState.level << std::endl;
    std::cout << "Players:" << std::endl;
    for (const auto &player : clientState.players)
    {
        std::cout << "  x: " << player.x << ", y: " << player.y << ", z: " << player.z
                  << ", orientation: " << player.orientation << ", score: " << player.score << std::endl;
    }
    std::cout << "Students:" << std::endl;
    for (const auto &student : clientState.students)
    {
        std::cout << "  x: " << student.x << ", y: " << student.y << ", z: " << student.z
                  << ", orientation: " << student.orientation << std::endl;
    }
    printf("\n\n");
}