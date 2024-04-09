#include "server.h"
#include "client.h"
#include <iostream>

// CreateThread doesn't like calling an object's member function;
// call non-member function which calls the obj's member function instead
DWORD __stdcall call_listen(void* server){
   auto object = reinterpret_cast<Server*>(server);
   if (object){
     object->sock_listen();
   } 
   return 0U;
} 

int main() {
    /*
    This file currently starts a server which echoes the first message from a client
    before closing connections and shutting down. That's it, that's all it does, and
    it's hardcoded.
    This is meant to be a file for testing, so feel free to edit.
    */

    Server server = Server();

    // thread id is maybe arbitrary; research more when it's time for multiple clients
    unsigned long threadID = 0U;
    // need a thread bc calling a server's listen function will enter a while loop until a connection is made;
    // we will never reach the line instantiating the client if the main thread just gets stuck listening
    HANDLE hand = CreateThread(nullptr, 0U, &call_listen, &server, 0, &threadID);

    // instantiation automatically connects to localhost on default port (which is our default server port/addr)
    Client client = Client();

    // joins server thread to main, not super necessary tbh
    WaitForSingleObject(hand, 100000);

    // send a hardcoded "Hello, world" msg to server
    const char* str = "Hello, world";
    client.sock_send(13, str);

    // server receives whatever its first client connection (our only client rn) has sent
    char* buf = server.sock_receive(server.get_client_sock(0));
    printf("server got \"%s\" from client\n", buf);

    // send from server and have client spit out what it receives as well
    server.sock_send(server.get_client_sock(0), 13, buf);
    buf = client.sock_receive();
    printf("client got \"%s\" from server\n", buf);

    // client closes its own connection, server handles closing its conns + shutting down server
    client.close_conn();
    server.sock_shutdown();

    return 0;
}