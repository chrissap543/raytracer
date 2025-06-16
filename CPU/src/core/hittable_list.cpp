#include "core/hittable_list.h"
#include "geometry/bounding_box.h"

void HittableList::add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
}

void HittableList::clear() {
    objects.clear();
}

bool HittableList::hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    
    for (const auto& object : objects) {
        if (object->hit(ray, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    
    return hit_anything;
}

BoundingBox HittableList::bounding_box() const {
    if (objects.empty()) return BoundingBox();
    
    BoundingBox output_box = objects[0]->bounding_box();
    for (size_t i = 1; i < objects.size(); i++) {
        output_box = surrounding_box(output_box, objects[i]->bounding_box());
    }
    return output_box;
}