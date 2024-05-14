#include "../include/window.h"
#include <iostream>

int Window::width;
int Window::height;
const char* Window::window_title = "Graphics Client";
Shader* Window::shader_program = nullptr;
Input* Window::input = nullptr;

// Mouse
float Window::lastX = 400, Window::lastY = 300; // TODO: change if resolution changes

// TODO: Remove cubes?
std::vector<Drawable*> Window::players;

short Window::player_id = 0; // 0 by default

// Camera
Camera* Window::cam;


GLFWwindow* Window::create_window(int width, int height) {
	Window::width = width;
	Window::height = height;
	
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// For Mac OS X:
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 4x antialiasing?
	// glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window) {
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	// Call AFTER making window and making it current
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, 800, 600);

	// Shader program - maybe move somewhere else?
	// Initialize shader
	shader_program = new Shader("shaders/shader.vert", "shaders/shader.frag");
	// Initialize input
	input = new Input();

	// TODO: Set up camera here
	// Origin by default - should snap to player once scene is set up
	cam = new Camera(glm::mat4(1.0)); 
	cam->set_aspect(float(width) / float(height));

	// TODO: Initialize the interaction variables?

	// Call the resize callback to make sure things get drawn immediately.
	Window::resize_callback(window, width, height);

	// Capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void Window::setup_callbacks(GLFWwindow* window) {
	// Set the required callback functions
	/*glfwSetScrollCallback(window, Window::scroll_callback);*/

	glfwSetKeyCallback(window, Window::key_callback);
	glfwSetWindowSizeCallback(window, Window::resize_callback);
	glfwSetCursorPosCallback(window, Window::mouse_callback);

}

void Window::setup_scene() {
	// Populate players
	// 
	// unused player models sent into space
	glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100, 0));


	Model *player = new Model("art/models/green.fbx");
	player->set_color(glm::vec3(0, 1, 0)); // p1 - green
	player->set_world(temp);
	players.push_back(player);


	Model* player2 = new Model("art/models/green2.fbx");
	player2->set_color(glm::vec3(1, 0, 0)); // p2 - red
	player2->set_world(temp);
	players.push_back(player2);

	// p3 - purple
	Model* player3 = new Model("art/models/green3.fbx");
	player3->set_color(glm::vec3(1, 0, 1));
	player3->set_world(temp);
	players.push_back(player3);

	// p4 - blue
	Model* player4 = new Model("art/models/green4.fbx");
	player4->set_color(glm::vec3(0, 0, 1));
	player4->set_world(temp);
	players.push_back(player4);


	// TODO: Move to callback -- Do I need to center here...
	//cam->update(cube->get_world());
}

void Window::clean_up() {
	// Deallcoate the objects
	delete cam;
	delete input;

	for(Drawable* player : players) {
		delete player;
	}

	// Delete the shader program
	delete shader_program;

}

// CALLBACKS -------------------------------------------------------------------
void Window::resize_callback(GLFWwindow* window, int width, int height) {
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	// Camera re-set aspect ratio
	cam->set_aspect(float(width) / float(height));

	// TODO re-center the mouse
}

void Window::display_callback(GLFWwindow* window) {
	// Main rendering loop
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Render any objects you need to here
	// TODO: First set the camera to the right location
	std::cout << "I am player " << player_id << std::endl;
	cam->update(players[player_id]->get_world());

	for(Drawable* player : players) {
		player->draw(glm::rotate(cam->get_view_project_mtx(), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), shader_program);
	}


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::idle_callback() {

	// Will eventually be deprecated I think... server handles all updates

	// Perform any updates as necessary
	// This is called every frame

}

std::vector<int> Window::get_input_actions() {
	return input->get_action();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	// Use PRESS AND RELEASE to set the state of keys
	// Then have another function use the key state to update game state
	// https://www.reddit.com/r/opengl/comments/i8lv8u/how_can_i_optimize_my_key_handling_and_make_it/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		input->update(key, action);
	}
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float offsetX = (float)(xpos - lastX);
	float offsetY = (float)(lastY - ypos); // Reversed since y-coordinates range from bottom to top
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.1f; // TODO set this somewhere
	
	// Update camera
	cam->turn_azimuth(offsetX * sensitivity);
	cam->turn_incline(offsetY * sensitivity);
}

// First constrain to -180, 180
// Then convert to radians
float Window::get_cam_angle_radians() {
	float angle = cam->get_azimuth();
	while (angle > 180) {
		angle -= 360;
	}
	while (angle < -180) {
		angle += 360;
	}

	return glm::radians(angle);
}

void Window::update_state(GameState& state) {
	// player positions
	for(int i = 0; i < state.players.size(); i++) {
		players[i]->set_world(state.players[i].world);
	}
	// TODO: Update other fields - student, etc
}