#pragma once
#include "math/vec3.h"

class Ray;

class BoundingBox {
public:
    Point3 min, max;
    
    BoundingBox();
    BoundingBox(const Point3& min, const Point3& max);
    
    Point3 center() const;
    Vec3 size() const;
    
    // Quick ray-box intersection test
    bool hit(const Ray& ray, float t_min, float t_max) const;
};

// Helper function to create bounding box that contains two boxes
BoundingBox surrounding_box(const BoundingBox& box1, const BoundingBox& box2);