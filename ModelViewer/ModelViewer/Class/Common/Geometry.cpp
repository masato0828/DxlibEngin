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
