#pragma once
#include "core/hit_record.h"
#include "geometry/bounding_box.h"

class Ray;

// Abstract base class for anything that can be hit by a ray
class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const = 0;
    virtual BoundingBox bounding_box() const = 0;
};