#include "window.h"

int Window::width;
int Window::height;
const char* Window::window_title = "Graphics Client";
Shader* Window::shader_program = nullptr;

// TODO: Replace with Scene later
Cube* Window::cube = nullptr;
Cube* cube2 = nullptr;

// Camera
Camera* cam;


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

	// TODO: Set up camera here
	// Origin by default - should snap to player once scene is set up
	cam = new Camera(glm::mat4(1.0)); 
	cam->set_aspect(float(width) / float(height));

	// TODO: Initialize the interaction variables?

	// Call the resize callback to make sure things get drawn immediately.
	Window::resize_callback(window, width, height);

	return window;
}

void Window::setup_callbacks(GLFWwindow* window) {
	// Set the required callback functions

	/* glfwSetCursorPosCallback(window, Window::mouse_callback);
	glfwSetScrollCallback(window, Window::scroll_callback);*/

	glfwSetKeyCallback(window, Window::key_callback);
	glfwSetWindowSizeCallback(window, Window::resize_callback);
}

void Window::setup_scene() {
	cube = new Cube();
	cube2 = new Cube();
	cube2->set_color(glm::vec3(1, 0, 0));

	cam->update(cube->get_world());
}

void Window::clean_up() {
	// Deallcoate the objects
	delete cube;
	delete cube2;
	delete cam;

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
}

void Window::display_callback(GLFWwindow* window) {
	// Main rendering loop
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Render any objects you need to here

	cube->draw(cam->get_view_project_mtx(), shader_program);
	cube2->draw(cam->get_view_project_mtx(), shader_program);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(window);

}

void Window::idle_callback() {
	// Perform any updates as necessary
	// This is called every frame
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// This is FOR TYPING
	// TODO:
	// Use PRESS AND RELEASE to set the state of keys
	// Then have another function use the key state to update game state
	// https://www.reddit.com/r/opengl/comments/i8lv8u/how_can_i_optimize_my_key_handling_and_make_it/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			// Forward
			// TODO: Abstract into EVENTS for the server
			cube->move(glm::vec3(0, 0, -1));
			cam->update(cube->get_world());
			break;
		case GLFW_KEY_S:
			// Back
			cube->move(glm::vec3(0, 0, 1));
			cam->update(cube->get_world());
			break;
		case GLFW_KEY_A:
			// Left
			cube->move(glm::vec3(-1, 0, 0));
			cam->update(cube->get_world());
			break;
		case GLFW_KEY_D:
			// Right
			cube->move(glm::vec3(1, 0, 0));
			cam->update(cube->get_world());
			break;

		// TODO: Reset camera? once we have one


		}
	}
}