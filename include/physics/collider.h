#pragma once

#include "../core.h"

#define PLAYER_LENGTH 0.73
#define PLAYER_WIDTH 0.55
#define PLAYER_HEIGHT 0.9

class Collider {
public:
    virtual ~Collider() {}

    // function for collision detection
    virtual bool collide(Collider& other) = 0;

    // function for setting bounding box based on position
    virtual void setBoundingBox(const glm::vec3& position) = 0;

	// functions for handling collisions
	virtual glm::vec3 getCollisionNormal(Collider& other) const = 0;
    // virtual float getPenetrationDepth(Collider& other) const = 0; do we need this??

	glm::vec3 minExtents;
    glm::vec3 maxExtents;
};

class AABB : public Collider {
public:

	// constructor with parameters
    AABB(glm::vec3 min, glm::vec3 max) :
        minExtents(min),
        maxExtents(max) {}
    
    AABB() {
        minExtents = glm::vec3(0.0f);
        maxExtents = glm::vec3(0.0f);
    }

    // constructor that takes position
    AABB(const glm::vec3& position) {
        setBoundingBox(position);
    }

    bool collide(Collider& other) override;
    void setBoundingBox(const glm::vec3& position) override;

    bool collidingAABB(AABB& other);

    glm::vec3 getMinExtents() const { return minExtents; }
    glm::vec3 getMaxExtents() const { return maxExtents; }

	glm::vec3 getCollisionNormal(Collider& other) const override;
    // float getPenetrationDepth(Collider& other) const override;

    glm::vec3 minExtents;
    glm::vec3 maxExtents;
};

class Plane : public Collider {
public:
    bool collide(Collider& other) override;  // To be implemented in collider.cpp
    void setBoundingBox(const glm::vec3& position) override;  // To be implemented in collider.cpp
};
