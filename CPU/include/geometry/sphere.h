#pragma once
#include "core/hittable.h"
#include <memory>

class Material;

// Sphere primitive
class Sphere : public Hittable {
public:
    Point3 center;
    float radius;
    std::shared_ptr<Material> material;
    
    Sphere(const Point3& center, float radius, std::shared_ptr<Material> material);
    
    bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const override;
    BoundingBox bounding_box() const override;
};