#include "particle.h"

particle::particle() {

}

particle::particle(float newMass) {
    mass = newMass;
}

particle::~particle() {

}

vectorType particle::getPosition() const {
    return position;
}

vectorType particle::getVelocity() const {
    return velocity;
}

vectorType particle::getAcceleration() const {
    return acceleration;
}

float particle::getMass() const {
    return mass;
}

void particle::setPosition(const vectorType& newPosition) {
    position = newPosition;
}

void particle::setVelocity(const vectorType& newVelocity) {
    velocity = newVelocity;
}

void particle::setAcceleration(const vectorType& newAcceleration) {
    acceleration = newAcceleration;
}
void particle::setMass(const float& newMass) {
    if (newMass > 0) {
        mass = newMass;
    }
}

void particle::clear() {
    position.clear();
    velocity.clear();
    acceleration.clear();
}

void particle::clearAcceleration() {
    acceleration.clear();
}
