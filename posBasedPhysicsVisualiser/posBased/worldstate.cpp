#include "worldstate.h"

worldstate::worldstate() {
    for (int index = 0; index < softforcePoolSize; index++) {
        softforcePool[index] = nullptr;
    }

    for (int index = 0; index < constraintPoolSize; index++) {
        constraintPool[index] = nullptr;
    }
}
worldstate::worldstate(const worldstate& newValue) {
    *this = newValue;
}

worldstate::~worldstate() {
    //delete the data referenced to by softForcePool and constraintPool as these arrays contain pointers
    for (int i = 0; i < softforcePoolSize; i++) {
        delete softforcePool[i];
    }

    for (int i = 0; i < constraintPoolSize; i++) {
        delete constraintPool[i];
    }
}

particle worldstate::getParticle(const int& index) {
    return myParticlePool.getParticle(index);
}

softforce* worldstate::getSoftforce(const int& index) {
    softforce* tempSoftforcePointer = nullptr;
    if (index >= 0 && index < softforcePoolSize) {
        tempSoftforcePointer = softforcePool[index];
    }
    return tempSoftforcePointer;
}

constraint* worldstate::getConstraint(const int& index) {
    constraint* tempConstraintPointer = nullptr;
    if (index >= 0 && index < constraintPoolSize) {
        tempConstraintPointer = constraintPool[index];
    }
    return tempConstraintPointer;
}

void worldstate::setParticle(const int& index, const particle& newParticle) {
    myParticlePool.setParticle(index, newParticle);
}

int worldstate::addParticle(const particle& newParticle) {
    return myParticlePool.addParticle(newParticle);
}

int worldstate::addSoftforce(softforce* const newSoftforce) {
    int newSoftforceIndex = -1;
    for (int i = 0; i < softforcePoolSize; i++) {
        if (softforcePool[i] == nullptr) {
            softforcePool[i] = newSoftforce;
            newSoftforceIndex = i;
            break;
        }
    }
    return newSoftforceIndex;
}

int worldstate::addConstraint(constraint* const newConstraint) {
    int newConstraintIndex = -1;
    for (int i = 0; i < constraintPoolSize; i++) {
        if (constraintPool[i] == nullptr) {
            constraintPool[i] = newConstraint;
            newConstraintIndex = i;
            break;
        }
    }
    return newConstraintIndex;
}

void worldstate::removeParticle(const int& index) {
    //remove all soft forces related to this particle;
    for (int i = 0; i < softforcePoolSize; i++) {
        if (softforcePool[i]->getUsingParticle(index)) {
            removeSoftforce(i);
        }
    }

    //remove all constraints related to this particle;
    for (int i = 0; i < constraintPoolSize; i++) {
        if (constraintPool[i]->getUsingParticle(index)) {
            removeConstraint(i);
        }
    }

    myParticlePool.removeParticle(index);
}

void worldstate::removeSoftforce(const int& index) {
    if (index >= 0 && index < softforcePoolSize) {
        delete softforcePool[index];
        softforcePool[index] = nullptr;
    }
}

void worldstate::removeConstraint(const int& index) {
    if (index >= 0 && index < constraintPoolSize) {
        delete constraintPool[index];
        constraintPool[index] = nullptr;
    }
}

particlePool worldstate::getParticlePool() const {
    return myParticlePool;
}

void worldstate::setParticlePool(const particlePool& newParticlePool) {
    myParticlePool = newParticlePool;
}

int worldstate::getSoftforcePoolSize() const {
    return softforcePoolSize;
}

int worldstate::getConstraintPoolSize() const {
    return constraintPoolSize;
}

void worldstate::operator=(const worldstate& newValue) {

}
