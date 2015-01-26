#ifndef SPRING_H
#define SPRING_H

#include <softforces/softforce.h>


class spring : public softforce {
    public:
        spring(unit newLength = 1);
        virtual ~spring();
        virtual bool getUsingParticle(const int& index) const;
        virtual void changeIndex(const int& oldIndex, const int& newIndex);
    protected:
        void virtualApplySoftforce();   //DOESN'T DO ANYTHING YET!!!
    private:
        unit length;
        int particle1;
        int particle2;
};

#endif // SPRING_H
