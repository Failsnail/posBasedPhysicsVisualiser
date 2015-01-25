#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>

#include "particles/particle.h"
#include "particles/particlePool.h"
#include "softforces/softforce.h"
#include "constraints/constraint.h"

#include "worldstate.h"

using timeUnit = float;

class simulator {
    public:
        simulator();
        virtual ~simulator();
        void simulate(worldstate* providedWorld, timeUnit deltaTime);
        void virtualSimulate(worldstate* providedWorld, timeUnit deltaTime);
        void setFullIterationsNumber(int newFullIterationsNumber);
        void setProjectionIterationsNumber(int newProjectionIterationsNumber);
        void setRelaxationIterationsNumber(int newRelaxationIterationsNumber);
    protected:
        worldstate* world;

        void project(timeUnit deltaTime);
        void Relax();
        void integrate(timeUnit deltaTime);

        particlePool t0;        //initial state
        particlePool tP;        //state after projection
        particlePool t1;        //state after relaxation
        particlePool tBuffer;   //temporal buffer
    private:
        int fullIterationsNumber = 1;
        int projectionIterationsNumber = 1;
        int relaxationIterationsNumber = 1;
};

#endif // SIMULATOR_H
