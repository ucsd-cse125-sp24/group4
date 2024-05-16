#include "../include/server_core.h"

#define NUM_TEST_CLIENTS 4

// CreateThread doesn't like calling an object's member function;
// call non-member function which calls the obj's member function instead
DWORD __stdcall call_listen(void* servercore){
   auto object = reinterpret_cast<ServerCore*>(servercore);
   while (object->server.get_num_clients() < NUM_TEST_CLIENTS){
     object->listen();
   } 
   return 0U;
}

void close_and_shutdown(ServerCore* sc, std::vector<Client> client_list) {
    for (Client c : client_list)
        c.close_conn();
    sc->shutdown();
}

int test_listen_accept() {
    ServerCore sc;
    if (sc.isRunning() || sc.ready_players != 0 || sc.state != LOBBY) {
        sc.shutdown();
        return 1;
    }

    sc.listen();
    if (!sc.isRunning() || sc.clients_data.size() != 0) {
        sc.shutdown();
        return 1;
    }
    
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &sc, 0, &threadID);
    std::vector<Client> client_list;
    for (int i = 0; i < NUM_TEST_CLIENTS; i++) {
        // connect client to server
        client_list.push_back(Client());
        printf("connected %d\n", i);
    }
    WaitForSingleObject(hand, 1000);
    
    if (!sc.isRunning() || sc.clients_data.size() != NUM_TEST_CLIENTS || sc.serverState.players.size() != NUM_TEST_CLIENTS) {
        close_and_shutdown(&sc, client_list);
        return 1;
    }
    
    for (int i = 0; i < NUM_TEST_CLIENTS; i++) {
        if (sc.clients_data[i]->id != (short)i) {
            printf("wrong id %d\n", sc.clients_data[i]->id);
            close_and_shutdown(&sc, client_list);
            return 1;
        }
    }
    for (PlayerState ps : sc.serverState.players) {
        if (ps.world != glm::rotate(glm::mat4(1.0f), glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f))
            || ps.score != 0) {
            close_and_shutdown(&sc, client_list);
            return 1;
        }
    }

    close_and_shutdown(&sc, client_list);
    return 0;
}

int test_receive() {
    ServerCore sc;
    
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &sc, 0, &threadID);
    std::vector<Client> client_list;
    for (int i = 0; i < NUM_TEST_CLIENTS; i++) {
        // connect client to server
        client_list.push_back(Client());
        printf("connected %d\n", i);
    }
    WaitForSingleObject(hand, 1000);
    
    // make each client send input
    InputPacket packet;
    packet.events.push_back(0);
    packet.events.push_back(1);
    packet.cam_angle = 2.0f;
    size_t bufferSize = packet.calculateSize();
    char* buf = new char[bufferSize];
    InputPacket::serialize(packet, buf);
    
    for (Client c : client_list) {
        c.sock_send(SERVER_RECV_BUFLEN, buf);
    }

    // confirm receipt from each client
    sc.state = MAIN_LOOP;
    sc.receive_data();
    delete[] buf;
    for (PlayerState ps : sc.serverState.players) {
        // TODO: test serverstate's players' worlds all align with sent data
        // i.e., check that process_input did its job (idk how tho so maybe let it be)
        printf("hi :>");
    }

    // make each client send vote
    VotePacket vote;
    vote.vote = READY;
    bufferSize = vote.calculateSize();
    buf = new char[bufferSize];
    VotePacket::serialize(vote, buf);
    for (Client c : client_list) {
        c.sock_send(SERVER_RECV_BUFLEN, buf);
    }

    // confirm receipt from each client
    sc.state = LOBBY;
    sc.receive_data();
    delete[] buf;
    assert(sc.ready_players == client_list.size());

    close_and_shutdown(&sc, client_list);
    return 0;
}

int test_update() {
    // TODO: determine what update func in servercore does, prob after physics are in place
    return 0;
}

int test_send() {
    ServerCore sc;
    
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &sc, 0, &threadID);
    std::vector<Client> client_list;
    for (int i = 0; i < NUM_TEST_CLIENTS; i++) {
        // connect client to server
        Client c = Client();
        c.sock_receive(); // id can be ignored for this test, just recv to flush
        client_list.push_back(c);
        printf("connected %d\n", i);
    }
    WaitForSingleObject(hand, 1000);

    // send gamestate to all clients
    sc.send_updates();
    
    GameStatePacket packet;
    char* buf;
    // confirm receipt from server
    for (Client c : client_list) {
        buf = c.sock_receive();
        if (!buf || !buf[0] || Packet::get_packet_type(buf) != GAME_STATE) {
            close_and_shutdown(&sc, client_list);
            return 1;
        }
        GameStatePacket::deserialize(buf, packet);
        if (packet.state.level != sc.serverState.level ||
            packet.state.players.size() != sc.serverState.players.size() ||
            packet.state.students.size() != sc.serverState.students.size()) {
            close_and_shutdown(&sc, client_list);
            return 1;
        }
        for (int i = 0; i < packet.state.players.size(); i++) {
            if (packet.state.players[i].score != sc.serverState.players[i].score ||
                packet.state.players[i].world != sc.serverState.players[i].world) {
                    close_and_shutdown(&sc, client_list);
                    return 1;
                }
        }
        for (int i = 0; i < packet.state.students.size(); i++) {
            if (packet.state.students[i].world != sc.serverState.students[i].world) {
                    close_and_shutdown(&sc, client_list);
                    return 1;
                }
        }
    }

    // send heartbeat to all clients
    sc.send_heartbeat();
    ServerHeartbeatPacket hb;
    // confirm receipt
    for (Client c : client_list) {
        buf = c.sock_receive();
        if (!buf || !buf[0] || Packet::get_packet_type(buf) != SERVER_HEARTBEAT) {
            close_and_shutdown(&sc, client_list);
            return 1;
        }
        ServerHeartbeatPacket::deserialize(buf, hb);
        if (hb.state != LOBBY) {
            close_and_shutdown(&sc, client_list);
            return 1;
        }
    }

    close_and_shutdown(&sc, client_list);
    return 0;
}

int test_shutdown() {
    ServerCore sc;
    for (int i = 0; i < 4; i++)
    {
        sc.clients_data.push_back(new ClientData);
    }

    if (sc.clients_data.size() != 4)
        return 1;

    sc.shutdown();

    return (sc.isRunning() || sc.clients_data.size() != 0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("specify which test:\n./test {\"listen_accept\", \"receive\", \"update\", \"send\", \"shutdown\"}\n");
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "listen_accept")
        return test_listen_accept();
    else if (arg == "receive")
        return test_receive();
    else if (arg == "update")
        return test_update();
    else if (arg == "send")
        return test_send();
    else if (arg == "shutdown")
        return test_shutdown();
    printf("specify which test:\n./test {\"listen_accept\", \"receive\", \"update\", \"send\", \"shutdown\"}\n");
    return 1;
}