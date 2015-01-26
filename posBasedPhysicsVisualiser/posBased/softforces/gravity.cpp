#include "gravity.h"

gravity::gravity(float newGravitation, vectorType newDirection) {
    gravitation = newGravitation;
    direction = newDirection;
}

gravity::~gravity() {
    //dtor
}

bool gravity::getUsingParticle(const int& index) const {
    return false;   //even though the particle is used, the softforce doesn't rely on it
}

void gravity::changeIndex(const int& oldIndex, const int& newIndex) {
    return;
}

void gravity::virtualApplySoftforce() {
    for (int i = 0; i < getPoolSize(); i++) {
        if (getMass(i) != INFINITY) {
            vectorType force = direction * gravitation * getMass(i);
            applyForce(i, force);
        }
    }
}
