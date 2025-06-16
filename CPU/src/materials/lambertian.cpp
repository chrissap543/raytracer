#include "materials/lambertian.h"
#include "core/hit_record.h"
#include "math/ray.h"
#include <random>
#include <cmath>

Lambertian::Lambertian(const Color& albedo) : albedo(albedo) {}

bool Lambertian::scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
    Vec3 scatter_direction = rec.normal + random_unit_vector();
    
    // Catch degenerate scatter direction
    if (near_zero(scatter_direction)) {
        scatter_direction = rec.normal;
    }
    
    scattered = Ray(rec.point, scatter_direction);
    attenuation = albedo;
    return true;
}

Vec3 Lambertian::random_unit_vector() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    float a = dis(gen) * 2.0f * M_PI;
    float z = dis(gen) * 2.0f - 1.0f;
    float r = sqrt(1.0f - z * z);
    return Vec3(r * cos(a), r * sin(a), z);
}

bool Lambertian::near_zero(const Vec3& v) const {
    const float s = 1e-8f;
    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}