#pragma once

#include "../core.h"

#define PLAYER_LENGTH 0.73
#define PLAYER_WIDTH 0.55
#define PLAYER_HEIGHT 0.9
#define NPC_LENGTH 0.725
#define NPC_WIDTH 0.725
#define NPC_HEIGHT 2.16

#define GIRL_NPC_LENGTH 0.515
#define GIRL_NPC_WIDTH 0.55
#define GIRL_NPC_HEIGHT 1.8

#define TYPE_PLAYER 1
#define TYPE_NPC 0

class Collider {
public:
    virtual ~Collider() {}

    // function for collision detection
    virtual bool collide(Collider& other) = 0;

    // function for setting bounding box based on position
    virtual void setBoundingBox(const glm::vec3& position, int type_player=1) = 0;

	// functions for handling collisions
	virtual glm::vec3 getCollisionNormal(Collider& other) const = 0;
    // virtual float getPenetrationDepth(Collider& other) const = 0; do we need this??

	glm::vec3 minExtents;
    glm::vec3 maxExtents;
};

class AABB : public Collider {
public:

	// constructor with parameters
    AABB(glm::vec3 min, glm::vec3 max){
        minExtents = min;
        maxExtents = max;
    }
    
    // AABB() {
    //     minExtents = glm::vec3(0.0f);
    //     maxExtents = glm::vec3(0.0f);
    // }

    // constructor that takes position
    AABB(const glm::vec3& position, int type_player) {
        setBoundingBox(position, type_player);
    }

    bool collide(Collider& other) override;
    void setBoundingBox(const glm::vec3& position, int type_player=1) override;

    bool collidingAABB(AABB& other);

    glm::vec3 getMinExtents() const { return minExtents; }
    glm::vec3 getMaxExtents() const { return maxExtents; }

	glm::vec3 getCollisionNormal(Collider& other) const override;
    // float getPenetrationDepth(Collider& other) const override;

};

// class Plane : public Collider {
// public:
//     bool collide(Collider& other) override;  // To be implemented in collider.cpp
//     void setBoundingBox(const glm::vec3& position) override;  // To be implemented in collider.cpp
// };
