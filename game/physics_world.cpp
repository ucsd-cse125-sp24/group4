#include "../include/physics/physics_world.h"
#define COLLISION_FORCE_FACTOR 1.0f

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
        // TODO: add a check for y falling below 0, and response
    }

    handleCollisions(); // update position and then check for collision?
    
}

void PhysicsWorld::handleCollisions() {
    // Loop through each combination of distinct pairs of objects
    for (unsigned int i = 0; i < p_objects.size(); i++) {
        for (unsigned int j = i + 1; j < p_objects.size(); j++) {
            // Check if either object pointer is null
            if (!p_objects[i] || !p_objects[j]) {
                std::cout << "Error: Null player object detected." << std::endl;
                continue;
            }

            // Get the colliders
            Collider& collider1 = p_objects[i]->getCollider();
            Collider& collider2 = p_objects[j]->getCollider();

            // Check for collision using references
            bool collision = collider1.collide(collider2);
            //printf("bbox %f, %f, %f \n", collider1.maxExtents[0], collider1.maxExtents[1], collider1.maxExtents[2]);
            //printf("bbox %f, %f, %f \n\n", collider2.maxExtents[0], collider2.maxExtents[1], collider2.maxExtents[2]);
            if (collision) {
                std::cout << "Collision happened between object " << i << " and object " << j << std::endl;

                glm::vec3 collision_dir = collider1.getCollisionNormal(collider2);
                //glm::vec3 collision_force = collision_dir * COLLISION_FORCE_FACTOR; // later use penetrationDepth ?
                //glm::vec3 collision_vel = collision_dir * (p_objects[i]->getVelocity() + p_objects[j]->getVelocity()); // later use penetrationDepth ?

                //p_objects[i]->applyForce(-collision_force);
                //p_objects[j]->applyForce(collision_force);
                // p_objects[i]->applyForce(-collision_force);
                // p_objects[j]->applyForce(collision_force);
                p_objects[i]->setVelocity(-collision_dir * 20.0f);
                p_objects[j]->setVelocity(collision_dir * 20.0f);
                if (p_objects[i]->getPosition() != p_objects[j]->getPosition()) {
                    p_objects[i]->setPosition(p_objects[i]->getOldPosition() - collision_dir * 0.5f);
                    p_objects[j]->setPosition(p_objects[j]->getOldPosition() + collision_dir * 0.5f);
                }
                else {
                    p_objects[i]->setPosition(p_objects[i]->getPosition() + 1.0f);
                    p_objects[j]->setPosition(p_objects[j]->getPosition());
                }
                p_objects[i]->setForce(glm::vec3(0.0));
                p_objects[j]->setForce(glm::vec3(0.0));
            }
        }
    }
}

