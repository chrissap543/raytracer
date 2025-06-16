#pragma once
#include "materials/material.h"

// Lambertian (matte) material
class Lambertian : public Material {
public:
    Color albedo;
    
    Lambertian(const Color& albedo);
    
    bool scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override;
    
private:
    Vec3 random_unit_vector() const;
    bool near_zero(const Vec3& v) const;
};