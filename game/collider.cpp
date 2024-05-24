#include "../include/physics/collider.h"

bool Collider::collide(Collider& other) {
    if (type == AABB && other.getType() == AABB) {
        AABB* self = (AABB*)this;
        return self->collideAABB((AABB&) other);
    }

    return false;


    // if (type == AABB && other.getType() == AABB) {
    //     AABB* self = dynamic_cast<AABB*>(this);
    //     AABB* otherAABB = dynamic_cast<AABB*>(&other);
    //     if (self && otherAABB) {
    //         return self->collidingAABB(*otherAABB);
    //     }
    // }

    return false;
}