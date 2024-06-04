#include "../include/physics/collider.h"

bool AABB::collide(Collider& other) {
    if (AABB* otherAABB = dynamic_cast<AABB*>(&other)) {
        return collidingAABB(*otherAABB);
    }
    // Add more collision logic for other types of colliders if needed
    return false;
}

bool AABB::collidingAABB(AABB& other) {
    glm::vec3 a_min = minExtents;
    glm::vec3 a_max = maxExtents;
    glm::vec3 b_min = other.getMinExtents();
    glm::vec3 b_max = other.getMaxExtents();

    return (
        a_min.x <= b_max.x &&
        a_max.x >= b_min.x &&
        a_min.y <= b_max.y &&
        a_max.y >= b_min.y &&
        a_min.z <= b_max.z &&
        a_max.z >= b_min.z
    );
}

void AABB::setBoundingBox(const glm::vec3& position, int type) {
    float minX, minY, minZ, maxX, maxY, maxZ;
    if (type == TYPE_PLAYER){
        minX = position.x - PLAYER_WIDTH / 2.0;
        minY = position.y;
        minZ = position.z - PLAYER_LENGTH / 2.0;
        maxX = position.x + PLAYER_WIDTH / 2.0;
        maxY = position.y + PLAYER_HEIGHT;
        maxZ = position.z + PLAYER_LENGTH / 2.0;
    }

    if (type == TYPE_NPC){
        minX = position.x - NPC_WIDTH / 2.0;
        minY = position.y;
        minZ = position.z - NPC_LENGTH / 2.0;
        maxX = position.x + NPC_WIDTH / 2.0;
        maxY = position.y + NPC_HEIGHT;
        maxZ = position.z + NPC_LENGTH / 2.0;
    }

    minExtents = glm::vec3(minX, minY, minZ);
    maxExtents = glm::vec3(maxX, maxY, maxZ);
}


glm::vec3 AABB::getCollisionNormal(Collider& other) const {
    if (const AABB* otherAABB = dynamic_cast<const AABB*>(&other)) {
        glm::vec3 centerThis = (minExtents + maxExtents) * 0.5f;
        glm::vec3 centerOther = (otherAABB->getMinExtents() + otherAABB->getMaxExtents()) * 0.5f;

        glm::vec3 normal = centerOther - centerThis;
        float length = glm::length(normal);
        if (length != 0.0f) {
            normal /= length;
        }

        return normal;
    }
    // Handle other collider types if needed
    return glm::vec3(0.0f, 0.0f, 0.0f);
}
