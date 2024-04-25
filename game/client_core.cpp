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

    // Example packet
    packet.type = InputPacket::KEYBOARD;
    packet.keyboard.keyCode = 65;
    packet.keyboard.pressed = false;

    size_t bufferSize = sizeof(InputPacket);
    char *buffer = new char[bufferSize];

    InputPacket::serialize(packet, buffer);
    client.sock_send(bufferSize, buffer);

    delete[] buffer;
}

void ClientCore::receive_updates()
{
    // Receive data from the server
    char *received_data = client.sock_receive();

    GameStatePacket packet;
    GameStatePacket::deserialize(received_data, packet);
    clientState = packet.state;
}

void ClientCore::process_server_data() {}

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