#include "../include/physics/aabb.h"

bool AABB::collidingAABB(AABB& other) {
    glm::vec3 a_min = minExtents;
    glm::vec3 a_max = maxExtents;
    glm::vec3 b_min = other.getMinExtents;
    glm::vec3 b_max = other.getMaxExtents;


    return (
        a_min.x <= b_max.x &&
        a_max.x >= b_min.x &&
        a_min.y <= b_max.y &&
        a_max.y >= b_min.y &&
        a_min.z <= b_max.z &&
        a_max.z >= b_min.z
    );
}