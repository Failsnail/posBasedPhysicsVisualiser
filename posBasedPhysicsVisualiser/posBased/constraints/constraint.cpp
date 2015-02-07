#include "constraint.h"

constraint::constraint() {

}

constraint::~constraint() {

}

void constraint::resolveConstraint(const float& relaxationCoeficient, particlePool* const newInBuffer, particlePool* newOutBuffer) {
    inBuffer = newInBuffer;
    outBuffer = newOutBuffer;

    float error = getError();

    if (error != error || error < 0) {
        std::cout << "\nA CONSTRAINT WENT ROGUE!!!\n\n";
    }

    if (error > 0) {
        virtualResolveConstraint(error * relaxationCoeficient);
    }

    inBuffer = nullptr;
    outBuffer = nullptr;
}

int constraint::getPoolSize() const {
    return inBuffer->getParticlePoolSize();
}

vectorType constraint::getPosition(const int& particleIndex) const {
    return inBuffer->getPosition(particleIndex);
}

unit constraint::getDistance(const int& particleIndex1, const int& particleIndex2) const {
    return (getPosition(particleIndex1) - getPosition(particleIndex2)).getLength();
}

const particle constraint::getParticle(const int& particleIndex) const {
    return inBuffer->getParticle(particleIndex);
}

float constraint::getMass(const int& particleIndex) const {
    return inBuffer->getMass(particleIndex);
}

void constraint::displace(const int& particleIndex, const vectorType& displacement) {
    outBuffer->setPosition(particleIndex, outBuffer->getPosition(particleIndex) + displacement);
}

void constraint::setPosition(const int& particleIndex, const vectorType& newPosition) {
    displace(particleIndex, newPosition - getPosition(particleIndex));
}
