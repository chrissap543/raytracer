#include "math/vec3.h"
#include <cmath>

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(float t) const {
    return Vec3(x * t, y * t, z * t);
}

Vec3 Vec3::operator/(float t) const {
    return *this * (1.0f / t);
}

float Vec3::dot(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

float Vec3::length() const {
    return sqrt(x * x + y * y + z * z);
}

float Vec3::length_squared() const {
    return x * x + y * y + z * z;
}

Vec3 Vec3::normalize() const {
    float len = length();
    return len > 0 ? *this / len : Vec3(0, 0, 0);
}