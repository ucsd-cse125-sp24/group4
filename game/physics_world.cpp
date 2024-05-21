#include "../include/physics/physics_world.h"

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
        if (player->getPlayerId() == id) {
            return player;
        }
    }
    printf("Error: player with id %d does not exist.\n", id);
    return NULL;
}

void PhysicsWorld::step()
{
    float dt = 1.0f;
    for (GameObject* obj : m_objects) {
        //obj->force += obj->mass * m_gravity; // apply a force


        // glm::vec3 ori_v = obj->velocity;
        // obj->velocity += obj->force / obj->mass * dt;

        // // check y >= 0
        // obj->old_position = obj->position;
        // obj->position += 0.5f * (ori_v + obj->velocity) * dt;
        // printf("positions: <%f, %f, %f>\n", obj->position.x, obj->position.y, obj->position.z);
        // obj->force = glm::vec3(0, 0, 0); // reset net force at the end
        // //obj->velocity = glm::vec3(0, 0, 0);
        // printf("forces: <%f, %f, %f>\n", obj->force.x, obj->force.y, obj->force.z);

        if (obj != nullptr) {
            obj->simulate(dt);
        }
        // TBD: add a check for y falling below 0, and response
    }

    handleCollisions(); // update position and then check for collision?
    
}

void PhysicsWorld::handleCollisions() {
    for (unsigned int i = 0; i < p_objects.size(); i++) {
       for (unsigned int j = i + 1; j < p_objects.size(); j++) {
            // bool collision = p_objects[i]->getCollider()->collide(p_objects[j].getCollider());

            // if (collision) {
            //     printf("Collision happened");
            // }
       }
    }
}
