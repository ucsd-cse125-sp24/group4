#pragma once
#include "core.h"
#include "cube.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "enums.h"
#include "game_state.h"
#include "model.h"
#include "windows.h"
#include "image.h"

enum CurrentAudio {
	NONE,
	LOADING,
	SNEAKY_BACKGROUND,
	ALERT_CHASE,
	INTENSE
};

/*
 * Window.h
 */

class Window{
public:
	// Window properties
	static int width;
	static int height;
	static const char* window_title;

	// Helper objects
	static Input* input;
	static Shader* shader_program;
	static Shader* shader_anim_program;
	static Shader* shader_image_program;

	static short player_id;
	// Objects to draw - TODO
	static std::vector<Drawable*> players;
	static std::vector<Drawable*> students;
	static std::vector<bool> studentsChasing;
	static Drawable* map;
	static Drawable* exit_square;
	static Drawable* exit_square2;
	static Drawable* exit_sign;
	static std::vector<Drawable*> batteries;

	static Image* winScreen;
	static Image* loseScreen;
	static std::vector<Image*> progress;
	static int score;

	// Camera
	static Camera* cam;

	// Audio
	static CurrentAudio currAudio;
	static time_t audio_finish_time;

	// For tracking the mouse
	static float lastX;
	static float lastY;
	static float lastFrameTime;


	// INITIALIZATION ----------------------------------------------------------
	static GLFWwindow* create_window(int width, int height);
	
	static void setup_callbacks(GLFWwindow* window);
	static void setup_scene();

    static AnimationState getAnimationState(Input *input);

    // DESTRUCTOR
	static void clean_up();
	
	// TODO callbacks
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void display_callback(GLFWwindow* window);
	static void idle_callback();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	// Helper to let client poll for events
	static std::vector<int> get_input_actions();
	static float get_cam_angle_radians();

	static void update_state(GameState& state);
    static float calculateDeltaTime();

	static void draw_lose(GLFWwindow* window);
	static void draw_win(GLFWwindow* window);
};