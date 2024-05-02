#include "../include/server.h"
#include "../include/client.h"
#include "../include/client_core.h"
#include "../include/core.h"
#include "../include/graphics.h"
#include <windows.h>
#include <iostream>
#include <string>

int main() {
    ClientCore clientCore = ClientCore(); // Client connects to localhost by default
    clientCore.initialize();

    clientCore.run();

    if (clientCore.is_connected()) {
        clientCore.shutdown();
    }

    return 0;
}