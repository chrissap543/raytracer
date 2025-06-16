#include <iostream>
#include <memory>
#include <random>
#include <limits>

#include "math/vec3.h"
#include "math/ray.h"
#include "core/hit_record.h"
#include "core/hittable.h"
#include "core/hittable_list.h"
#include "geometry/sphere.h"
#include "materials/lambertian.h"
#include "rendering/camera.h"
#include "utils/color.h"

// Utility functions
Color ray_color(const Ray& ray, const Hittable& world, int depth) {
    HitRecord rec;
    
    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
        return Color(0, 0, 0);
    
    if (world.hit(ray, 0.001f, std::numeric_limits<float>::infinity(), rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.material->scatter(ray, rec, attenuation, scattered)) {
            Color scattered_color = ray_color(scattered, world, depth - 1);
            return Color(attenuation.x * scattered_color.x, 
                        attenuation.y * scattered_color.y, 
                        attenuation.z * scattered_color.z);
        }
        return Color(0, 0, 0);
    }
    
    // Background gradient
    Vec3 unit_direction = ray.direction.normalize();
    float t = 0.5f * (unit_direction.y + 1.0f);
    return Color(1.0f, 1.0f, 1.0f) * (1.0f - t) + Color(0.5f, 0.7f, 1.0f) * t;
}

int main() {
    // Image settings
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 400;
    const int max_depth = 50;
    
    // World
    HittableList world;
    
    auto material_ground = std::make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
    auto material_center = std::make_shared<Lambertian>(Color(0.7f, 0.3f, 0.3f));
    auto material_left = std::make_shared<Lambertian>(Color(0.0f, 0.0f, 1.0f));
    auto material_right = std::make_shared<Lambertian>(Color(0.8f, 0.6f, 0.2f));
    
    world.add(std::make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    world.add(std::make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
    world.add(std::make_shared<Sphere>(Point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));
    
    // Camera
    Camera cam(Point3(0, 0, 0), Point3(0, 0, -1), Vec3(0, 1, 0), 90, aspect_ratio);
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = (i + dis(gen)) / (image_width - 1);
                float v = (j + dis(gen)) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color = pixel_color + ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    
    std::cerr << "\nDone.\n";
    return 0;
}