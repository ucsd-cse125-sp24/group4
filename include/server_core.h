#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <vector>
#include <string>
#include <Winsock2.h>

#include "networking/server.h"
#include "networking/windows_socket.h"

struct ClientData {
    SOCKET sock;                        // not sure about this
    std::vector<std::string> messages;  // string for now
};

class ServerCore {
    public:
        ServerCore();                   // Constructor
        ~ServerCore();                  // Destructor
        void initialize();              // Initialize server resources
        void shutdown();                // Clean up resources
        void run();                     // Run the server's main loop

        bool isRunning() const;         // Check if the server is running
        int get_num_clients();          // Number of Clients (default to 4)

        void receive_data();            // Receive data from clients
        void process_input();           // Process inputs
        void update_game_state();       // Update the game state
        void send_updates();            // Send updates to clients

        bool running;                   // Server running state

        // std::vector<ClientData> data;   // all client data passed in. later add other data like changes in npc or environment?
        std::vector<std::string> data;  // string for now
};

#endif