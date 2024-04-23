#include "../include/server.h"
#include "../include/client.h"
#include "../include/client_core.h"
#include <windows.h>
#include <iostream>
#include <string>

int main() {
    ClientCore clientCore = ClientCore(); // Client connects to localhost by default
    clientCore.initialize();

    clientCore.run();
    clientCore.shutdown();

    return 0;
}
