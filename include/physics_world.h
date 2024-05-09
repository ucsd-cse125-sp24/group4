#include "game_object.h"
#include "core.h"
#include <vector>
#include <algorithm>

class PhysicsWorld {
	private:
		std::vector<GameObject*> m_objects;
		std::vector<PlayerObject*> p_objects;
		glm::vec3 m_gravity = glm::vec3(0, -9.81f, 0);
	
	public:
		void addObject (GameObject* object);
		void removeObject (GameObject* object);

		void addPlayer (PlayerObject* object);
		void removePlayer (PlayerObject* object);
		PlayerObject* findPlayer(int id);

		void step();
		bool hasCollision();
};