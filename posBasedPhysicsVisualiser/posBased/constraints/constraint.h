#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <cmath>

#include <iostream>

#include <assert.h>

#include "particles/particle.h"
#include "particles/particlepool.h"
#include "vector/vector2d.h"
//#include "vector/vector3d.h"

using unit = double;
using vectorType = vector2D;

class constraint {
    public:
        constraint();
        virtual ~constraint();
        void resolveConstraint(const float& relaxationCoeficient, particlePool& newInBuffer, particlePool& newOutBuffer);
        virtual bool getUsingParticle(const int& index) const =0;
        virtual void changeIndex(const int& oldIndex, const int& newIndex) =0;
        float getError(particlePool& newInBuffer);
    protected:
        virtual void virtualResolveConstraint(const float& resolveError) =0;
        virtual float virtualGetError() const =0;
    //subclass sandbox:
        int getPoolSize() const;
        vectorType getPosition(const int& particleIndex) const;
        unit getDistance(const int& particleIndex1, const int& particleIndex2) const;
        const particle getParticle(const int& particleIndex) const;
        float getMass(const int& particleIndex) const;

        void displace(const int& particleIndex, const vectorType& displacement);
        void setPosition(const int& particleIndex, const vectorType& newPosition);
    private:
        particlePool* inBuffer;
        particlePool* outBuffer;
};

#endif // CONSTRAINT_H
