#include "../networking/server.h"
#include "../networking/client.h"
#include "../include/server_core.h"
#include <windows.h>
#include <iostream>
#include <string>

int main() {
    Client client = Client(); // Client connects to localhost by default
    printf("Client initialized\n");

    std::string input;
    while (true) {
        std::cout << "Enter message to send (q to quit): ";
        std::getline(std::cin, input);
        
        if (input == "q") {
            break; // Exit the loop if the user types 'q'
        }

        client.sock_send(input.length() + 1, input.c_str()); // Send the input to the server
    }

    client.close_conn(); // Close the connection
    printf("Client shutting down\n");
    return 0;
}
