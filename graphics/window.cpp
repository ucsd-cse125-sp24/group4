#include "../include/window.h"
#include <iostream>
#include "window.h"

#define NUM_NPC 10

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
std::vector<Drawable *> Window::students;
std::vector<bool> Window::studentsChasing;

Drawable *Window::map;
Drawable *Window::exit_square;
Drawable *Window::exit_sign;
std::vector<Drawable *> Window::batteries;

short Window::player_id = 0; // 0 by default

// Camera
Camera *Window::cam;

void writeBoundingBoxToTextFile(const glm::vec3 &minVec, const glm::vec3 &maxVec, bool map=true)
{
	if (map){ 
		std::ofstream file("../game/map_stat", std::ios::app); // Open in text mode to append data
		if (!file)
		{
			std::cerr << "Failed to open the file for writing.\n";
			return;
		}

		file << minVec.x << ", " << minVec.y << ", " << minVec.z << std::endl;
		file << maxVec.x << ", " << maxVec.y << ", " << maxVec.z << "." << std::endl;
		file.close();
	}
	else{
		std::ofstream file("../game/batteries_stat", std::ios::app); // Open in text mode to append data
		if (!file)
		{
			std::cerr << "Failed to open the file for writing.\n";
			return;
		}

		file << minVec.x << ", " << minVec.y << ", " << minVec.z << std::endl;
		file << maxVec.x << ", " << maxVec.y << ", " << maxVec.z << "." << std::endl;
		file.close();
	}
	

}

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
	std::string alienGreen = "art/models/character/green_alien_wbones.fbx";
	std::string alienMint = "art/models/animation/walking/mint_alien_walking.fbx";
	std::string alienPink = "art/models/animation/walking/pink_alien_walking.fbx";
	std::string alienPurple = "art/models/animation/walking/purple_alien_walking.fbx";

	std::string boyPath = "art/models/character/boy_standing.fbx";
	std::string girlPath = "art/models/character/girl_standing.fbx";

	std::map<AnimationState, std::string> alienAnim = {
		{AnimationState::Idle, "art/models/character/green_alien_wbones.fbx"},
		{AnimationState::Walking, "art/models/animation/walking/green_alien_walking.fbx"}};

	std::map<AnimationState, std::string> boyAnim = {
		{AnimationState::Idle, "art/models/character/boy_standing.fbx"},
		{AnimationState::Walking, "art/models/animation/walking/boy_walking.fbx"},
		{AnimationState::Running, "art/models/animation/running/boy_running.fbx"}};

	std::map<AnimationState, std::string> girlAnim = {
		{AnimationState::Idle, "art/models/character/girl_standing.fbx"},
		{AnimationState::Walking, "art/models/animation/walking/girl_walking.fbx"},
		{AnimationState::Running, "art/models/animation/running/girl_running.fbx"}};

	glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100, 0));

	std::cout << "Load player" << std::endl;
	Model *player = new Model(alienGreen, alienAnim);
	player->set_color(glm::vec3(0, 1, 0)); 
	player->set_world(temp);
	players.push_back(player);

	Model *player2 = new Model(alienMint, alienAnim);
	player2->set_color(glm::vec3(1, 0, 0)); 
	player2->set_world(temp);
	players.push_back(player2);

	Model *player3 = new Model(alienPink, alienAnim);
	player3->set_color(glm::vec3(1, 0, 1));
	player3->set_world(temp);
	players.push_back(player3);

	Model *player4 = new Model(alienPurple, alienAnim);
	player4->set_color(glm::vec3(0, 0, 1));
	player4->set_world(temp);
	players.push_back(player4);

	std::cout << "Load students\n";
	for (int i = 0; i < NUM_NPC/2; i++)
	{
		Model *boy = new Model(boyPath,boyAnim);
		boy->set_color(glm::vec3(0, 0, 1));
		boy->set_world(temp);
		students.push_back(boy);
		studentsChasing.push_back(false);
		std::cout << "Done boy: " << i << "\n";

		Model *girl = new Model(girlPath, girlAnim);
		girl->set_color(glm::vec3(0, 0, 1));
		girl->set_world(temp);
		students.push_back(girl);
		studentsChasing.push_back(false);
		std::cout << "Done girl: " << i << "\n";
	}

	std::cout << "Load map\n";
	// Model* mp = new Model("art/models/chair.fbx");
	Model* mp = new Model("art/models/environment/floor2.fbx");
	mp->set_color(glm::vec3(0.5, 0.5, 0.5));

	// Scale by half
	mp->set_world(glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
	map = mp;

	glm::mat4 end_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-95, 0, 25));
	Model* end = new Model("art/models/exit_square.fbx");
	end->set_color(glm::vec3(0, 1, 0));
	end->set_world(glm::scale(end_loc, glm::vec3(0.05, 0.05, 0.05)));
	exit_square = end;

	glm::mat4 sign_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-95, 2, 25));
	Model* end_sign = new Model("art/models/exit_sign.fbx");
	end_sign->set_world(sign_loc);
	end_sign->set_world(glm::scale(sign_loc, glm::vec3(0.02, 0.02, 0.02)));
	exit_sign = end_sign;

	glm::mat4 bat1_loc = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, -3));
	Model* bat1 = new Model("art/models/battery.fbx");
	bat1->set_color(glm::vec3(1, 1, 0));
	bat1->set_world(glm::scale(bat1_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat1);

	glm::mat4 bat2_loc = glm::translate(glm::mat4(1.0f), glm::vec3(31, 0, -2.5));
	Model* bat2 = new Model("art/models/battery.fbx");
	bat2->set_color(glm::vec3(1, 1, 0));
	bat2->set_world(glm::scale(bat2_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat2);

	glm::mat4 bat3_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-40, 0, 42));
	Model* bat3 = new Model("art/models/battery.fbx");
	bat3->set_color(glm::vec3(1, 1, 0));
	bat3->set_world(glm::scale(bat3_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat3);

	glm::mat4 bat4_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-49, 0, 100));
	Model* bat4 = new Model("art/models/battery.fbx");
	bat4->set_color(glm::vec3(1, 1, 0));
	bat4->set_world(glm::scale(bat4_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat4);

	glm::mat4 bat5_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-89, 0, -46));
	Model* bat5 = new Model("art/models/battery.fbx");
	bat5->set_color(glm::vec3(1, 1, 0));
	bat5->set_world(glm::scale(bat5_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat5);

	glm::mat4 bat6_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-41.82, 0, -112.8));
	Model* bat6 = new Model("art/models/battery.fbx");
	bat6->set_color(glm::vec3(1, 1, 0));
	bat6->set_world(glm::scale(bat6_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat6);

	glm::mat4 bat7_loc = glm::translate(glm::mat4(1.0f), glm::vec3(52.65, 0, -2.6));
	Model* bat7 = new Model("art/models/battery.fbx");
	bat7->set_color(glm::vec3(1, 1, 0));
	bat7->set_world(glm::scale(bat7_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat7);

	glm::mat4 bat8_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-54, 8.13, -54));
	Model* bat8 = new Model("art/models/battery.fbx");
	bat8->set_color(glm::vec3(1, 1, 0));
	bat8->set_world(glm::scale(bat8_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat8);

	glm::mat4 bat9_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-73.12, 7.14, -21));
	Model* bat9 = new Model("art/models/battery.fbx");
	bat9->set_color(glm::vec3(1, 1, 0));
	bat9->set_world(glm::scale(bat9_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat9);

	glm::mat4 bat10_loc = glm::translate(glm::mat4(1.0f), glm::vec3(-48.66, 0, 17.5));
	Model* bat10 = new Model("art/models/battery.fbx");
	bat10->set_color(glm::vec3(1, 1, 0));
	bat10->set_world(glm::scale(bat10_loc, glm::vec3(0.01, 0.01, 0.01)));
	batteries.push_back(bat10);

	bool write_to_stat = false;
	if (write_to_stat)
	{
		for (const Mesh &mesh : mp->meshes)
		{
			if (mesh.hitbox != nullptr)
			{
				writeBoundingBoxToTextFile(glm::vec3(mp->get_world() * glm::vec4(mesh.hitbox->cubeMin, 1.0f)), glm::vec3(mp->get_world() * glm::vec4(mesh.hitbox->cubeMax, 1.0f)));
			}
		}

		for (auto drawable_bat : batteries){
			Model* battery = dynamic_cast<Model*>(drawable_bat);
			for (const Mesh &mesh : battery->meshes)
			{
				if (mesh.hitbox != nullptr)
				{
					writeBoundingBoxToTextFile(glm::vec3(battery->get_world() * glm::vec4(mesh.hitbox->cubeMin, 1.0f)), glm::vec3(battery->get_world() * glm::vec4(mesh.hitbox->cubeMax, 1.0f)), false);
				}
			}
		}

		std::cout << "Written\n";
	}

}

