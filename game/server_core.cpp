#include <algorithm>
#include <chrono>
#include "../include/server_core.h"

#define TICK_MICROSECS 20000 // this gives 50 fps (fps = 1M / TICK_US)
#define NUM_NPC 10

ServerCore::ServerCore()
{
    reader = INIReader("../config.ini");
    if (reader.ParseError() != 0)
    {
        std::cout << "Can't load 'config.ini'\n";
    }

    this->running = false;
    this->ready_players = 0;
    this->state = ServerState(reader.GetInteger("debug", "start_state", 1));
    for (short i = 0; i < MAX_CLIENTS; i++) // set up available ids to include [0, n)
        this->available_ids.push_back(i);
    std::sort(this->available_ids.begin(), this->available_ids.end());
}

ServerCore::~ServerCore()
{
    if (running)
        shutdown();
}

void ServerCore::listen()
{
    // get new connection and, if found, add associated client_data
    server.sock_listen();
    for (int i = int(this->clients_data.size()); i < server.get_num_clients(); i++)
        this->accept_new_clients(i);

    // check for votes
    this->receive_data();

    running = true; // this might be superfluous now, we'll see
}

void ServerCore::run()
{
    running = true;
    readBoundingBoxes();
    send_heartbeat();

    // start once max players is reached OR all (nonzero) players are ready anyway
    while (server.get_num_clients() < reader.GetInteger("debug", "expected_clients", 4) &&
           (this->ready_players < server.get_num_clients() || this->ready_players < 1))
    {
        this->listen();
    }
    printf("All players have joined or are ready! Game beginning...\n");
    this->state = MAIN_LOOP;
    send_heartbeat();

    while (isRunning())
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (reader.GetBoolean("debug", "accept_midgame", 0))
            this->listen(); // join mid-game
        if (serverState.students.size() < NUM_NPC)
        {
            initialize_npcs();
        }
        receive_data();
        update_game_state();
        send_updates();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        while (duration_us.count() < TICK_MICROSECS)
        {
            stop = std::chrono::high_resolution_clock::now();
            duration_us = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        }
    }

    this->state = END_WIN; // future todo to implement logic handling win and lose states
    send_heartbeat();
}

void ServerCore::shutdown()
{
    for (ClientData *c : clients_data)
        free(c);
    clients_data.clear(); // Clear the client data vector
    // pWorld.cleanup();
    server.sock_shutdown();
    running = false;
}

bool ServerCore::isRunning() const
{
    return running;
}

void ServerCore::initialize_npcs()
{
    while (serverState.students.size() < NUM_NPC)
    {
        glm::mat4 world = glm::scale(glm::mat4(1.0f), glm::vec3(reader.GetReal("graphics", "student_model_scale", 0.02f)));

        // Generate random positions
        float randomX = getRandomFloat(-50.0f, 50.0f);
        float randomY = getRandomFloat(0.0f, 0.0f); 
        float randomZ = getRandomFloat(-50.0f, 50.0f);

        world = glm::translate(glm::mat4(1.0f), glm::vec3(randomX, randomY, randomZ)) * world;

        // create collider for npc student and add to physics world
        AABB* c = new AABB(world[3], TYPE_NPC);
        GameObject* newStudentObject = new GameObject(c);
        pWorld.addNPC(newStudentObject);
        
        StudentState student;
        student.physicalObject = newStudentObject;
        student.world = world;
        serverState.students.push_back(student);

    }
}

void ServerCore::receive_data()
{
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10;

    InputPacket input_packet;
    VotePacket vote_packet;
    char *buf;

    for (ClientData *client : clients_data)
    {
        FD_SET(client->sock, &readFdSet);
        if (select(FD_SETSIZE, &readFdSet, NULL, NULL, &timeout) > 0)
        {
            buf = server.sock_receive(client->sock);
            if (buf && buf[0])
            {
                // printf("server received %s\n", buf);
                PacketType type = Packet::get_packet_type(buf);
                switch (type)
                {
                // handle diff kinds of packets in diff ways depending on game state
                case PLAYER_INPUT:
                    if (state != MAIN_LOOP) // only accept input during core gameplay loop
                        return;
                    InputPacket::deserialize(buf, input_packet);
                    process_input(input_packet, client->id);
                    // Print for testing
                    // printf("\nEvents: ");
                    // for (const auto &event : input_packet.events)
                    //    printf("%d ", event);
                    // printf("\nCamera angle: %f\n\n", input_packet.cam_angle);
                    break;

                case VOTE:
                    if (state != LOBBY) // only accept votes while in lobby
                        return;
                    VotePacket::deserialize(buf, vote_packet);
                    printf("received vote %d\n", *(buf + sizeof(Vote)));
                    // check that vote is actually state-changing and, if it is, update readiness
                    if (vote_packet.vote == READY && client->ready_to_start == NOT_READY)
                        ready_players++;
                    else if (vote_packet.vote == NOT_READY && client->ready_to_start == READY)
                        ready_players--;

                    client->ready_to_start = vote_packet.vote;
                    break;

                default: // shouldn't reach this
                    printf("Error: unexpected receipt of packet type %d", type);
                }
            }
        }
    }
}

