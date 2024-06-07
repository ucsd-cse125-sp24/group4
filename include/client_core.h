#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H
#pragma once

#include <vector>
#include <deque>
#include <string>
#include <Winsock2.h>

#include "client.h"
#include "game_state.h"
#include "packets/game_state_packet.h"
#include "packets/input_packet.h"
#include "packets/server_heartbeat_packet.h"
#include "packets/vote_packet.h"
#include "server.h"
#include "server_core.h"
#include "windows_socket.h"

// Include graphics
#include "core.h"
#include "graphics.h"
#include "enums.h"

class ClientCore
{
    public:
        short id;

        ClientCore();
        ~ClientCore();

        void initialize();                  // Initialize client resources
        bool is_connected();
        void shutdown();                    // Clean up resources
        void run();                         // Main client loop

        void send_vote();                      // Send or rescind "ready to start" vote
        void send_input();                     // Send player input (event?) to the server
        void receive_updates();                // Receive updates from server
        void process_server_data();            // Process the received data
        void update_local_game_state();        // Update local game state based on server updates
        void renderGameState();                // Render the game state to the user

        bool connected; // Connection state
        Client client;  // client.conn_sock = socket,
        GameState world_state;
        ServerState server_state;
        GLFWwindow* window; // Game window
};

#endif