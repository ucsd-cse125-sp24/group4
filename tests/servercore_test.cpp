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
    if (sc.isRunning()) {
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
    
    for (PlayerState ps : sc.serverState.players) {
        if (ps.world != glm::mat4(1.0f) || ps.score != 0) {
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
    
    // make each client send some data
    InputPacket packet;
    packet.events.push_back(0);
    packet.events.push_back(1);
    packet.cam_angle = 2.0f;
    size_t bufferSize = packet.calculateSize();
    char* buf = new char[bufferSize];;
    InputPacket::serialize(packet, buf);
    
    for (Client c : client_list) {
        c.sock_send((int)bufferSize, buf);
    }
    delete[] buf;

    // confirm receipt from each client
    sc.receive_data();
    for (PlayerState ps : sc.serverState.players) {
        // TODO: test serverstate's players' worlds all align with sent data
        printf("hi :>");
    }

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
        client_list.push_back(Client());
        printf("connected %d\n", i);
    }
    WaitForSingleObject(hand, 1000);

    // send data to all clients
    sc.send_updates();
    
    char* buf;
    GameStatePacket packet;
    // confirm receipt from server
    for (Client c : client_list) {
        buf = c.sock_receive();
        if (!buf || !buf[0]) {
            close_and_shutdown(&sc, client_list);
            return 1;
        } else {
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