#include "server.h"
#include "client.h"
#include "server_core.h"
#include <windows.h>
#include <iostream>

// // Non-member function to allow threading for server listening
// DWORD __stdcall call_listen(void* server){
//     auto object = reinterpret_cast<Server*>(server);
//     if (object){
//         object->sock_listen();
//     }
//     return 0U;
// } 

int main() {
    ServerCore serverCore = ServerCore(); // Assuming ServerCore manages the server

    serverCore.initialize(); // Setup server
    Client client = Client(); // Client connects to localhost by default
    Server server = serverCore.server;
    serverCore.acceptNewClients();


    const char* str = "Hello, world!";
    client.sock_send(strlen(str) + 1, str);
    serverCore.run();


    // Clean up: close client connection, stop server
    client.close_conn();
    serverCore.shutdown();


    return 0;
}
