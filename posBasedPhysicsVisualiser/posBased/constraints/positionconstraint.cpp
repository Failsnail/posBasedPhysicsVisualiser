#include "positionconstraint.h"

positionconstraint::positionconstraint(const int& newParticle, const vectorType& newPosition) {
    particle = newParticle;
    position = newPosition;
}

positionconstraint::~positionconstraint() {
    //dtor
}

void positionconstraint::virtualResolveConstraint() {
    std::cout << "resolving constraint\n";
    setPosition(particle, position);
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


