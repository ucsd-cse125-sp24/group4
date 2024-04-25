#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H

#include <vector>
#include <deque>
#include <string>
#include <Winsock2.h>

#include "server.h"
#include "client.h"
#include "windows_socket.h"
#include "input_packet.h"
#include "game_state_packet.h"
#include "game_state.h"

// Include graphics
#include "core.h"
#include "graphics.h"

class ClientCore
{
public:
    ClientCore();
    ~ClientCore();

    void initialize(); // Initialize client resources
    void shutdown();   // Clean up resources
    void run();        // Main client loop

    void send_input();                     // Send player input (event?) to the server
    void receive_updates();                // Receive updates from server
    void process_server_data();            // Process the received data
    void update_local_game_state();        // Update local game state based on server updates
    void renderGameState(); // Render the game state to the user

    bool connected; // Connection state
    Client client;  // client.conn_sock = socket,
    GameState clientState;
    GLFWwindow* window; // Game window
};

#endif