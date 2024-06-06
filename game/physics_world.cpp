#include "../include/physics/physics_world.h"
#define COLLISION_FORCE_FACTOR 1.0f

void PhysicsWorld::addObject(GameObject *object)
{
    m_objects.push_back(object);
}

void PhysicsWorld::removeObject(GameObject *object)
{
    // Find the object in the vector
    auto it = std::find(m_objects.begin(), m_objects.end(), object);

    // If the object is found, erase it
    if (it != m_objects.end())
    {
        m_objects.erase(it);
        // delete object->collider;
        delete object;
    }
}

void PhysicsWorld::addNPC(GameObject *object)
{
    s_objects.push_back(object);
}

void PhysicsWorld::removeNPC(GameObject *object)
{
    // Find the object in the vector
    auto it = std::find(s_objects.begin(), s_objects.end(), object);

    // If the object is found, erase it
    if (it != s_objects.end())
    {
        s_objects.erase(it);
        // delete object->collider;
        delete object;
    }
}

void PhysicsWorld::addBatteries(GameObject *object)
{
    b_objects.push_back(object);
}

void PhysicsWorld::removeBatteries(GameObject *object)
{
    // Find the object in the vector
    auto it = std::find(b_objects.begin(), b_objects.end(), object);

    // If the object is found, erase it
    if (it != b_objects.end())
    {
        b_objects.erase(it);
        delete object;
    }
}

void PhysicsWorld::addPlayer(PlayerObject *player)
{
    p_objects.push_back(player);
}

void PhysicsWorld::removePlayer(PlayerObject *player)
{
    // Find the object in the vector
    auto it = std::find(p_objects.begin(), p_objects.end(), player);

    // If the object is found, erase it
    if (it != p_objects.end())
    {
        p_objects.erase(it);
        // delete object->collider;
        delete player;
    }
}

void PhysicsWorld::cleanup()
{
    for (auto it = p_objects.begin(); it != p_objects.end(); ++it)
    {
        delete *it;
    }
    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        delete *it;
    }
    for (auto it = b_objects.begin(); it != b_objects.end(); ++it) {
        delete *it;
    }

    p_objects.clear();
    m_objects.clear();
    b_objects.clear();
}

PlayerObject *PhysicsWorld::findPlayer(int id)
{
    for (PlayerObject *player : p_objects)
    {
        if (player->getPlayerId() == id)
        {
            return player;
        }
    }
    printf("Error: player with id %d does not exist.\n", id);
    return NULL;
}

void PhysicsWorld::step()
{
    float dt = 1.0f;
    for (PlayerObject *obj : p_objects)
    {

        if (obj != nullptr)
        {
            obj->simulate_player(dt);
        }

    }

    handleCollisions(); // update position and then check for collision?
}

void PhysicsWorld::step_student(StudentState &s)
{
    float dt = 1.0f;
    s.physicalObject->setPosition(s.world[3]);
    s.physicalObject->simulate(dt, s.world); // for NPC student
    s.world[3] = glm::vec4(s.physicalObject->getPosition(), 1.0f);
    handleCollisionsNPC();

}

