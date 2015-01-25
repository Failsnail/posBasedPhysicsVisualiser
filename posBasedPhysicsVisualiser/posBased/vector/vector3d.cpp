#include "vector3d.h"

vector3D::vector3D(unit newX, unit newY, unit newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

vector3D::vector3D() {
    x = 0;
    y = 0;
    z = 0;
}

vector3D::~vector3D() {

}

unit vector3D::getLength() const {
    return (unit)sqrtl(x * x + y * y + z * z);
}

void vector3D::normalize() {
    *this = *this / (getLength());
}

unit vector3D::getX() const {
    return x;
}

unit vector3D::getY() const {
    return y;
}

unit vector3D::getZ() const {
    return z;
}

void vector3D::setX(const unit& newX) {
    x = newX;
}

void vector3D::setY(const unit& newY) {
    y = newY;
}

void vector3D::setZ(const unit& newZ) {
    z = newZ;
}

void vector3D::set(const unit& newX, const unit& newY, const unit& newZ) {
    x = (unit)newX;
    y = (unit)newY;
    z = (unit)newZ;
}

void vector3D::clear() {
    x = 0;
    y = 0;
    z = 0;
}

vector3D vector3D::operator+ (const vector3D& otherVector) const {
    return vector3D(x + otherVector.x, y + otherVector.y, z + otherVector.z);
}

vector3D vector3D::operator- (const vector3D& otherVector) const {
    return vector3D(x - otherVector.x, y - otherVector.y, z - otherVector.z);
}

vector3D vector3D::operator* (const unit& factor) const {
    return vector3D(x * factor, y * factor, z * factor);
}

vector3D vector3D::operator/ (const unit& factor) const {
    return operator*(1 / factor);
}
