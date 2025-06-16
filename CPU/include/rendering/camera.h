#pragma once
#include "math/vec3.h"
#include "math/ray.h"

// Camera class
class Camera {
public:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    
    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, float vfov, float aspect_ratio);
    
    Ray get_ray(float s, float t) const;
};