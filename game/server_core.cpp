#include "../include/server_core.h"

ServerCore::ServerCore() : running(false) {}

ServerCore::~ServerCore()
{
    shutdown();
}

void ServerCore::initialize()
{
    // Initialize network components, game state, graphics, etc.
    printf("initializing\n");

    while (server.get_num_clients() < NUM_CLIENTS)
        server.sock_listen();

    running = true;
}

void ServerCore::run()
{
    int prev = 0;
    while (isRunning())
    {
        while (server.get_num_clients() < NUM_CLIENTS)
        {
            prev = server.get_num_clients();
            server.sock_listen();
            // maybe display some waiting for players screen?
            if (server.get_num_clients() > prev)
            { // for each new connected client, initialize ClientData
                this->accept_new_clients();
                printf("server connection %i\n", server.get_num_clients());
            }
        }
        receive_data();
        process_input();
        update_game_state();
        send_updates();
    }
}

void ServerCore::shutdown()
{
    for (ClientData client : clients_data)
    {
        server.close_client(client.sock);
    }
    clients_data.clear(); // Clear the client data vector
    server.sock_shutdown();
    running = false;
}

bool ServerCore::isRunning() const
{
    return running;
}

void ServerCore::receive_data()
{
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    for (ClientData client : clients_data)
    {
        FD_SET(client.sock, &readFdSet);
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0)
        {
            char *buf = server.sock_receive(client.sock);
            if (buf)
            {
                InputPacket packet;
                InputPacket::deserialize(buf, packet);

                // Print for testing
                if (packet.type == InputPacket::KEYBOARD)
                {
                    std::cout << "Received keyboard input: KeyCode = " << packet.keyboard.keyCode
                              << ", Pressed = " << (packet.keyboard.pressed ? "True" : "False") << std::endl;
                }
            }
        }
    }
}

void ServerCore::process_input() {}

void ServerCore::update_game_state() {}

void ServerCore::send_updates()
{
    GameStatePacket packet;
    // Example packet
    for (int i = 0; i < 4; i++)
    {
        packet.state.players[i].x = i * 1.0f;
        packet.state.players[i].y = i * 2.0f;
        packet.state.players[i].z = i * 3.0f;
        packet.state.players[i].orientation = i * 10.0f;
        packet.state.players[i].score = i * 100;
    }

    packet.state.students.push_back({1.0f, 2.0f, 3.0f, 0.0f});
    packet.state.students.push_back({4.0f, 5.0f, 6.0f, 1.0f});
    packet.state.level = 5;

    size_t bufferSize = sizeof(size_t) +                                                     // Number of players
                        4 * (sizeof(float) * 3 + sizeof(float) + sizeof(int)) +              // Player data
                        sizeof(size_t) +                                                     // Number of students
                        packet.state.students.size() * (sizeof(float) * 3 + sizeof(float)) + // Student data
                        sizeof(int);                                                         // Level

    char *buffer = new char[bufferSize];
    GameStatePacket::serialize(packet, buffer);

    auto i = std::begin(clients_data);
    while (i != std::end(clients_data))
    {
        bool send_success = server.sock_send((*i).sock, bufferSize, buffer);
        if (!send_success)
            i = clients_data.erase(i);
        else
            i++;
    }
    delete[] buffer;
}

void ServerCore::accept_new_clients()
{
    SOCKET clientSock = server.get_client_sock(0);
    ClientData client;
    client.sock = clientSock;
    clients_data.push_back(client);
    printf("added new client data\n");
}