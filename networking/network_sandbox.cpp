#include "server.h"
#include "client.h"
#include <iostream>

DWORD __stdcall call_listen(void* server){
   auto object = reinterpret_cast<Server*>(server);
   if (object){
     object->sock_listen();
   } 
   return 0U;
} 

int main() {
    /*
    This file currently runs a server which echoes the first message from a client.
    This is meant to be a file for testing, so feel free to edit.
    */
    Server server = Server();

    //std::thread t([&]{ server.sock_listen(); });
    unsigned long threadID = 0U;
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &server, 0, &threadID);

    Client client = Client();

    WaitForSingleObject(hand, 100000);

    const char* str = "Hello, world";
    client.sock_send(13, str);

    char* buf = server.sock_receive(server.get_client_sock(0));
    printf("got \"%s\" from client\n", buf);

    server.sock_send(server.get_client_sock(0), 13, buf);

    client.close_conn();
    server.sock_shutdown();

    return 0;
}