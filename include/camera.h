/*
 *  Camera that centers upon a provided target position
 *  Must provide an initial position upon construction
 *  Will either use the previously saved position or a new provided position
 *  when Updated
 *  Can control the zoom and rotation through polar controls
 */

#pragma once
#include "core.h"

// "GLM: GLM_GTX_euler_angles is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

class Camera {
public:
	Camera(glm::mat4 start);

    // Update about a fixed position
    void update();
    // Update about a target that has moved
    void update(glm::mat4 target);
    // Do I need a reset?

    // Polar controls
    // TODO ZOOM?
    void turn_azimuth(float angle) { azimuth -= angle; } // Also inverted
    void turn_incline(float angle) { incline -= angle; if (incline < 0) incline = 0; if (incline >= 90) incline = 89.999f;  } // Inverted

    // Getters and setters
    void set_aspect(float a) { aspect = a; }
	float get_azimuth() { return azimuth; }
    glm::mat4 get_view_project_mtx() { return view_project_mtx; }

private:
	// Perspective controls
    float fov;       // Field of View Angle (degrees)
    float aspect;    // Aspect Ratio
    float near_clip;  // Near clipping plane distance
    float far_clip;   // Far clipping plane distance

    // Polar controls
    float distance;  // Distance of the camera eye position to the tracked target
    float azimuth;   // Rotation of the camera eye position around the Y axis (degrees)
    float incline;   // Angle of the camera eye position over the XZ plane (degrees)

    // Computed data
    glm::mat4 prev_position;
    glm::mat4 view_project_mtx;
};