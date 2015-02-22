#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <cmath>

using unit = double;

class vector2D {
    public:
        vector2D(unit newX, unit newY);
        vector2D();
        ~vector2D();
        unit getLength() const;
        unit getX() const;
        unit getY() const;
        vector2D normalize();
        void setX(const unit& newX);
        void setY(const unit& newY);
        void set(const unit& newX, const unit& newY);
        void clear();
        unit dot(const vector2D& otherVector) const;
        vector2D operator + (const vector2D& otherVector) const;
        vector2D operator - (const vector2D& otherVector) const;
        vector2D operator * (const unit& factor) const;
        vector2D operator / (const unit& factor) const;
    protected:
    private:
        unit x;
        unit y;
};

#endif // VECTOR2D_H
