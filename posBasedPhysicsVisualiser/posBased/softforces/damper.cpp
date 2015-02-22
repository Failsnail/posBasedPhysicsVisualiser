#include "damper.h"

damper::damper(const int& newParticle1, const int& newParticle2, float newStiffness) {
    particle1 = newParticle1;
    particle2 = newParticle2;
    stiffness = newStiffness;
}

damper::~damper() {
    //dtor
}

bool damper::getUsingParticle(const int& index) const {
    bool tempBool = false;
    if (particle1 == index) {
        tempBool = true;
    }
    if (particle2 == index) {
        tempBool = true;
    }
    return tempBool;
}

void damper::changeIndex(const int& oldIndex, const int& newIndex) {
    if (particle1 == oldIndex) {
        particle1 = newIndex;
    }
    if (particle2 == oldIndex) {
        particle2 == newIndex;
    }
}

void damper::virtualApplySoftforce() {
    vectorType relativeVelocity = getVelocity(particle1) - getVelocity(particle2);
        //velocity of particle1 relative to particle2

    vectorType relativePosition = getPosition(particle2) - getPosition(particle1);
    relativePosition = relativePosition.normalize();
        //unit vector of the position of particle2 relative to particle1

    float force = stiffness * relativePosition.dot(relativeVelocity);
        //force with wich the particles should be pushed outward

    applyForce(particle1, relativePosition * force * -1);
    applyForce(particle2, relativePosition * force * +1);
}
