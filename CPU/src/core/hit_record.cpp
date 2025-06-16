#include "core/hit_record.h"
#include "math/ray.h"

void HitRecord::set_face_normal(const Ray& ray, const Vec3& outward_normal) {
    front_face = ray.direction.dot(outward_normal) < 0;
    normal = front_face ? outward_normal : outward_normal * -1;
}