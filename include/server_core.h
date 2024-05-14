#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <Winsock2.h>

#include "client.h"
#include "game_state.h"
#include "physics_world.h"
#include "packets/game_state_packet.h"
#include "packets/input_packet.h"
#include "packets/server_heartbeat_packet.h"
#include "server.h"
#include "windows_socket.h"
#include "input_packet.h"
#include "game_state_packet.h"
#include "game_state.h"
#include "physics_world.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

#define NUM_CLIENTS 1

struct ClientData {
    SOCKET sock;                        // not sure about this
    std::vector<std::string> messages;  // string for now
    short id;
};

class ServerCore {
    private:
        ServerState state;
        std::vector<short> available_ids;
        short ready_players;

    public:
        ServerCore();                   // Constructor
        ~ServerCore();                  // Destructor
        void listen();              // Initialize server resources
        void shutdown();                // Clean up resources
        void run();                     // Run the server's main loop

        bool isRunning() const;         // Check if the server is running

        void receive_data();            // Receive data from clients
        void process_input(InputPacket packet, short id);           // Process inputs
        void update_game_state();       // Update the game state
        void send_updates();            // Send updates to clients
        void accept_new_clients(int i);

        bool running;                   // Server running state
        Server server;
        std::vector<ClientData*> clients_data;
        GameState serverState;
        PhysicsWorld pWorld;
};

#endif