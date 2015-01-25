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

void distanceconstraint::virtualResolveConstraint() {
    vectorType relativePosition = getPosition(particle1) - getPosition(particle2);
    //vector pointing from particle1 to particle2

    switch (myType) {
        case greater:
            if (relativePosition.getLength() >= length) {
                return;
            }
            break;
        case smaller:
            if (relativePosition.getLength() <= length) {
                return;
            }
            break;
    }

    if (relativePosition.getLength() == 0) {
        //there is no distance between the two constraints, so the constraint cannot be resolved
        return;
    }

    float displacementLength = relativePosition.getLength() - length;
    //float representing how far particle1 and two should be moved apart
    //this value is negative if they should be moved towards each other
    //if this value equals zero, the constraint is already resolved

    vectorType displacementVector;
    //vector representing the displacement of particle2 relative to particle1
    displacementVector = relativePosition;

    displacementVector.normalize();
    displacementVector = displacementVector * displacementLength;

    displace(particle1, displacementVector * -1 / (1 + getParticle(particle1).getMass() / getParticle(particle2).getMass()));
    displace(particle2, displacementVector *  1 / (1 + getParticle(particle2).getMass() / getParticle(particle1).getMass()));
}

bool distanceconstraint::getUsingParticle(const int& index) const {
    if (particle1 == index) {
        return true;
    }
    if (particle2 == index) {
        return true;
    }
    return false;
}

void distanceconstraint::changeIndex(const int& oldIndex, const int& newIndex) {
    if (particle1 == oldIndex) {
        particle1 = newIndex;
    }
    if (particle2 == oldIndex) {
        particle2 == newIndex;
    }
}
