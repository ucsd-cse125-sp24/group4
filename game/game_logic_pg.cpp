#include "../include/server.h"
#include "../include/client.h"
#include "../include/server_core.h"
#include <windows.h>
#include <iostream>


int main() {
    ServerCore serverCore = ServerCore();

    serverCore.run();

    if (serverCore.isRunning())
        serverCore.shutdown();

    return 0;
}
