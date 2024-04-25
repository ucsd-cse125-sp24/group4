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

    renderGameState(packet.state);
}

void ClientCore::process_server_data()
{
    // Process received updates
    // Update the game state based on received data

    // while (server_updates.spacket_buffer.size() > 0) {
    //     ServerPacket* s_pkt = server_updates.spacket_buffer.front();
    //     server_updates.spacket_buffer.pop_front();
    //     printf("Packet #%d: %s: My location is (%d, %d, %d).\n", s_pkt->id, s_pkt->message.c_str(), s_pkt->coor[0], s_pkt->coor[1], s_pkt->coor[2]);

    //     delete s_pkt;
    // }
}

void ClientCore::renderGameState(GameState state)
{
    // Print
    std::cout << "Level: " << state.level << std::endl;
    std::cout << "Players:" << std::endl;
    for (const auto &player : state.players)
    {
        std::cout << "  x: " << player.x << ", y: " << player.y << ", z: " << player.z
                  << ", orientation: " << player.orientation << ", score: " << player.score << std::endl;
    }
    std::cout << "Students:" << std::endl;
    for (const auto &student : state.students)
    {
        std::cout << "  x: " << student.x << ", y: " << student.y << ", z: " << student.z
                  << ", orientation: " << student.orientation << std::endl;
    }
}