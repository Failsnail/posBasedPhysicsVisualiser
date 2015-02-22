#ifndef DAMPER_H
#define DAMPER_H

#include <softforces/softforce.h>


class damper : public softforce {
    public:
        damper(const int& newParticle1, const int& newParticle2, float newStiffness);
        virtual ~damper();
        virtual bool getUsingParticle(const int& index) const;
        virtual void changeIndex(const int& oldIndex, const int& newIndex);
    protected:
        void virtualApplySoftforce();
    private:
        float stiffness;
        int particle1;
        int particle2;
};

#endif // DAMPER_H
