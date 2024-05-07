#include "../include/server.h"
#include "../include/client.h"
#include "../include/server_core.h"
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

    //Client client = Client(); // Client connects to localhost by default
    // TODO: connect another client in another process, then initialize will unblock
    Server server = serverCore.server;


    //const char* str = "Hello, world!";
    //client.sock_send(strlen(str) + 1, str);
    serverCore.run();


    // Clean up: close client connection, stop server
    //client.close_conn();
    serverCore.shutdown();


    return 0;
}
