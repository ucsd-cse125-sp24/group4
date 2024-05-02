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
    window = Graphics::set_up_window();
    while (!client.is_connected()) {
        client.connect_to_server();
    }
    
    // recv id from server
    char* buffer = client.sock_receive();
    if (!buffer || !buffer[0]){
        // TODO: indicate failure somehow
        printf("recv failure\n");
        return;
    }
    this->id = *((short*)buffer);
    connected = true;
    printf("client connected with id %d\n", this->id);
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
    packet.cam_angle = Window::get_cam_angle_radians();
    // TODO: Get events and push it into packet
    std::vector<int> tmp = Window::get_input_actions();
    for (int event : tmp)
        packet.events.push_back(event);

    // TODO: Get cam_angle and push it to packet - Camera controls need to be implemented first
 
    if (glfwWindowShouldClose(window)) {
        shutdown();
    }

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];

    InputPacket::serialize(packet, buffer);
    if (!client.sock_send((int)bufferSize, buffer)) {
        delete[] buffer;
        shutdown();
    }

    delete[] buffer;
}

void ClientCore::receive_updates() {
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;

    char * received_data;
    GameStatePacket packet;

    FD_SET(client.conn_sock, &readFdSet);
    while (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
        received_data = client.sock_receive();
        if (received_data && received_data[0]){
            printf("receive is non null, deserializing...\n");
            GameStatePacket::deserialize(received_data, packet);
            clientState = packet.state;
            //server_updates.messages.push_back(received_data);
            printf("client got \"%s\" from server\n", received_data);
        }
    }
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