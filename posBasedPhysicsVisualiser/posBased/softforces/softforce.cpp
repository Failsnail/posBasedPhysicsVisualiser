#include "softforce.h"

softforce::softforce() {

}

softforce::~softforce() {

}

void softforce::applySoftforce(particlePool& newInBuffer, particlePool& newOutBuffer) {
    inBuffer = &newInBuffer;
    outBuffer = &newOutBuffer;

    virtualApplySoftforce();

    inBuffer = nullptr;
    outBuffer = nullptr;
}

int softforce::getPoolSize() const {
    return inBuffer->getParticlePoolSize();
}

vectorType softforce::getPosition(const int& particleIndex) const {
    return inBuffer->getPosition(particleIndex);
}

unit softforce::getDistance(const int& particleIndex1, const int& particleIndex2) const {
    return (getPosition(particleIndex1) - getPosition(particleIndex2)).getLength();
}

const particle softforce::getParticle(const int& particleIndex) const {
    return inBuffer->getParticle(particleIndex);
}

float softforce::getMass(const int& particleIndex) const {
    return inBuffer->getMass(particleIndex);
}

vectorType softforce::getVelocity(const int& particleIndex) const {
    return inBuffer->getVelocity(particleIndex);
}

void softforce::applyForce(const int& particleIndex, vectorType force) {
    vectorType newAcceleration = outBuffer->getAcceleration(particleIndex) + force / getMass(particleIndex);
    outBuffer->setAcceleration(particleIndex, newAcceleration);
}
