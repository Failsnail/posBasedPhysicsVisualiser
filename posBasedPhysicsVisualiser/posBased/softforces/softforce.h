#ifndef SOFTFORCE_H
#define SOFTFORCE_H

#include <cmath>

#include <iostream>

#include "particles/particle.h"
#include "particles/particlePool.h"
#include "vector/vector2d.h"
//#include "vector/vector3d.h"

using unit = double;
using vectorType = vector2D;

class softforce {
    public:
        softforce();
        virtual ~softforce();
        void applySoftforce(particlePool& newInBuffer, particlePool& newOutBuffer);
        virtual bool getUsingParticle(const int& index) const =0;
        virtual void changeIndex(const int& oldIndex, const int& newIndex) =0;
    protected:
        virtual void virtualApplySoftforce() =0;
        //subclass sandbox:
        int getPoolSize() const;
        vectorType getPosition(const int& particleIndex) const;
        unit getDistance(const int& particleIndex1, const int& particleIndex2) const;
        const particle getParticle(const int& particleIndex) const;
        float getMass(const int& particleIndex) const;
        vectorType getVelocity(const int& particleIndex) const;

        void applyForce(const int& particleIndex, vectorType force);
    private:
        particlePool* inBuffer;
        particlePool* outBuffer;
};

#endif // SOFTFORCE_H
