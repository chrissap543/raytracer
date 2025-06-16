#include "geometry/bounding_box.h"
#include "math/ray.h"
#include <algorithm>

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const Point3& min, const Point3& max) : min(min), max(max) {}

Point3 BoundingBox::center() const {
    return (min + max) * 0.5f;
}

Vec3 BoundingBox::size() const {
    return max - min;
}

bool BoundingBox::hit(const Ray& ray, float t_min, float t_max) const {
    for (int axis = 0; axis < 3; axis++) {
        float axis_min, axis_max;
        float ray_origin, ray_dir;
        
        if (axis == 0) {
            axis_min = min.x; axis_max = max.x;
            ray_origin = ray.origin.x; ray_dir = ray.direction.x;
        } else if (axis == 1) {
            axis_min = min.y; axis_max = max.y;
            ray_origin = ray.origin.y; ray_dir = ray.direction.y;
        } else {
            axis_min = min.z; axis_max = max.z;
            ray_origin = ray.origin.z; ray_dir = ray.direction.z;
        }
        
        float inv_dir = 1.0f / ray_dir;
        float t0 = (axis_min - ray_origin) * inv_dir;
        float t1 = (axis_max - ray_origin) * inv_dir;
        
        if (inv_dir < 0.0f) {
            std::swap(t0, t1);
        }
        
        t_min = std::max(t0, t_min);
        t_max = std::min(t1, t_max);
        
        if (t_max <= t_min) {
            return false;
        }
    }
    return true;
}

BoundingBox surrounding_box(const BoundingBox& box1, const BoundingBox& box2) {
    Point3 small(
        std::min(box1.min.x, box2.min.x),
        std::min(box1.min.y, box2.min.y),
        std::min(box1.min.z, box2.min.z)
    );
    Point3 big(
        std::max(box1.max.x, box2.max.x),
        std::max(box1.max.y, box2.max.y),
        std::max(box1.max.z, box2.max.z)
    );
    return BoundingBox(small, big);
}