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

    // Initialize graphics
    window = Graphics::set_up_window();

    while (client.is_connected() == false)
        ;
    connected = true;
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
    InputPacket packet;/*
    packet.events.push_back(0);
    packet.events.push_back(1);*/
    packet.cam_angle = 0.0f;
    // TODO: Get events and push it into packet
    std::vector<int> tmp = Window::get_input_actions();
    for (int event : tmp)
        packet.events.push_back(event);

    // TODO: Get cam_angle and push it to packet - Camera controls need to be implemented first

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];

    InputPacket::serialize(packet, buffer);
    if (!client.sock_send(bufferSize, buffer)) {
        delete[] buffer;
        shutdown();
    }

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

void ClientCore::process_server_data() {
    // Only update the single cube for now.
    // TODO: Extend to multiple objects (students, players, etc.) - need a Scene class for that.
    
    // TODO: Take the ClientState World and slap it into the cube
}

void ClientCore::renderGameState()
{
    // Print
    printf("\n\n");
    std::cout << "Level: " << clientState.level << std::endl;
    std::cout << "Players:" << std::endl;
    for (const auto &player : clientState.players)
    {

        Window::cube->set_world(player.world);
        // Print player world matrix
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
				std::cout << player.world[i][j] << " ";
			}
			std::cout << std::endl;
		}
        std::cout << "  Score: " << player.score << std::endl;
        
        std::cout.flush();
    }

    // Don't need students rn...
    //std::cout << "Students:" << std::endl;
    //for (const auto &student : clientState.students)
    //{
    //    // Print student world matrix
    //    for (int i = 0; i < 4; i++)
    //    {
    //        for (int j = 0; j < 4; j++)
    //        {
    //            std::cout << student.world[i][j] << " ";
    //        }
    //        std::cout << std::endl;
    //    }
    //}
    printf("\n\n");



    // Render
    Window::display_callback(window);
    Window::idle_callback();
}