#include "scenes/complex_scene.h"
#include "geometry/sphere.h"
#include "materials/lambertian.h"
#include <random>
#include <cmath>

std::vector<std::shared_ptr<Hittable>> ComplexScene::create_objects() {
    std::vector<std::shared_ptr<Hittable>> objects;
    
    // Ground
    auto ground_material = std::make_shared<Lambertian>(Color(0.5f, 0.5f, 0.5f));
    objects.push_back(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(42); // Fixed seed for reproducible results
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Generate lots of random spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = dis(gen);
            Point3 center(a + 0.9f * dis(gen), 0.2f, b + 0.9f * dis(gen));
            
            if ((center - Point3(4, 0.2f, 0)).length() > 0.9f) {
                std::shared_ptr<Lambertian> sphere_material;
                
                if (choose_mat < 0.8f) {
                    // Diffuse material with random color
                    Color albedo = random_color();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                } else {
                    // Darker materials
                    Color albedo = random_color() * 0.5f;
                    sphere_material = std::make_shared<Lambertian>(albedo);
                }
                
                objects.push_back(std::make_shared<Sphere>(center, 0.2f, sphere_material));
            }
        }
    }
    
    // Three larger spheres
    auto material1 = std::make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
    objects.push_back(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0f, material1));
    
    auto material2 = std::make_shared<Lambertian>(Color(0.7f, 0.6f, 0.5f));
    objects.push_back(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0f, material2));
    
    auto material3 = std::make_shared<Lambertian>(Color(0.7f, 0.3f, 0.3f));
    objects.push_back(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0f, material3));
    
    return objects;
}

SceneConfig ComplexScene::get_config() {
    SceneConfig config;
    config.aspect_ratio = 3.0f / 2.0f;
    config.image_width = 600;
    config.samples_per_pixel = 50;  // Reduced for faster rendering
    config.max_depth = 50;
    
    // Better camera position for the complex scene
    config.camera_pos = Point3(13, 3, 3);  // Raised camera height from 2 to 3
    config.camera_target = Point3(0, 1, 0);  // Look slightly up to see sphere bottoms
    config.camera_up = Vec3(0, 1, 0);
    config.camera_fov = 25.0f;  // Slightly wider field of view
    
    return config;
}

const char* ComplexScene::get_name() {
    return "Complex Scene (500+ spheres)";
}

Color ComplexScene::random_color() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    return Color(dis(gen), dis(gen), dis(gen));
}