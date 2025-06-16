#pragma once
#include "math/vec3.h"

class Ray;
struct HitRecord;

// Simple material base class
class Material {
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};