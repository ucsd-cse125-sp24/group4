#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <vector>
#include <string>
#include <iostream>
#include <Winsock2.h>

#include "server.h"
#include "client.h"
#include "windows_socket.h"
#include "packet.h"
#include "input_packet.h"
#include "game_state_packet.h"

#define NUM_CLIENTS 1

struct ClientData {
    SOCKET sock;                        // not sure about this
    std::vector<std::string> messages;  // string for now
};

class ServerCore {
    private:
        int curr_id = 0;

    public:
        ServerCore();                   // Constructor
        ~ServerCore();                  // Destructor
        void initialize();              // Initialize server resources
        void shutdown();                // Clean up resources
        void run();                     // Run the server's main loop

        bool isRunning() const;         // Check if the server is running

        void receive_data();            // Receive data from clients
        void process_input();           // Process inputs
        void update_game_state();       // Update the game state
        void send_updates();            // Send updates to clients
        void accept_new_clients();

        bool running;                   // Server running state
        Server server;
        std::vector<ClientData> clients_data;

        // std::vector<ClientData> data;   // all client data passed in. later add other data like changes in npc or environment?
        std::vector<std::string> data;  // string for now
};

#endif