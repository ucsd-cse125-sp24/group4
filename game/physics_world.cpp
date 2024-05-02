#include "../include/physics_world.h"

void PhysicsWorld::AddObject   (GameObject* object) { 
    m_objects.push_back(object);
}

void PhysicsWorld::RemoveObject(GameObject* object) { 
    // Find the object in the vector
    auto it = std::find(m_objects.begin(), m_objects.end(), object);
    
    // If the object is found, erase it
    if (it != m_objects.end()) {
        m_objects.erase(it);
        //delete object->collider;
        delete object;
    }
}

void PhysicsWorld::Step(float dt)
{
    for (GameObject* obj : m_objects) {
        obj->force += obj->mass * m_gravity; // apply a force

        obj->velocity += obj->force / obj->mass * dt;

        // check y >= 0
        obj->position += obj->velocity * dt;

        obj->force = glm::vec3(0, 0, 0); // reset net force at the end
    }
}

