#include "constraint.h"

constraint::constraint() {

}

constraint::~constraint() {

}

void constraint::resolveConstraint(particlePool* const newInBuffer, particlePool* newOutBuffer) {
    inBuffer = newInBuffer;
    outBuffer = newOutBuffer;

    virtualResolveConstraint();

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

void constraint::displace(const int& particleIndex, const vectorType& displacement) {
    outBuffer->setPosition(particleIndex, outBuffer->getPosition(particleIndex) + displacement);
}

void constraint::setPosition(const int& particleIndex, const vectorType& newPosition) {
    displace(particleIndex, newPosition - getPosition(particleIndex));
}
