#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include "game_object.h"
#include "../game_state.h"
#include "../core.h"
#include <vector>
#include <algorithm>

class PhysicsWorld {
	private:
		std::vector<GameObject*> m_objects;
		std::vector<PlayerObject*> p_objects;
		std::vector<GameObject*> s_objects;
		std::vector<GameObject*> b_objects;
	
	public:
		void addObject (GameObject* object);
		void removeObject (GameObject* object);

		void addNPC (GameObject* object);
		void removeNPC (GameObject* object);

		void addBatteries (GameObject* object);
		void removeBatteries (GameObject* object);

		void addPlayer (PlayerObject* object);
		void removePlayer (PlayerObject* object);
		PlayerObject* findPlayer(int id);

		int getLenBatteries () {
			return b_objects.size();
		}

		void step();
		void step_student(StudentState &student); // step for a single student
		void handleCollisions();
		void handleCollisionsNPC();

		void cleanup();
};

#endif