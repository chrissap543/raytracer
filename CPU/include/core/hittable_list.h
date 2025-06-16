#pragma once
#include "core/hittable.h"
#include <vector>
#include <memory>

// Simple list of hittable objects (we'll replace this with kd-tree later)
class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;
    
    void add(std::shared_ptr<Hittable> object);
    void clear();
    
    bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const override;
    BoundingBox bounding_box() const override;
};