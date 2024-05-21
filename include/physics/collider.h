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
	virtual void transform(const Vector3f& translation) {}

	// /**
	//  * Returns the center position of the collider. Should be overriden by
	//  * subclasses.
	//  */
	// virtual Vector3f GetCenter() const { return Vector3f(0,0,0); }

	/** Basic getter */
	int getType() const { return m_type; }
private:
	int type;
};
