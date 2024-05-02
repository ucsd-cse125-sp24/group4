#include "game_object.h"
#include "core.h"
#include <vector>
#include <algorithm>

class PhysicsWorld {
	private:
		std::vector<GameObject*> m_objects;
		glm::vec3 m_gravity = glm::vec3(0, -9.81f, 0);
	
	public:
		void AddObject   (GameObject* object);
		void RemoveObject(GameObject* object);
		void Step(float dt);
};