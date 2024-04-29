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

    serverCore.initialize(); // Setup server

    serverCore.students.push_back(new StaticStudent(10, 10, 0));
    serverCore.students.push_back(new MovingStudent(20, 20, 0, 10, 1.0f));

    Server server = serverCore.server;
    serverCore.accept_new_clients();

    serverCore.run();

    serverCore.shutdown();


    return 0;
}
