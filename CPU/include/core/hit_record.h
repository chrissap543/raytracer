#pragma once
#include "math/vec3.h"

// Forward declaration
class Material;
class Ray;

struct HitRecord {
    Point3 point;           // Where the ray hit
    Vec3 normal;           // Surface normal at hit point
    float t;               // Distance along ray
    bool front_face;       // Did ray hit from outside?
    Material* material;    // What material was hit
    
    void set_face_normal(const Ray& ray, const Vec3& outward_normal);
};