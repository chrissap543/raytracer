#pragma once
#include "math/vec3.h"

class Ray {
public:
    Point3 origin;
    Vec3 direction;
    
    Ray();
    Ray(const Point3& origin, const Vec3& direction);
    
    Point3 at(float t) const;
};