#include "positionconstraint.h"

positionconstraint::positionconstraint(const int& newParticle, const vectorType& newPosition) {
    particle = newParticle;
    position = newPosition;
}

positionconstraint::~positionconstraint() {
    //dtor
}

void positionconstraint::virtualResolveConstraint(const float& resolveError) {
    float St = resolveError / getMass(particle);
    vectorType tempVector = position - getPosition(particle);
    tempVector.normalize();
    tempVector = tempVector * St;
    displace(particle, tempVector);
}

float positionconstraint::virtualGetError() const {
    return getMass(particle) * (getPosition(particle) - position).getLength();
}

bool positionconstraint::getUsingParticle(const int& index) const {
    if (particle == index) {
        return true;
    } else {
        return false;
    }
}

void positionconstraint::changeIndex(const int& oldIndex, const int& newIndex) {
    if (particle == oldIndex) {
        particle = newIndex;
    }
}


