#include "game_object.h"

class PhysicsWorld {
private:
	std::vector<GameObject*> m_objects;
	vec3 m_gravity = vec3(0, -9.81f, 0);
 
public:
	void AddObject   (GameObject* object) { /* ... */ }
	void RemoveObject(GameObject* object) { /* ... */ }
 
	void Step(
		float dt)
	{
		for (GameObject* obj : m_objects) {
			obj->force += obj->mass * m_gravity; // apply a force
 
			obj->velocity += obj->force / obj->mass * dt;

			// check y >= 0
			obj->position += obj->velocity * dt;
 
			obj->force = vec3(0, 0, 0); // reset net force at the end
		}
	}
};