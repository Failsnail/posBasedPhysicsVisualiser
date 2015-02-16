#include "simulator.h"

simulator::simulator() {

}

simulator::~simulator() {

}

void simulator::simulate(worldstate& providedWorld, timeUnit deltaTime) {
    if (deltaTime <= 0) {
        std::cout << "ERROR! the deltaTime provided for the simulator is invalid!" << std::endl;
        return;
    }

    timeUnit tempTime = deltaTime / (float)fullIterationsNumber;

    for (int i = 0; i < fullIterationsNumber; i++) {
        virtualSimulate(providedWorld, tempTime);
    }
}

void simulator::relaxConstraints(worldstate& providedWord, const int& iterations) {
    world = &providedWord;

    t1 = world->getParticlePool();

    Relax(relaxationCoefficient, iterations);

    world->setParticlePool(t1);

    world = nullptr;
}

float simulator::getError(worldstate& providedWorld) {
    std::cout << "getError\n";
    //ergens in deze method crashed het programma

    world = &providedWorld;

    t1 = world->getParticlePool();

    std::cout << "check?\n";

    float tempError = 0;
    constraint* tempConstraint;

    for (int i = 0; i < world->getConstraintPoolSize(); i++) {
        tempConstraint = world->getConstraint(i);
        if (tempConstraint != nullptr) {
            tempError += tempConstraint->getError(t1);
        }
    }

    std::cout << "check?\n";

    world = nullptr;

    std::cout << "error succesfully found!\n";

    return tempError;
}

void simulator::virtualSimulate(worldstate& providedWorld, timeUnit deltaTime) {
    world = &providedWorld;

    tP = t0 = world->getParticlePool();

    project(projectionIterationsNumber, deltaTime);

    t1 = tP;

    Relax(relaxationCoefficient, relaxationIterationsNumber);

    integrate(deltaTime);

    world->setParticlePool(t1);

    world = nullptr;
}

void simulator::setFullIterationsNumber(const int& newFullIterationsNumber) {
    if (newFullIterationsNumber >= 0) {
        fullIterationsNumber = newFullIterationsNumber;
    } else {
        fullIterationsNumber = 1;
    }
}

void simulator::setProjectionIterationsNumber(const int& newProjectionIterationsNumber) {
    if (newProjectionIterationsNumber >= 0) {
        projectionIterationsNumber = newProjectionIterationsNumber;
    } else {
        projectionIterationsNumber = 1;
    }
}

void simulator::setRelaxationIterationsNumber(const int& newRelaxationIterationsNumber) {
    if (newRelaxationIterationsNumber >= 0) {
        relaxationIterationsNumber = newRelaxationIterationsNumber;
    } else {
        relaxationIterationsNumber = 1;
    }
}

void simulator::setRelaxationCoefficient(const float& newRelaxationCoefficient) {
    if (newRelaxationCoefficient > 0 && newRelaxationCoefficient <= 1) {
        relaxationCoefficient = newRelaxationCoefficient;
    }
}

void simulator::project(const int& iterations, const timeUnit& deltaTime) {       //writes results to tP
    timeUnit tempTime = deltaTime / projectionIterationsNumber;
    for (int i = 0; i < projectionIterationsNumber; i++) {
        tP.clearAcceleration();
        tBuffer = tP;

        //put this loop in worldstate!
        for (int i = 0; i < world->getSoftforcePoolSize(); i++) {
            if (world->getSoftforce(i) != nullptr) {
                world->getSoftforce(i)->applySoftforce(tBuffer, tP);
            }
        }

        //put this loop somewhere else too
        for (int index = 0; index < t0.getParticlePoolSize(); index++) {
            tP.setPosition(index, tP.getPosition(index) + tP.getVelocity(index) * tempTime + tP.getAcceleration(index) * 0.5 * tempTime * tempTime);
            tP.setVelocity(index, tP.getVelocity(index) + tP.getAcceleration(index) * tempTime);
        }
    }
}

void simulator::Relax(const float& resolveCoeficient, const int& iterations) {                           //writes results to t1
    for (int i = 0; i < iterations; i++) {
        tBuffer = t1;

        //put this loop in worldstate!
        for (int i = 0; i < world->getConstraintPoolSize(); i++) {
            if (world->getConstraint(i) != nullptr) {
                world->getConstraint(i)->resolveConstraint(resolveCoeficient, tBuffer, t1);
            }
        }
    }
}

void simulator::integrate(timeUnit deltaTime) {     //writes results to t1
    for (int index = 0; index < t0.getParticlePoolSize(); index++) {
        #if false
            t1.setVelocity(index, (t1.getPosition(index) - t0.getPosition(index)) / deltaTime);
        #else
            t1.setVelocity(index, (t1.getPosition(index) - t0.getPosition(index)) / deltaTime * 2 - t0.getVelocity(index));
            t1.setAcceleration(index, (t1.getVelocity(index) - t0.getVelocity(index)) / deltaTime);
        #endif
    }
}
