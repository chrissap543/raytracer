#include "rendering/camera.h"
#include <cmath>

Camera::Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, float vfov, float aspect_ratio) {
    float theta = vfov * M_PI / 180.0f;
    float h = tan(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;
    
    Vec3 w = (lookfrom - lookat).normalize();
    Vec3 u = vup.cross(w).normalize();
    Vec3 v = w.cross(u);
    
    origin = lookfrom;
    horizontal = u * viewport_width;
    vertical = v * viewport_height;
    lower_left_corner = origin - horizontal/2 - vertical/2 - w;
}

Ray Camera::get_ray(float s, float t) const {
    return Ray(origin, lower_left_corner + horizontal * s + vertical * t - origin);
}