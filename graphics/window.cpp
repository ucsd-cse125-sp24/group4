#include "../include/window.h"
#include <iostream>
#include "window.h"

int Window::width;
int Window::height;
const char *Window::window_title = "Graphics Client";
Shader *Window::shader_program = nullptr;
Shader *Window::shader_anim_program = nullptr;
Input *Window::input = nullptr;

// Mouse
float Window::lastX = 400, Window::lastY = 300; // TODO: change if resolution changes
float Window::lastFrameTime = 0.0f;

// Drawable objects
std::vector<Drawable *> Window::players;
Drawable *Window::map;

short Window::player_id = 0; // 0 by default

// Camera
Camera *Window::cam;

GLFWwindow *Window::create_window(int width, int height)
{
	Window::width = width;
	Window::height = height;

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// For Mac OS X:
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 4x antialiasing?
	// glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow *window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
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
	shader_anim_program = new Shader("shaders/shader_anim.vert", "shaders/shader.frag");
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

void Window::setup_callbacks(GLFWwindow *window)
{
	// Set the required callback functions
	/*glfwSetScrollCallback(window, Window::scroll_callback);*/

	glfwSetKeyCallback(window, Window::key_callback);
	glfwSetWindowSizeCallback(window, Window::resize_callback);
	glfwSetCursorPosCallback(window, Window::mouse_callback);
}

void Window::setup_scene()
{
	// Populate players
	//
	// unused player models sent into space

	std::map<AnimationState, std::string> animationPath = {
		{AnimationState::Idle, "art/models/character/green_alien_wbones.fbx"},
		{AnimationState::Walking, "art/models/animation/walking/green_alien_walking.fbx"}};

	std::string alienPath = "art/models/character/green_alien_wbones.fbx";

	glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100, 0));

	Model *player = new Model(alienPath, animationPath);
	player->set_color(glm::vec3(0, 1, 0)); // p1 - green
	player->set_world(temp);
	players.push_back(player);

	Model *player2 = new Model(alienPath, animationPath);
	player2->set_color(glm::vec3(1, 0, 0)); // p2 - red
	player2->set_world(temp);
	players.push_back(player2);

	Model *player3 = new Model(alienPath, animationPath);
	player3->set_color(glm::vec3(1, 0, 1)); //p3 - purple
	player3->set_world(temp);
	players.push_back(player3);

	Model *player4 = new Model(alienPath, animationPath);
	player4->set_color(glm::vec3(0, 0, 1)); // p4 - blue
	player4->set_world(temp);
	players.push_back(player4);

	// Floor 6_empty works without rotations

	Model* mp = new Model("art/models/chair.fbx");
	//Model* mp = new Model("art/models/environment/floor2.fbx");
	mp->set_color(glm::vec3(0.5, 0.5, 0.5));
	mp->set_world(glm::mat4(1.0f));
	map = mp;
}

AnimationState Window::getAnimationState(Input *input)
{
	if (input->isKeyPressed(GLFW_KEY_W) || input->isKeyPressed(GLFW_KEY_S) || input->isKeyPressed(GLFW_KEY_A) || input->isKeyPressed(GLFW_KEY_D))
	{
		return AnimationState::Walking;
	}
	// else {
	//     return AnimationState::Idle;
	// }
	// return AnimationState::Idle;
	return AnimationState::Idle;
}

float ::Window::calculateDeltaTime()
{
	float currentFrameTime = glfwGetTime();
	float deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	return deltaTime;
}

void Window::clean_up()
{
	// Deallcoate the objects
	delete cam;
	delete input;

	for (Drawable *player : players)
	{
		delete player;
	}

	delete map;

	// Delete the shader program
	delete shader_program;
	delete shader_anim_program;
}

// CALLBACKS -------------------------------------------------------------------
void Window::resize_callback(GLFWwindow *window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	// Camera re-set aspect ratio
	cam->set_aspect(float(width) / float(height));

	// TODO re-center the mouse
}

void Window::display_callback(GLFWwindow *window)
{
	// Main rendering loop
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Render any objects you need to here
	// First set the camera to the right location
	cam->update(players[player_id]->get_world());

	float deltaTime = calculateDeltaTime();
	

	for (Drawable *player : players)
	{
		player->draw(cam->get_view_project_mtx(), shader_anim_program);
		player->debug_draw(cam->get_view_project_mtx(), shader_program);
		Model *model = dynamic_cast<Model *>(player);
		if (model)
		{
			AnimationState currentState = getAnimationState(input);
			if (currentState != AnimationState::Idle)
			{
				model->updateAnimations(deltaTime, currentState);
			}
		}
	}

	map->draw(cam->get_view_project_mtx(), shader_program);
	// map->debug_draw(cam->get_view_project_mtx(), shader_program);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::idle_callback()
{

	// Will eventually be deprecated I think... server handles all updates

	// Perform any updates as necessary
	// This is called every frame
}

std::vector<int> Window::get_input_actions()
{
	return input->get_action();
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	// Use PRESS AND RELEASE to set the state of keys
	// Then have another function use the key state to update game state
	// https://www.reddit.com/r/opengl/comments/i8lv8u/how_can_i_optimize_my_key_handling_and_make_it/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		input->update(key, action);
	}
}

void Window::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
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
float Window::get_cam_angle_radians()
{
	float angle = cam->get_azimuth();
	while (angle > 180)
	{
		angle -= 360;
	}
	while (angle < -180)
	{
		angle += 360;
	}

	return glm::radians(angle);
}

void Window::update_state(GameState &state)
{
	// player positions
	for (int i = 0; i < state.players.size(); i++)
	{
		players[i]->set_world(state.players[i].world);
	}

	// float deltaTime = calculateDeltaTime();

	// AnimationState currentState = getAnimationState(input);

	// for (auto &player : players)
	// {
	// 	Model *model = dynamic_cast<Model *>(player);
	// 	if (model)
	// 	{
	// 		model->updateAnimations(deltaTime, currentState);
	// 	}
	// }

	// TODO: Update other fields - student, etc
}