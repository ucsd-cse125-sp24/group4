#include "../include/camera.h"

Camera::Camera(glm::mat4 target) {
	// Set default values
    fov = 45.0f;
    aspect = 1.33f;
    near_clip = 0.1f;
    far_clip = 100.0f;

    distance = 10.0f;
    azimuth = 0.0f;
    incline = 20.0f;

    prev_position = target;
}

void Camera::update() {
    //glm::mat4 world = prev_position;
    //world[3][2] = distance;

    //world = glm::eulerAngleY(glm::radians(-azimuth)) * glm::eulerAngleX(glm::radians(-incline)) * world;

    glm::mat4 target = prev_position;
    glm::vec3 targetPosition = glm::vec3(target[3]);
    glm::vec3 cameraPosition;
    cameraPosition.x = targetPosition.x + distance * cos(glm::radians(incline)) * sin(glm::radians(azimuth));
    cameraPosition.y = targetPosition.y + distance * sin(glm::radians(incline));
    cameraPosition.z = targetPosition.z + distance * cos(glm::radians(incline)) * cos(glm::radians(azimuth));

    // Calculate camera's direction vector
    glm::vec3 direction = glm::normalize(targetPosition - cameraPosition);

    // Calculate camera's up vector based on incline angle
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Assuming up is always in the positive y direction
    if (incline > 90.0f && incline < 270.0f) {
        up = glm::vec3(0.0f, -1.0f, 0.0f); // Flip up vector if incline angle is upside down
    }

    // Construct view matrix
    glm::mat4 view = glm::lookAt(cameraPosition, targetPosition, up);

    //// Compute view matrix (inverse of world matrix)
    //glm::mat4 view = glm::inverse(world);

    // Compute perspective projection matrix
    glm::mat4 project = glm::perspective(glm::radians(fov), aspect, near_clip, far_clip);

    // Compute view-projection matrix
    view_project_mtx = project * view;
}

void Camera::update(glm::mat4 target) {
    prev_position = target;
    update();
}