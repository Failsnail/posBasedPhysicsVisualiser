#ifndef GRAVITY_H
#define GRAVITY_H

#include <softforces/softforce.h>


class gravity : public softforce {
    public:
        gravity(float newGravitation = 9.81, vectorType newDirection = vectorType(0, -1));
        virtual ~gravity();
        virtual bool getUsingParticle(const int& index) const;
        virtual void changeIndex(const int& oldIndex, const int& newIndex);
    protected:
        void virtualApplySoftforce();
    private:
        vectorType direction;
        float gravitation;
};

#endif // GRAVITY_H
