#include "../include/client_core.h"
#include <iostream>

ClientCore::ClientCore() {
    this->connected = false;
    this->server_state = LOBBY;
}

ClientCore::~ClientCore()
{
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
    this->id = *((short*)buffer) - 1;
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
    printf("Successfully joined game! Be sure to vote to start once you're ready.");
    while (this->server_state == LOBBY) {
        // check if player has voted or rescinded vote to start; if either, send vote packet w deets
        // TODO: get actual input lmao
        send_vote(); // hard-coded to just vote READY immediately

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
        printf("yalls won\n"); // TODO: actual win logic
        return;
    }
    while (this->server_state == END_LOSE) {
        printf("a loser is u\n"); // TODO: actual loss logic
        break;
    }
}

void ClientCore::send_vote() {
    VotePacket packet;
    packet.vote = READY; // TODO
    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];

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
    char *buffer = new char[bufferSize];

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

    char * received_data;
    ServerHeartbeatPacket hb;
    GameStatePacket packet;

    FD_SET(client.conn_sock, &readFdSet);
    while (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0) {
        received_data = client.sock_receive();
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
                        printf("Error: unexpected receipt of packet type %d", type);
            }

            printf("client got \"%s\" from server\n", received_data);
        }
    }
}
void ClientCore::process_server_data() {

    // Processed in Window
    Window::update_state(world_state);
}

void ClientCore::renderGameState()
{
    printf("\n\n");
  //  std::cout << "Level: " << clientState.level << std::endl;
  //  std::cout << "Players:" << std::endl;
  //  for (const auto &player : clientState.players)
  //  {
  //      // Print player world matrix
  //      for (int i = 0; i < 4; i++)
  //      {
  //          for (int j = 0; j < 4; j++)
  //          {
		//		std::cout << player.world[i][j] << " ";
		//	}
		//	std::cout << std::endl;
		//}
  //      std::cout << "  Score: " << player.score << std::endl;
  //  }

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