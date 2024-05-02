#pragma once

#include "core.h"

enum ColliderType {
	SPHERE,
	PLANE,
    BOX
};

struct Collider {
	ColliderType type;
};

struct SphereCollider : Collider {
	glm::vec3 center;
	float radius;

};

struct PlaneCollider : Collider {
	glm::vec3 normal;
	float distance;

};

struct BoxCollider : Collider {
    float max;
    float min;

};
