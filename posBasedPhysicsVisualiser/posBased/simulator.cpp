#include "simulator.h"

simulator::simulator() {

}

simulator::~simulator() {

}

void simulator::simulate(worldstate* providedWorld, timeUnit deltaTime) {
    if (deltaTime <= 0) {
        std::cout << "ERROR! the deltaTime provided for the simulator is invalid!" << std::endl;
        return;
    }

    timeUnit tempTime = deltaTime / (float)fullIterationsNumber;

    for (int i = 0; i < fullIterationsNumber; i++) {
        virtualSimulate(providedWorld, tempTime);
    }
}

void simulator::relaxConstraints(worldstate* providedWord, const int& iterations) {
    world = providedWord;

    t1 = world->getParticlePool();

    Relax(iterations);

    world->setParticlePool(t1);

    world = nullptr;
}

void simulator::virtualSimulate(worldstate* providedWorld, timeUnit deltaTime) {
    world = providedWorld;

    tP = t0 = world->getParticlePool();

    timeUnit tempTime = deltaTime / projectionIterationsNumber;
    for (int i = 0; i < projectionIterationsNumber; i++) {
        project(tempTime);
    }

    t1 = tP;

    Relax(relaxationIterationsNumber);

    integrate(deltaTime);

    world->setParticlePool(t1);

    world = nullptr;
}

void simulator::setFullIterationsNumber(int newFullIterationsNumber) {
    if (newFullIterationsNumber > 0) {
        fullIterationsNumber = newFullIterationsNumber;
    } else {
        fullIterationsNumber = 1;
    }
}

void simulator::setProjectionIterationsNumber(int newProjectionIterationsNumber) {
    if (newProjectionIterationsNumber > 0) {
        projectionIterationsNumber = newProjectionIterationsNumber;
    } else {
        projectionIterationsNumber = 1;
    }
}

void simulator::setRelaxationIterationsNumber(int newRelaxationIterationsNumber) {
    if (newRelaxationIterationsNumber > 0) {
        relaxationIterationsNumber = newRelaxationIterationsNumber;
    } else {
        relaxationIterationsNumber = 1;
    }
}

void simulator::project(timeUnit deltaTime) {       //writes results to tP
    tP.clearAcceleration();
    tBuffer = tP;

    //put this loop in worldstate!
    for (int i = 0; i < world->getSoftforcePoolSize(); i++) {
        if (world->getSoftforce(i) != nullptr) {
            world->getSoftforce(i)->applySoftforce(&tBuffer, &tP);
        }
    }

    //put this loop somewhere else too
    for (int index = 0; index < t0.getParticlePoolSize(); index++) {
        tP.setPosition(index, tP.getPosition(index) + tP.getVelocity(index) * deltaTime + tP.getAcceleration(index) * 0.5 * deltaTime * deltaTime);
        tP.setVelocity(index, tP.getVelocity(index) + tP.getAcceleration(index) * deltaTime);
    }
}

void simulator::Relax(const int& iterations) {                           //writes results to t1
    for (int i = 0; i < iterations; i++) {
        tBuffer = t1;

        //put this loop in worldstate!
        for (int i = 0; i < world->getConstraintPoolSize(); i++) {
            if (world->getConstraint(i) != nullptr) {
                world->getConstraint(i)->resolveConstraint(&tBuffer, &t1);
            }
        }
    }
}

void simulator::integrate(timeUnit deltaTime) {     //writes results to t1
    for (int index = 0; index < t0.getParticlePoolSize(); index++) {
        #if false
            t1.setVelocity(index, (t1.getPosition(index) - t0.getPosition(index)) / deltaTime);
        #endif
        #if true
            t1.setVelocity(index, (t1.getPosition(index) - t0.getPosition(index)) / deltaTime * 2 - t0.getVelocity(index));
            t1.setAcceleration(index, (t1.getVelocity(index) - t0.getVelocity(index)) / deltaTime);
        #endif
        #if false   //make this an integration sceme based on tP and t1
            t1.setVelocity(index, (t1.getPosition(index) - tP.getPosition(index)) / deltaTime * 2 - tP.getVelocity(index));
            t1.setAcceleration(index, (t1.getVelocity(index) - tP.getVelocity(index)) / deltaTime);
        #endif
    }
}
