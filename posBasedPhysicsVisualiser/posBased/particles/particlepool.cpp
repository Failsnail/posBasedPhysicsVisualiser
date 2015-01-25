#include "particlepool.h"

particlePool::particlePool(const int& initialParticlePoolSize) {
    initialize(initialParticlePoolSize);
}

particlePool::particlePool(const particlePool& newValue) {
    clear();
    *this = newValue;
}

particlePool::~particlePool() {
    clear();
}

particle particlePool::getParticle(const int& index) const {
    particle tempParticle;
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            tempParticle = myParticlePool[index];
        }
    }
    return tempParticle;
}

vectorType particlePool::getPosition(const int& index) const {
    vectorType tempVector;
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            tempVector = myParticlePool[index].getPosition();
        }
    }
    return tempVector;
}

vectorType particlePool::getVelocity(const int& index) const {
    vectorType tempVector;
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            tempVector = myParticlePool[index].getVelocity();
        }
    }
    return tempVector;
}

vectorType particlePool::getAcceleration(const int& index) const {
    vectorType tempVector;
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            tempVector = myParticlePool[index].getAcceleration();
        }
    }
    return tempVector;
}

float particlePool::getMass(const int& index) const {
    float tempFloat = -1;
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            tempFloat = myParticlePool[index].getMass();
        }
    }
    return tempFloat;
}

void particlePool::setParticle(const int& index, const particle& newParticle) {
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            myParticlePool[index] = newParticle;
        }
    }
}

void particlePool::setPosition(const int& index, const vectorType& newPosition) {
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            myParticlePool[index].setPosition(newPosition);
        }
    }
}

void particlePool::setVelocity(const int& index, const vectorType& newVelocity) {
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            myParticlePool[index].setVelocity(newVelocity);
        }
    }
}

void particlePool::setAcceleration(const int& index, const vectorType& newAcceleration) {
    if (index >= 0 && index < particlePoolSize) {
        if (myParticlePoolFlag[index] == true) {
            myParticlePool[index].setAcceleration(newAcceleration);
        }
    }
}

void particlePool::clearAcceleration() {
    for (int index = 0; index < particlePoolSize; index++) {
        vectorType tempVector;
        tempVector.clear();
        setAcceleration(index, tempVector);
    }
}

int particlePool::addParticle(const particle& newParticle) {
    int tempInt = -1;
    for (int index = 0; index < particlePoolSize; index++) {
        if (myParticlePoolFlag[index] == false) {
            myParticlePool[index] = newParticle;
            myParticlePoolFlag[index] = true;
            tempInt = index;
            break;
        }
    }
    return tempInt;
}

void particlePool::removeParticle(const int& index) {
    if (index >= 0 && index < particlePoolSize) {
        myParticlePool[index].clear();
        myParticlePoolFlag[index] = false;
    }
}

int particlePool::getParticlePoolSize() const {
    return particlePoolSize;
}

void particlePool::clear() {
    delete[] myParticlePoolFlag;
    myParticlePoolFlag = nullptr;

    delete[] myParticlePool;
    myParticlePool = nullptr;

    particlePoolSize = 0;
}

void particlePool::initialize(const int& newParticlePoolSize) {
    if (newParticlePoolSize < 1) {
        return;
    }

    clear();
    particlePoolSize = newParticlePoolSize;
    myParticlePoolFlag = new bool [particlePoolSize];
    myParticlePool = new particle [particlePoolSize];

    for (int index = 0; index < particlePoolSize; index++) {
        myParticlePoolFlag[index] = false;
    }
}

particlePool& particlePool::operator= (const particlePool& newValue) {
    if (this == &newValue) {
        return *this;
    }

    if (particlePoolSize != newValue.particlePoolSize) {
        initialize(newValue.particlePoolSize);
    }

    for (int index = 0; index < particlePoolSize; index++) {
        myParticlePoolFlag[index] = newValue.myParticlePoolFlag[index];
        myParticlePool[index] = newValue.myParticlePool[index];
    }

    return *this;
}
