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
        void simulate(worldstate& providedWorld, timeUnit deltaTime);
        void relaxConstraints(worldstate& providedWord, const int& iterations = 20);

        float getError(worldstate& providedWorld);

        void setFullIterationsNumber(const int& newFullIterationsNumber);
        void setProjectionIterationsNumber(const int& newProjectionIterationsNumber);
        void setRelaxationIterationsNumber(const int& newRelaxationIterationsNumber);
        void setRelaxationCoefficient(const float& newRelaxationCoefficient);
    protected:
        worldstate* world;

        void virtualSimulate(worldstate& providedWorld, timeUnit deltaTime);

        void project(const int& iterations, const timeUnit& deltaTime);
        void Relax(const float& resolveCoeficient, const int& iterations);
        void integrate(timeUnit deltaTime);

        particlePool t0;        //initial state
        particlePool tP;        //state after projection
        particlePool t1;        //state after relaxation
        particlePool tBuffer;   //temporal buffer
    private:
        int fullIterationsNumber = 1;
        int projectionIterationsNumber = 1;
        int relaxationIterationsNumber = 1;
        float relaxationCoefficient = 0.3f;
};

#endif // SIMULATOR_H
