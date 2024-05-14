#include "../include/physics_world.h"

void PhysicsWorld::addObject (GameObject* object) { 
    m_objects.push_back(object);
}

void PhysicsWorld::removeObject (GameObject* object) { 
    // Find the object in the vector
    auto it = std::find(m_objects.begin(), m_objects.end(), object);
    
    // If the object is found, erase it
    if (it != m_objects.end()) {
        m_objects.erase(it);
        //delete object->collider;
        delete object;
    }
}

void PhysicsWorld::addPlayer (PlayerObject* player) {
    p_objects.push_back(player);
}

void PhysicsWorld::removePlayer (PlayerObject* player) {
    // Find the object in the vector
    auto it = std::find(p_objects.begin(), p_objects.end(), player);
    
    // If the object is found, erase it
    if (it != p_objects.end()) {
        p_objects.erase(it);
        //delete object->collider;
        delete player;
    }
}

PlayerObject* PhysicsWorld::findPlayer(int id) {
    for (PlayerObject* player : p_objects) {
        if (player->playerId == id) {
            return player;
        }
    }
    printf("Error: player with id %d does not exist.\n", id);
    return NULL;
}

void PhysicsWorld::step()
{
    float dt = 0.02f;
    for (GameObject* obj : m_objects) {
        obj->force += obj->mass * m_gravity; // apply a force

        glm::vec3 ori_v = obj->velocity;
        obj->velocity += obj->force / obj->mass * dt;

        // check y >= 0
        obj->position += 0.5f * (ori_v + obj->velocity) * dt;
        obj->position = glm::vec3(0, 0, 0);
        printf("positions: <%f, %f, %f>\n", obj->position.x, obj->position.y, obj->position.z);
        obj->force = glm::vec3(0, 0, 0); // reset net force at the end

        // TBD: add a check for y falling below 0, and response
    }

    hasCollision(); // update position and then check for collision?
    
}

bool PhysicsWorld::hasCollision() {
    for (PlayerObject* player : p_objects) {
        // treat center point as a collision point for now
        glm::vec3 point(0.0f, 0.0f, 0.0f);
        glm::vec3 min = player->minBound;
        glm::vec3 max = player->maxBound;

        // check if cube/box colliding with the the point
        return (
            point.x >= min.x &&
            point.x <= max.x &&
            point.y >= min.y &&
            point.y <= max.y &&
            point.z >= min.z &&
            point.z <= max.z
        );
    }
    return false;
}
