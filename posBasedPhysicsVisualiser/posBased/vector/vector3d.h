#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <cmath>

using unit = double;

class vector3D {
    public:
        vector3D(unit newX, unit newY, unit newZ);
        vector3D();
        ~vector3D();
        unit getLength() const;
        unit getX() const;
        unit getY() const;
        unit getZ() const;
        vector3D normalize();
        void setX(const unit& newX);
        void setY(const unit& newY);
        void setZ(const unit& newZ);
        void set(const unit& newX, const unit& newY, const unit& newZ);
        void clear();
        unit dot(const vector3D& otherVector) const;
        vector3D operator + (const vector3D& otherVector) const;
        vector3D operator - (const vector3D& otherVector) const;
        vector3D operator * (const unit& factor) const;
        vector3D operator / (const unit& factor) const;
    protected:
    private:
        unit x;
        unit y;
        unit z;
};

#endif // VECTOR3D_H
