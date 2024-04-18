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
    while (!glfwWindowShouldClose(window)) {
        // std::cout << "Enter message to send (q to quit): ";
        // vv this blocks the rendering loop so I commented it out
        //std::getline(std::cin, input);


        // Graphics callbacks
        // Render
        Window::display_callback(window);
        // Updates
        Window::idle_callback();


        client.sock_send(int(input.length()) + 1, input.c_str()); // Send the input to the server
    }

    Window::clean_up();
    glfwTerminate();

    client.close_conn(); // Close the connection
    printf("Client shutting down\n");
    return 0;
}