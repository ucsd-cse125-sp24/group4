#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H

#include <vector>
#include <string>
#include <Winsock2.h>

#include "../include/server.h"
#include "../include/client.h"
#include "../include/windows_socket.h"

struct ServerData {
    std::vector<std::string> messages;  // Received updates from server
};

class ClientCore {
public:
    ClientCore();
    ~ClientCore();

    void initialize();             // Initialize client resources
    void shutdown();               // Clean up resources
    void run();                    // Main client loop


    void send_input();                              // Send player input (event?) to the server
    void receive_updates();                         // Receive updates from server
    void process_server_data();                     // Process the received data
    void update_local_game_state();                 // Update local game state based on server updates
    void render();                                  // Render the game state to the user

    bool connected;                // Connection state
    Client client;                 // client.conn_sock = socket, 
    ServerData server_updates;
};


#endif