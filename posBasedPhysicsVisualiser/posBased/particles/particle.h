#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector/vector2d.h"
//#include "vector/vector3d.h"

using vectorType = vector2D;

class particle {
    public:
        particle();
        particle(float newMass);
        virtual ~particle();
        vectorType getPosition() const;
        vectorType getVelocity() const;
        vectorType getAcceleration() const;
        float getMass() const;
        void setPosition(const vectorType& newPosition);
        void setVelocity(const vectorType& newVelocity);
        void setAcceleration(const vectorType& newAcceleration);
        void clearAcceleration();
        void clear();
    protected:
    private:
        vectorType position;
        vectorType velocity;
        vectorType acceleration;
        float mass = 1;
};

#endif // PARTICLE_H
