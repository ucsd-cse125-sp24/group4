#include "../networking/server.h"
#include "../networking/client.h"
#include "../include/server_core.h"
#include <windows.h>
#include <iostream>

int main() {

    Client client = Client(); // Client connects to localhost by default
    printf("initialized client\n");
    Sleep(1000);
    const char* str = "Hello, world";
    client.sock_send(strlen(str) + 1, str);
    client.close_conn();
    return 0;
}
