#include "vector2d.h"

vector2D::vector2D(unit newX, unit newY) {
    x = newX;
    y = newY;
}

vector2D::vector2D() {
    x = 0;
    y = 0;
}

vector2D::~vector2D() {

}

unit vector2D::getLength() const {
    return (unit)sqrtl(x * x + y * y);
}

vector2D vector2D::normalize() {
    return *this / getLength();
}

unit vector2D::getX() const {
    return x;
}

unit vector2D::getY() const {
    return y;
}

void vector2D::setX(const unit& newX) {
    x = newX;
}

void vector2D::setY(const unit& newY) {
    y = newY;
}

void vector2D::set(const unit& newX, const unit& newY) {
    x = (unit)newX;
    y = (unit)newY;
}

void vector2D::clear() {
    x = 0;
    y = 0;
}

unit vector2D::dot(const vector2D& otherVector) const {
    return x * otherVector.x + y * otherVector.y;
}

vector2D vector2D::operator+ (const vector2D& otherVector) const {
    return vector2D(x + otherVector.x, y + otherVector.y);
}

vector2D vector2D::operator- (const vector2D& otherVector) const {
    return vector2D(x - otherVector.x, y - otherVector.y);
}

vector2D vector2D::operator* (const unit& factor) const {
    return vector2D(x * factor, y * factor);
}

vector2D vector2D::operator/ (const unit& factor) const {
    return operator*(1 / factor);
}
