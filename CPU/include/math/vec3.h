#pragma once

class Vec3 {
public:
    float x, y, z;
    
    Vec3();
    Vec3(float x, float y, float z);
    
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float t) const;
    Vec3 operator/(float t) const;
    
    float dot(const Vec3& v) const;
    Vec3 cross(const Vec3& v) const;
    
    float length() const;
    float length_squared() const;
    Vec3 normalize() const;
};

// Type aliases for clarity
using Point3 = Vec3;
using Color = Vec3;