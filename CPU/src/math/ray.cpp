#include "math/ray.h"

Ray::Ray() {}

Ray::Ray(const Point3& origin, const Vec3& direction) 
    : origin(origin), direction(direction) {}

Point3 Ray::at(float t) const {
    return origin + direction * t;
}