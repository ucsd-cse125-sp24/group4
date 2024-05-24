/**
 * code adapted from https://github.com/BennyQBD/3DEngineCpp.git
 */

#pragma once

#include "../core.h"
#include "collider.h"

/**
 * Represents an Axis Aligned Bounding Box that can be used as
 * a collider.
 */
class AABB : public Collider
{
public:
	
	AABB(glm::vec3 min, glm::vec3 max) :
		Collider(Collider::AABB),
		minExtents(min),
		maxExtents(max) {}
	
	/**
	* Checks for collisions
	*/
	bool collidingAABB(AABB& other);

	glm::vec3 getMinExtents()  { return minExtents; }
	glm::vec3 getMaxExtents()  { return maxExtents; }

	// static void Test();
private:
	glm::vec3 minExtents;
	glm::vec3 maxExtents;
};