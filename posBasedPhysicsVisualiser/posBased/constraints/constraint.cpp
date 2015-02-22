#include "constraint.h"

constraint::constraint() {

}

constraint::~constraint() {

}

void constraint::resolveConstraint(const float& relaxationCoeficient, particlePool& newInBuffer, particlePool& newOutBuffer) {
    inBuffer = &newInBuffer;
    outBuffer = &newOutBuffer;

    float error = getError(*inBuffer);

    /*
    if (error != error || error < 0) {
        std::cout << "\nA CONSTRAINT WENT ROGUE!!!\n\n";
    }
    */

    if (error > 0) {
        virtualResolveConstraint(error * relaxationCoeficient);
    }

    inBuffer = nullptr;
    outBuffer = nullptr;
}

float constraint::getError(particlePool& newInBuffer) {
    particlePool* tempPool = inBuffer;

    inBuffer = &newInBuffer;
    float error = 0.0f;

    error = virtualGetError();

    inBuffer = tempPool;

    return error;
}

int constraint::getPoolSize() const {
    assert(inBuffer != nullptr);
    return inBuffer->getParticlePoolSize();
}

vectorType constraint::getPosition(const int& particleIndex) const {
    assert(inBuffer != nullptr);
    return inBuffer->getPosition(particleIndex);
}

unit constraint::getDistance(const int& particleIndex1, const int& particleIndex2) const {
    assert(inBuffer != nullptr);
    return (getPosition(particleIndex1) - getPosition(particleIndex2)).getLength();
}

const particle constraint::getParticle(const int& particleIndex) const {
    assert(inBuffer != nullptr);
    return inBuffer->getParticle(particleIndex);
}

float constraint::getMass(const int& particleIndex) const {
    assert(inBuffer != nullptr);
    return inBuffer->getMass(particleIndex);
}

void constraint::displace(const int& particleIndex, const vectorType& displacement) {
    assert(inBuffer != nullptr);
    assert(outBuffer != nullptr);
    outBuffer->setPosition(particleIndex, outBuffer->getPosition(particleIndex) + displacement);
}

void constraint::setPosition(const int& particleIndex, const vectorType& newPosition) {
    displace(particleIndex, newPosition - getPosition(particleIndex));
}
