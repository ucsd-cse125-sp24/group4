#include "../include/client_core.h"
#include "../include/server.h"

#define NUM_TEST_CLIENTS 4

DWORD __stdcall call_init(void* clientcore){
   auto object = reinterpret_cast<ClientCore*>(clientcore);
   object->initialize();
   return 0U;
} 

int test_initialize() {
    ClientCore cc;

    if (cc.client.is_connected() || cc.connected) { // shouldn't be connected yet
        cc.shutdown();
        return 1;
    }
    
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_init, &cc, 0, &threadID);

    Server server;
    server.sock_listen();

    WaitForSingleObject(hand, 2000);

    if (!cc.client.is_connected() || !cc.connected || cc.id != 0) { // should be connected
        cc.shutdown();
        server.sock_shutdown();
        return 1;
    }

    cc.shutdown();
    server.sock_shutdown();
    return 0;
}

int test_receive() {
    ClientCore cc;
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_init, &cc, 0, &threadID);
    Server server;
    server.sock_listen();
    WaitForSingleObject(hand, 2000);
    
    // make server send some state packet
    GameStatePacket packet;
    packet.state.level = 1;
    for (int i = 0; i < NUM_TEST_CLIENTS; i++) {
        PlayerState ps;
        ps.world = glm::mat4((float)i);
        ps.score = i;
        packet.state.players.push_back(ps);

        StudentState ss;
        ss.world = glm::mat4((float)i);
        packet.state.students.push_back(ss);
    }
    size_t bufferSize = packet.calculateSize();
    char *buffer = new char[bufferSize];
    GameStatePacket::serialize(packet, buffer);
    server.sock_send(server.get_client_sock(0), (int)bufferSize, buffer);

    // confirm receipt from server
    cc.receive_updates();
    if (cc.world_state.level != packet.state.level ||
        cc.world_state.players.size() != packet.state.players.size() ||
        cc.world_state.students.size() != packet.state.students.size()) {
        cc.shutdown();
        server.sock_shutdown();
        return 1;
    }
    for (int i = 0; i < cc.world_state.players.size(); i++) {
        if (cc.world_state.players[i].world != packet.state.players[i].world ||
            cc.world_state.players[i].score != packet.state.players[i].score) {
            cc.shutdown();
            server.sock_shutdown();
            return 1;
        }
    }
    for (int i = 0; i < cc.world_state.students.size(); i++) {
        if (cc.world_state.students[i].world != packet.state.students[i].world) {
            cc.shutdown();
            server.sock_shutdown();
            return 1;
        }
    }

    cc.shutdown();
    server.sock_shutdown();
    return 0;
}

int test_process() {
    // TODO: determine what process func in clientcore does
    return 0;
}

int test_render() {
    // TODO: idk if this can even be tested we'll see
    return 0;
}

int test_send() {
    ClientCore cc;
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_init, &cc, 0, &threadID);
    Server server;
    server.sock_listen();
    WaitForSingleObject(hand, 2000);

    // send data to server
    InputPacket in_packet;
    in_packet.events.push_back(0);
    in_packet.events.push_back(1);
    in_packet.cam_angle = 2.0f;
    size_t bufferSize = in_packet.calculateSize();
    char* buf = new char[bufferSize];;
    InputPacket::serialize(in_packet, buf);
    
    cc.send_input(); // TODO: clientcore needs to be able to specify inputpacket somehow, either param or some member var
    delete[] buf;

    // confirm receipt from client
    char* server_buf = server.sock_receive(server.get_client_sock(0));
    InputPacket packet;
    InputPacket::deserialize(server_buf, packet);
    if (in_packet.cam_angle != packet.cam_angle ||
        in_packet.events.size() != packet.events.size()) {
        cc.shutdown();
        server.sock_shutdown();
        return 1;
    }
    for (int i = 0; i < packet.events.size(); i++) {
        if (in_packet.events[i] != packet.events[i]) {
            cc.shutdown();
            server.sock_shutdown();
            return 1;
        }
    }

    cc.shutdown();
    server.sock_shutdown();
    return 0;
}

int test_shutdown() {
    ClientCore cc;
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_init, &cc, 0, &threadID);
    Server server;
    server.sock_listen();
    WaitForSingleObject(hand, 2000);

    if (!cc.client.is_connected() || !cc.connected) {
        cc.shutdown();
        server.sock_shutdown();
        return 1;
    }

    cc.shutdown();
    server.sock_shutdown();

    return (cc.client.is_connected() || cc.connected);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("specify which test:\n./test {\"initialize\", \"receive\", \"process\", \"render\", \"send\", \"shutdown\"}\n");
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "initialize")
        return test_initialize();
    else if (arg == "receive")
        return test_receive();
    else if (arg == "process")
        return test_process();
    else if (arg == "render")
        return test_render();
    else if (arg == "send")
        return test_send();
    else if (arg == "shutdown")
        return test_shutdown();
    printf("specify which test:\n./test {\"initialize\", \"receive\", \"process\", \"render\", \"send\", \"shutdown\"}\n");
    return 1;
}