void ServerCore::process_input(InputPacket packet, short id)
{
    // Find player by id, not index (for loop kinda clunky but it works for now :p)
    glm::mat4 world = glm::mat4(1.0f);

    short i;
    for (i = 0; i < serverState.players.size(); i++)
    {
        if (clients_data[i]->id == id)
        { // assumes clients are ordered the same in clients_data & serverState T.T
            world = serverState.players[i].world;
            break;
        }
    }
    PlayerObject *client_player = pWorld.findPlayer(id);

    int num_events = int(packet.events.size());

    // why does scale depend on # of events?
    float scale = float(reader.GetReal("graphics", "player_movement_scale", 4.0)) / num_events;
    glm::vec3 turndir;

    // Process input events
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f);
    for (int j = 0; j < num_events; j++)
    {
        int event = packet.events[j];
        bool jumping = false;
        switch (event)
        {
        case MOVE_FORWARD:
            dir = glm::vec3(0.0f, 0.0f, -1.0f);
            client_player->move();
            break;
        case MOVE_BACKWARD:
            dir = glm::vec3(0.0f, 0.0f, 1.0f);
            client_player->move();
            break;
        case MOVE_LEFT:
            dir = glm::vec3(-1.0f, 0.0f, 0.0f);
            client_player->move();
            break;
        case MOVE_RIGHT:
            dir = glm::vec3(1.0f, 0.0f, 0.0f);
            client_player->move();
            break;
        case JUMP:
        {
            jumping = true;
            if (client_player->getPosition().y == 0)
                client_player->jump();
            break;
        }
        case INTERACT:
        {
            // Check if ready?
            float player_x = client_player->getPosition().x;
            float player_z = client_player->getPosition().z;

            if (player_x <= -270.0f && player_x >= -290.0f && player_z <= -90.0f && player_z >= -110.0f)
            {
                printf("This player is ready!\n");
                client_player->makeReady();
            }

            continue;
        }
        }

        float player_x = client_player->getPosition().x;
        float player_z = client_player->getPosition().z;

        if (!(player_x <= -270.0f && player_x >= -290.0f && player_z <= -90.0f && player_z >= -110.0f))
        {
            client_player->makeUnready();
        }

        dir = glm::normalize(glm::rotateY(dir, packet.cam_angle));

        // client_player->minBound += dir;
        // client_player->maxBound += dir;
        // printf("dirs: <%f, %f, %f>\n", dir.x, dir.y, dir.z);

        // Also turn the alien towards the direction the camera's facing
        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
        front = serverState.players[i].world * glm::vec4(front, 0.0f);
        if (jumping)
            continue;
        // std::cout << "front for " << i << ": " << front.x << " " << front.y << " " << front.z << "\n";
        //  Find if DIR is to the right or left of FRONT
        if (j == 0)
        {
            turndir = dir;
        }
        else
        {
            dir = turndir + dir;
        }

        // Calculate the cross product of frontVector and otherVector
        glm::vec3 crossProduct = glm::cross(front, dir);

        // Check the direction of the cross product to determine left or right
        if (crossProduct.y > 0) // Assuming y-axis is up in your coordinate system
        {
            // Right
            world = glm::rotate(world, float(-reader.GetReal("graphics", "player_rotation_scale", 0.1)), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else
        {
            world = glm::rotate(world, float(reader.GetReal("graphics", "player_rotation_scale", 0.1)), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
    pWorld.step();
    client_player->setPlayerWorld(world);

    // world = glm::translate(world, (client_player->getPosition() - client_player->getOldPosition()) * SCALE);
    // printf("world m %f,%f,%f\n", world[3][0], world[3][1], world[3][2]);
    world[3] = glm::vec4(client_player->getPosition(), 1.0f);

    // printf("forces: <%f, %f, %f>\n", client_player->force.x, client_player->force.y, client_player->force.z);

    serverState.players[i].world = world;
}

void ServerCore::update_game_state()
{
    int win = 1;
    // Update parts of the game that don't depend on player input.
    for (int i = 0; i < serverState.players.size(); i++)
    {
        PlayerObject *client_player = pWorld.findPlayer(i);
        if (client_player->getReady() == 0)
        {
            win = 0;
            break;
        }
    }
    if (win)
    {
        this->state = END_WIN;
        send_heartbeat();
    }

    auto now = std::chrono::high_resolution_clock::now();

    for (StudentState &s : serverState.students)
    {
        float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - s.lastUpdate).count();
        s.timeSinceLastUpdate += deltaTime;

        if (s.timeSinceLastUpdate >= 0.1f)
        {                                                                 // Check if 0.1 second has passed
            serverState.moveStudent(s, serverState.players, 1.0f, 10.0f); // Move student
            s.physicalObject->getCollider().setBoundingBox(s.world[3], TYPE_NPC);               // set npc student bounding box in pWorld
            // pWorld.step_student(s.world);
            // s.world[3] = glm::vec4(s.physicalObject->getPosition(), 1.0f);

            s.timeSinceLastUpdate = 0.0f;                                 // Reset the timer
        }
        s.lastUpdate = now; // Update the last update time

        if (s.hasCaughtPlayer)
        {
            this->state = END_LOSE;
            send_heartbeat();
            // break;
        }
    }
}

void ServerCore::send_heartbeat()
{
    ServerHeartbeatPacket packet;
    packet.state = this->state;

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[CLIENT_RECV_BUFLEN];
    ServerHeartbeatPacket::serialize(packet, buffer);

    this->send_serial(buffer);
    delete[] buffer;
}

void ServerCore::send_updates()
{
    GameStatePacket packet;
    packet.state = serverState;

    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[CLIENT_RECV_BUFLEN];
    GameStatePacket::serialize(packet, buffer);

    this->send_serial(buffer);
    delete[] buffer;
}

void ServerCore::send_serial(char *to_send)
{
    for (int i = 0; i < (int)clients_data.size(); i++)
    {
        bool send_success = server.sock_send(clients_data[i]->sock, CLIENT_RECV_BUFLEN, to_send);
        // if client shutdown, tear down this client/player
        if (!send_success)
        {
            this->available_ids.push_back(clients_data[i]->id); // reclaim id as available
            std::sort(this->available_ids.begin(), this->available_ids.end());
            server.close_client(clients_data[i]->sock);
            free(clients_data[i]);
            clients_data.erase(clients_data.begin() + i);
            serverState.players.erase(serverState.players.begin() + i);
            i--;
        }
    }
}

void ServerCore::accept_new_clients(int i)
{
    SOCKET clientSock = server.get_client_sock(i);
    ClientData *client = new ClientData;
    client->sock = clientSock;
    client->ready_to_start = NOT_READY;
    client->id = this->available_ids.front(); // assign next avail id to client
    char *buffer = new char[CLIENT_RECV_BUFLEN];
    const char send_id = char(client->id + 1); // add 1 bc we can't send 0 (null); clientcore subs 1 to correct
    strncpy_s(buffer, CLIENT_RECV_BUFLEN, &send_id, 16);
    bool send_success = server.sock_send(client->sock, CLIENT_RECV_BUFLEN, buffer);
    if (!send_success)
    {
        server.close_client(clientSock); // abort conn
        free(client);
        return;
    }
    delete[] buffer;
    this->available_ids.erase(this->available_ids.begin()); // on success, id is no longer available, client is added
    clients_data.push_back(client);

    PlayerState p_state;
    p_state.world = glm::scale(glm::mat4(1.0f), glm::vec3(reader.GetReal("graphics", "player_model_scale", 0.01),
                                                          reader.GetReal("graphics", "player_model_scale", 0.01),
                                                          reader.GetReal("graphics", "player_model_scale", 0.01)));

    p_state.score = 0;

    serverState.players.push_back(p_state);

    AABB *c = new AABB(glm::vec3(0.0f), TYPE_PLAYER); // create a collider for the player at position 0,0,0
    PlayerObject *newPlayerObject = new PlayerObject(c);

    newPlayerObject->setPlayerId(client->id);
    
    // pWorld.addObject(newPlayerObject);
    pWorld.addPlayer(newPlayerObject);

    printf("added new client data\n");
}

glm::vec3 parseLine(std::string line) {
    // Remove trailing period if present
    if (line.back() == '.') {
        line.pop_back();
    }

    // Replace commas with spaces
    std::replace(line.begin(), line.end(), ',', ' ');

    std::istringstream iss(line);
    glm::vec3 vec;
    iss >> vec.x >> vec.y >> vec.z;
    return vec;
}

void ServerCore::readBoundingBoxes() {
     std::ifstream file("../game/floor2Data");
    if (!file) {
        std::cerr << "Failed to open the file for reading.\n";
        return;
    }
    glm::vec3 minVec, maxVec;
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        if (lineCount % 2 == 0) {
            minVec = parseLine(line);
        } else {
            maxVec = parseLine(line);
            AABB* c = new AABB(minVec, maxVec);
            // printf("this is maxVec %f %f %f\n",maxVec.x, maxVec.y, maxVec.z);
            // printf("Added object to physics world with bounding box minExtents %f %f %f\n", c->minExtents.x, c->minExtents.y,c->minExtents.z);
            // printf("                                                maxExtents %f %f %f\n", c->maxExtents.x, c->maxExtents.y,c->maxExtents.z);
            GameObject* newGameObject = new GameObject(c);
            pWorld.addObject(newGameObject);
        }
        lineCount++;
    }

    if (lineCount % 2 != 0) {
        std::cerr << "File has an odd number of lines." << std::endl;
    }

    file.close();
}