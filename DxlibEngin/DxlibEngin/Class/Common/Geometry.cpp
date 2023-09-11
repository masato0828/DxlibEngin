#include "Geometry.h"

// ‚±‚±‚©‚ç3D
bool Vector3::operator==(const Vector3& v)
{
    return (x_ == v.x_&& y_ == v.y_&& z_ == v.z_);
}

bool Vector3::operator!=(const Vector3& v)
{
    return (x_ != v.x_ && y_ != v.y_ && z_ != v.z_);
}

void Vector3::operator+=(const Vector3& v)
{
    x_ += v.x_;
    y_ += v.y_;
    z_ += v.z_;
}

void Vector3::operator-=(const Vector3& v)
{
    x_ -= v.x_;
    y_ -= v.y_;
    z_ -= v.z_;
}

void Vector3::operator*=(const Vector3& v)
{
    x_ *= v.x_;
    y_ *= v.y_;
    z_ *= v.z_;
}

void Vector3::operator/=(const Vector3& v)
{
    x_ /= v.x_;
    y_ /= v.y_;
    z_ /= v.z_;
}

void Vector3::operator*=(float scale)
{
    x_ *= scale;
    y_ *= scale;
    z_ *= scale;
}

void Vector3::operator/=(float scale)
{
    x_ /= scale;
    y_ /= scale;
    z_ /= scale;
}

bool Vector3::operator>=(float scale)
{
    return (x_ >= scale) && (y_ >= scale) && (z_ >= scale);
}

bool Vector3::operator>(float scale)
{
    return (x_ > scale) && (y_ > scale) && (z_ > scale);
}

bool Vector3::operator<=(float scale)
{
    return (x_ <= scale) && (y_ <= scale) && (z_ <= scale);
}

bool Vector3::operator<(float scale)
{
    return (x_ < scale) && (y_ < scale) && (z_ < scale);
}

Vector3 Vector3::operator+(const Vector3& v)
{
    return { x_ + v.x_, y_ + v.y_, z_ + v.z_ };
}

Vector3 Vector3::operator-(const Vector3& v)
{
    return { x_ - v.x_, y_ - v.y_, z_ - v.z_ };
}

Vector3 Vector3::operator*(const Vector3& v)
{
    return { x_ * v.x_, y_ * v.y_, z_ * v.z_ };
}

Vector3 Vector3::operator/(const Vector3& v)
{
    return { x_ / v.x_, y_ / v.y_, z_ / v.z_ };
}

Vector3 Vector3::operator+(float scale)
{
    return {x_+scale,y_ + scale ,z_ + scale };
}

Vector3 Vector3::operator+(const VECTOR& v)
{
    return { x_ + v.x, y_ + v.y, z_ + v.z };
}

Vector3 Vector3::operator=(const VECTOR& v)
{
    return { x_ = v.x, y_ = v.y, z_ = v.z };
}

bool Vector3::operator>=(const Vector3& v)
{
    return (x_ >= v.x_) && (y_ >= v.y_) && (z_ >= v.z_);
}

bool Vector3::operator>(const Vector3& v)
{
    return (x_ > v.x_) && (y_ > v.y_) && (z_ > v.z_);
}

bool Vector3::operator<=(const Vector3& v)
{
    return (x_ <= v.x_) && (y_ <= v.y_) && (z_ <= v.z_);
}

bool Vector3::operator<(const Vector3& v)
{
    return (x_ < v.x_) && (y_ < v.y_) && (z_< v.z_);
}

VECTOR Vector3::toVECTOR()
{
    return VGet(x_,y_,z_);
}

void Vector3::setVECTOR(float x, float y, float z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

void Vector3::setVECTOR(VECTOR vec)
{
    x_ = vec.x;
    y_ = vec.y;
    z_ = vec.z;
}
