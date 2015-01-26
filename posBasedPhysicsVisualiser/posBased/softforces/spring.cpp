#include "spring.h"

spring::spring(unit newLength) {
    length = newLength;
}

spring::~spring() {
    //dtor
}

bool spring::getUsingParticle(const int& index) const {
    bool tempBool = false;
    if (particle1 == index) {
        tempBool = true;
    }
    if (particle2 == index) {
        tempBool = true;
    }
    return tempBool;
}

void spring::changeIndex(const int& oldIndex, const int& newIndex) {
    if (particle1 == oldIndex) {
        particle1 = newIndex;
    }
    if (particle2 == oldIndex) {
        particle2 == newIndex;
    }
}

void spring::virtualApplySoftforce() {
    //DO STUFF HERE
}