void PhysicsWorld::handleCollisions()
{
    // Loop through each combination of distinct pairs of players
    for (unsigned int i = 0; i < p_objects.size(); i++)
    {
        for (unsigned int j = i + 1; j < p_objects.size(); j++)
        {
            // Check if either object pointer is null
            if (!p_objects[i] || !p_objects[j])
            {
                std::cout << "Error: Null player object detected." << std::endl;
                continue;
            }

            // Get the colliders
            Collider &collider1 = p_objects[i]->getCollider();
            Collider &collider2 = p_objects[j]->getCollider();

            // Check for collision using references
            bool collision = collider1.collide(collider2);
            // printf("bbox %f, %f, %f \n", collider1.maxExtents[0], collider1.maxExtents[1], collider1.maxExtents[2]);
            // printf("bbox %f, %f, %f \n\n", collider2.maxExtents[0], collider2.maxExtents[1], collider2.maxExtents[2]);
            if (collision)
            {
                // std::cout << "Collision happened between player " << i << " and player " << j << std::endl;

                glm::vec3 collision_dir = collider1.getCollisionNormal(collider2);

                p_objects[i]->setVelocity(-collision_dir * 20.0f);
                p_objects[j]->setVelocity(collision_dir * 20.0f);
                if (p_objects[i]->getPosition() != p_objects[j]->getPosition())
                {
                    p_objects[i]->setPosition(p_objects[i]->getOldPosition() - collision_dir * 0.5f);
                    p_objects[j]->setPosition(p_objects[j]->getOldPosition() + collision_dir * 0.5f);
                }
                else
                {
                    p_objects[i]->setPosition(p_objects[i]->getPosition() + 1.0f);
                    p_objects[j]->setPosition(p_objects[j]->getPosition());
                }
                p_objects[i]->setForce(glm::vec3(0.0));
                p_objects[j]->setForce(glm::vec3(0.0));
            }
        }
    }

    // collision between player and game objects
    for (unsigned int i = 0; i < p_objects.size(); i++)
    {
        for (unsigned int j = i + 1; j < m_objects.size(); j++)
        {
            if (!p_objects[i] || !m_objects[j])
            {
                std::cout << "Error: Null player or object detected." << std::endl;
                continue;
            }

            Collider &playerCollider = p_objects[i]->getCollider();
            Collider &objectCollider = m_objects[j]->getCollider();

            // Check for collision using references
            // std::cout << "Checking collision between player " << i << " and object " << j << std::endl;
            bool collision = playerCollider.collide(objectCollider);

            if (collision)
            {
                std::cout << "Collision happened between player " << i << " and object " << j << std::endl;

                glm::vec3 collision_dir = playerCollider.getCollisionNormal(objectCollider);

                if (collision_dir.y < -0.5) {
                    p_objects[i]->setVelocity(glm::vec3(p_objects[i]->getVelocity().x, 0, p_objects[i]->getVelocity().z));
                    glm::vec3 maxExt = objectCollider.maxExtents;
                    p_objects[i]->setPosition(glm::vec3(p_objects[i]->getPosition().x, maxExt.y, p_objects[i]->getPosition().z));
                    p_objects[i]->setForce(glm::vec3(0.0));
                    continue;
                }

                p_objects[i]->setVelocity(-collision_dir * 20.0f);
                // glm::vec3 tempVel = p_objects[i]->getVelocity();
                // tempVel.y = 0.0;
                // p_objects[i]->setVelocity(tempVel);

                if (p_objects[i]->getPosition() != m_objects[j]->getPosition())
                {
                    p_objects[i]->setPosition(p_objects[i]->getOldPosition() - collision_dir * 0.5f);
                }
                else
                {
                    p_objects[i]->setPosition(p_objects[i]->getPosition() + 1.0f);
                }
                p_objects[i]->setForce(glm::vec3(0.0));
            }
        }
    }

    // collision between player and batteries
    for (unsigned int i = 0; i < p_objects.size(); i++)
    {
        for (unsigned int j = i + 1; j < b_objects.size(); j++)
        {
            if (!p_objects[i] || !b_objects[j])
            {
                std::cout << "Error: Null player or object detected." << std::endl;
                continue;
            }

            Collider &playerCollider = p_objects[i]->getCollider();
            Collider &objectCollider = b_objects[j]->getCollider();

            // Check for collision using references
            // std::cout << "Checking collision between player " << i << " and object " << j << std::endl;
            bool collision = playerCollider.collide(objectCollider);

            if (collision)
            {
                std::cout << "Collision happened between player " << i << " and battery " << j << std::endl;
                int score = p_objects[i]->updateScore();
                std::cout << "Player " << i << " collected " << score << " batteries!" << std::endl;
                removeBatteries(b_objects[j]);
            }
        }
    }

}

void PhysicsWorld::handleCollisionsNPC()
{
    // collision between NPC and game objects
    for (unsigned int i = 0; i < s_objects.size(); i++)
    {
        for (unsigned int j = i + 1; j < m_objects.size(); j++)
        {
            if (!s_objects[i] || !m_objects[j])
            {
                std::cout << "Error: Null student or object detected." << std::endl;
                continue;
            }

            Collider &studentCollider = s_objects[i]->getCollider();
            Collider &objectCollider = m_objects[j]->getCollider();

            // Check for collision using references
            // std::cout << "Checking collision between player " << i << " and object " << j << std::endl;
            bool collision = studentCollider.collide(objectCollider);

            if (collision)
            {
                std::cout << "Collision happened between student " << i << " and object " << j << std::endl;

                glm::vec3 collision_dir = studentCollider.getCollisionNormal(objectCollider);
                collision_dir.y = 0;
                collision_dir = glm::normalize(collision_dir);

                s_objects[i]->setVelocity(-collision_dir * 20.0f);

                if (s_objects[i]->getPosition() != m_objects[j]->getPosition())
                {
                    s_objects[i]->setPosition(s_objects[i]->getOldPosition() - collision_dir * 0.5f);
                }
                else
                {
                    s_objects[i]->setPosition(s_objects[i]->getPosition() + 1.0f);
                }
                s_objects[i]->setForce(glm::vec3(0.0));
            }
        }
    }
}
