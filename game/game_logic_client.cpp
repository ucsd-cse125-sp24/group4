#include "../include/server.h"
#include "../include/client.h"
#include "../include/server_core.h"
#include "../include/core.h"
#include "../include/graphics.h"
#include <windows.h>
#include <iostream>
#include <string>

int main() {
    Client client = Client(); // Client connects to localhost by default
    printf("Client initialized\n");

    // Graphics
    GLFWwindow* window = Graphics::set_up_window();

    std::string input;
    while (true) {
        std::cout << "Enter message to send (q to quit): ";
        //std::getline(std::cin, input);

        // Graphics callbacks
        // Render
        Window::display_callback(window);
        // Updates
        Window::idle_callback();
        
        while (!glfwWindowShouldClose) {
            Window::clean_up();
            glfwTerminate();
            break; // Exit the loop if the user types 'q'
        }

        client.sock_send(int(input.length()) + 1, input.c_str()); // Send the input to the server
    }

    client.close_conn(); // Close the connection
    printf("Client shutting down\n");
    return 0;
}