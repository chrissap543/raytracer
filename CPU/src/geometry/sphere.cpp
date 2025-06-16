#include "geometry/sphere.h"
#include "materials/material.h"
#include "math/ray.h"
#include <cmath>

Sphere::Sphere(const Point3& center, float radius, std::shared_ptr<Material> material)
    : center(center), radius(radius), material(material) {}

bool Sphere::hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const {
    Vec3 oc = ray.origin - center;
    float a = ray.direction.length_squared();
    float half_b = oc.dot(ray.direction);
    float c = oc.length_squared() - radius * radius;
    
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    
    float sqrtd = sqrt(discriminant);
    
    // Find the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }
    
    rec.t = root;
    rec.point = ray.at(rec.t);
    Vec3 outward_normal = (rec.point - center) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.material = material.get();
    
    return true;
}

BoundingBox Sphere::bounding_box() const {
    Vec3 radius_vec(radius, radius, radius);
    return BoundingBox(center - radius_vec, center + radius_vec);
}