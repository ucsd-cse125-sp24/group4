#include "../include/physics/collider.h"

bool Collider::collide(Collider& other) {
    if (type == AABB && other.getType == AABB) {
        AABB* self = (AABB*)this
        return self->collideAABB((AABB&) other);
    }

    return false;
}