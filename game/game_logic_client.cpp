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
    std::vector<int> events;
    while (!glfwWindowShouldClose(window)) {
        // std::cout << "Enter message to send (q to quit): ";
        // TODO: replace this getline for text with a pollevents to get keyboard input
        //std::getline(std::cin, input);
        events = Window::get_input_actions();
        input = "";
        for (int event : events)
            input += std::to_string(event) + ","; // be aware that this adds an extra ',' at the end
        // TODO: convert input to packet once that's ready

        if (input[0])
            client.sock_send(int(input.length()) + 1, input.c_str()); // Send the input to the server

        // Graphics callbacks
        // Render
        Window::display_callback(window);
        // Updates (handle server side but display each player's updated screen client-side?)
        Window::idle_callback();
    }

    Window::clean_up();
    glfwTerminate();

    client.close_conn(); // Close the connection
    printf("Client shutting down\n");
    return 0;
}