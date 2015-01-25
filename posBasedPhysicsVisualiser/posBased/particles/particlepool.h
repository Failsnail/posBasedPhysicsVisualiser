#ifndef PARTICLEPOOL_H
#define PARTICLEPOOL_H

#include <iostream>

//#include <new>

#include "particles/particle.h"
#include "vector/vector2d.h"
#include "vector/vector3d.h"

using vectorType = vector2D;

class particlePool {
    public:
        particlePool(const int& initialParticlePoolSize = 0);
        particlePool(const particlePool& newValue);
        virtual ~particlePool();

        particle getParticle(const int& index) const;
        vectorType getPosition(const int& index) const;
        vectorType getVelocity(const int& index) const;
        vectorType getAcceleration(const int& index) const;
        float getMass(const int& index) const;

        void setParticle(const int& index, const particle& newParticle);
        void setPosition(const int& index, const vectorType& newPosition);
        void setVelocity(const int& index, const vectorType& newVelocity);
        void setAcceleration(const int& index, const vectorType& newAcceleration);

        void clearAcceleration();

        int addParticle(const particle& newParticle);
        void removeParticle(const int& index);

        int getParticlePoolSize() const;

        void clear();
        void initialize(const int& newParticlePoolSize);

        particlePool& operator = (const particlePool& newValue);
    protected:
    private:
        bool* myParticlePoolFlag = nullptr;
        particle* myParticlePool = nullptr;
        int particlePoolSize = 0;
};

#endif // PARTICLEPOOL_H
