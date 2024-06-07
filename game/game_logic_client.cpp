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

    while (clientCore.is_connected()) {
        // display image
        clientCore.receive_updates();
        clientCore.process_server_data();
        clientCore.renderGameState();
        clientCore.send_input();

        if (clientCore.server_state == END_TOTAL_LOSE) {
            printf("oh no, your team lost\n");
            //clientCore.shutdown();
        }
        if (clientCore.server_state == END_WIN) {
            printf("congrats your team won\n");
            //clientCore.shutdown();
        }
    }

    return 0;
    // getting a segfault after return, some memory err or smth? anyway it's not gamebreaking so
}