AnimationState Window::getAnimationState(Input *input)
{
	if (input->isKeyPressed(GLFW_KEY_W) || input->isKeyPressed(GLFW_KEY_S) || input->isKeyPressed(GLFW_KEY_A) || input->isKeyPressed(GLFW_KEY_D))
	{
		return AnimationState::Walking;
	}
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

	for (Drawable *student : students)
	{
		delete student;
	}

	delete map;
	delete exit_square;
	delete exit_sign;
	
	for (Drawable *battery : batteries)
	{
		delete battery;
	}

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
		// player->debug_draw(cam->get_view_project_mtx(), shader_program);
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

	for (int i = 0; i < students.size(); i++)
	{
		students[i]->draw(cam->get_view_project_mtx(), shader_anim_program);
		Model *model = dynamic_cast<Model *>(students[i]);
		if (studentsChasing[i])
		{
			// std::cout << "chasing!" << std::endl;
			model->updateAnimations(deltaTime, AnimationState::Running);
		}
		else
		{
			// std::cout << "walking" << std::endl;
			model->updateAnimations(deltaTime, AnimationState::Walking);
		}
	}

	int firstBattery = 1;
	for (Drawable *battery : batteries)
	{
		battery->draw(cam->get_view_project_mtx(), shader_anim_program);
		//battery->debug_draw(cam->get_view_project_mtx(), shader_program);
	}

	map->draw(cam->get_view_project_mtx(), shader_program);
	exit_square->draw(cam->get_view_project_mtx(), shader_program);
	exit_sign->draw(cam->get_view_project_mtx(), shader_program);
	//map->debug_draw(cam->get_view_project_mtx(), shader_program);

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

	// TODO: Update other fields - student, etc

	for (int i = 0; i < state.students.size(); i++)
	{
		students[i]->set_world(state.students[i].world);
		studentsChasing[i] = state.students[i].chasingPlayer;

		// std::cout << "result: " << state.students[i].world[3][0] << ", " << state.students[i].world[3][1] << ", " << state.students[i].world[3][2] << std::endl;
	}
}