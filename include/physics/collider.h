#pragma once

#include "../core.h"


class Collider {
public:

	// types of colliders
	enum
	{
		AABB,
		PLANE,
	};


	Collider(int type) :
		type(type) {}
	
	Collider() {}

	virtual ~Collider() {}
	/**
	 * Check if one collider intersects another
	 */
	bool collide(Collider& other);

	/**
	 * Moves the entire collider by translation distance. Should be overriden
	 * by subclasses.
	 *
	 * @param translation Distance to move the collider
	 */
	virtual void transform(glm::vec3 translation) {}

	// /**
	//  * Returns the center position of the collider. Should be overriden by
	//  * subclasses.
	//  */
	// virtual Vector3f GetCenter() const { return Vector3f(0,0,0); }

	/** Basic getter */
	int getType() const { return type; }
private:
	int type;
};
