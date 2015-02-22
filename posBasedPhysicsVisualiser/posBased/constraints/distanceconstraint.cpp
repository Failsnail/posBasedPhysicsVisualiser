#include "distanceconstraint.h"

distanceconstraint::distanceconstraint(const int& newParticle1, const int& newParticle2, const unit& newLength, const int& newType) {
    particle1 = newParticle1;
    particle2 = newParticle2;
    switch (newType) {
        case 1:
            myType = equality;
            break;
        case 2:
            myType = greater;
            break;
        case 3:
            myType = smaller;
            break;
        default:
            //something went wrong
            myType = equality;
    }
    length = newLength;
}

distanceconstraint::~distanceconstraint() {
    //dtor
}

void distanceconstraint::virtualResolveConstraint(const float& resolveError) {
    vectorType relativePosition = getPosition(particle2) - getPosition(particle1);
        //vector pointing from particle1 to particle2 with unit length
    float distance = relativePosition.getLength();
    relativePosition = relativePosition.normalize();

    float displacement1;
    float displacement2;
    //floats representing how far both vectors have to be moved outward

    if (distance < length) {
        displacement1 = 0.5 * resolveError / getMass(particle1);
        displacement2 = 0.5 * resolveError / getMass(particle2);
    } else {
        displacement1 = - 0.5 * resolveError / getMass(particle1);
        displacement2 = - 0.5 * resolveError / getMass(particle2);
    }

    displace(particle1, relativePosition * displacement1 * -1);
    displace(particle2, relativePosition * displacement2 * +1);
}

float distanceconstraint::virtualGetError() const {
    float tempError = 0;

    float distance = (getPosition(particle1) - getPosition(particle2)).getLength();

    tempError = 2 * fabs((distance - length) * getMass(particle1) / (1 + getMass(particle1) / getMass(particle2)));

    switch (myType) {
        case greater:
            if (distance >= length) {
                tempError = 0;
            }
            break;
        case smaller:
            if (distance <= length) {
                tempError = 0;
            }
            break;
        default:
            break;
    }

    return tempError;
}

bool distanceconstraint::getUsingParticle(const int& index) const {
    bool tempBool = false;
    if (particle1 == index) {
        tempBool = true;
    }
    if (particle2 == index) {
        tempBool = true;
    }
    return tempBool;
}

void distanceconstraint::changeIndex(const int& oldIndex, const int& newIndex) {
    if (particle1 == oldIndex) {
        particle1 = newIndex;
    }
    if (particle2 == oldIndex) {
        particle2 == newIndex;
    }
}
