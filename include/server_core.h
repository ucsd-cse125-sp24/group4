#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <Winsock2.h>

#include "client.h"
#include "game_state.h"
#include "inih/INIReader.h"
#include "physics/physics_world.h"
#include "packets/game_state_packet.h"
#include "packets/input_packet.h"
#include "packets/server_heartbeat_packet.h"
#include "packets/vote_packet.h"
#include "server.h"
#include "windows_socket.h"
#include "game_state.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

struct ClientData {
    SOCKET sock;
    std::vector<std::string> messages;  // string for now
    short id;
    Vote ready_to_start;
};

class ServerCore {
    private:
        INIReader reader;                   // Configuration reader
        std::vector<short> available_ids;

    public:
        short ready_players;
        ServerState state;

        ServerCore();                       // Constructor
        ~ServerCore();                      // Destructor
        void listen();                      // Initialize server resources
        void shutdown();                    // Clean up resources
        void run();                         // Run the server's main loop

        bool isRunning() const;             // Check if the server is running

        void receive_data();                // Receive data from clients
        void process_input(InputPacket packet, short id); // Process inputs
        void update_game_state();           // Update the game state
        void send_heartbeat();              // Prepare ServerHeartbeatPacket and call send_packet
        void send_updates();                // Prepare GameStatePacket and call send_packet to send
        void send_serial(char* to_send);    // Send updates to clients
        void accept_new_clients(int i);

        bool running;                       // Server running state
        Server server;
        std::vector<ClientData*> clients_data;
        GameState serverState;
        PhysicsWorld pWorld;
};

#endif