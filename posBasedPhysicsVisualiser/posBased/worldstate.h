#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <iostream>

#include "particles/particle.h"
#include "particles/particlepool.h"
#include "softforces/softforce.h"
#include "constraints/constraint.h"

#define softforcePoolSize_ 50
#define constraintPoolSize_ 100

class particle;
class softforce;
class constraint;
class particlePool;

class worldstate {
    public:
        worldstate();
        worldstate(const worldstate& newValue);
        virtual ~worldstate();

        particle getParticle(const int& index);
        softforce* getSoftforce(const int& index);
        constraint* getConstraint(const int& index);

        void setParticle(const int& index, const particle& newParticle);    //is this still required?

        int addParticle(const particle& newParticle);
        int addSoftforce(softforce* const newSoftforce);
        int addConstraint(constraint* const newConstraint);

        void removeParticle(const int& index);
        void removeSoftforce(const int& index);
        void removeConstraint(const int& index);

        particlePool getParticlePool() const;
        void setParticlePool(const particlePool& newParticlePool);

        int getSoftforcePoolSize() const;
        int getConstraintPoolSize() const;

        void operator = (const worldstate& newValue);
    protected:
    private:
        const int softforcePoolSize = softforcePoolSize_;
        const int constraintPoolSize = constraintPoolSize_;

        particlePool myParticlePool;
        softforce* softforcePool [softforcePoolSize_];
        constraint* constraintPool [constraintPoolSize_];
};

#endif // WORLDSTATE_H
