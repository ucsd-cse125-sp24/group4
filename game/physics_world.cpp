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

void PhysicsWorld::step(float dt)
{
    for (GameObject* obj : m_objects) {
        obj->force += obj->mass * m_gravity; // apply a force

        obj->velocity += obj->force / obj->mass * dt;

        // check y >= 0
        obj->position += obj->velocity * dt;

        obj->force = glm::vec3(0, 0, 0); // reset net force at the end
    }
}

