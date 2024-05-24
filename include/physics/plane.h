#pragma once

#include "aabb.h"

/**
 * Represents an infinitely large plane that can be used as
 * a collider.
 */
class Plane
{
public:

	Plane(glm::vec3 n, float d) :
		normal(n),
		distance(d) {}

	/**
	 * Creates an equivalent plane with a normal at unit length and distance
	 * adjusted accordingly.
	 */
	Plane normalized() const;

	bool collideAABB(AABB& other);

	glm::vec3 getNormal() const { return normal; }
	float getDistance() const { return distance; }

	// static void Test();
private:
	const glm::vec3 normal;
	const float distance;
};
