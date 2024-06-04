#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include "game_object.h"
#include "../core.h"
#include <vector>
#include <algorithm>

class PhysicsWorld {
	private:
		std::vector<GameObject*> m_objects;
		std::vector<PlayerObject*> p_objects;
		std::vector<GameObject*> s_objects;
	
	public:
		void addObject (GameObject* object);
		void removeObject (GameObject* object);

		void addNPC (GameObject* object);
		void removeNPC (GameObject* object);

		void addPlayer (PlayerObject* object);
		void removePlayer (PlayerObject* object);
		PlayerObject* findPlayer(int id);

		void step();
		void step_student(glm::mat4 world = glm::mat4(1.0f));
		void handleCollisions();
		void handleCollisionsNPC();

		void cleanup();
};

#endif