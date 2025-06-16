#include "scenes/simple_scene.h"
#include "geometry/sphere.h"
#include "materials/lambertian.h"

std::vector<std::shared_ptr<Hittable>> SimpleScene::create_objects() {
    std::vector<std::shared_ptr<Hittable>> objects;
    
    // Materials
    auto material_ground = std::make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
    auto material_center = std::make_shared<Lambertian>(Color(0.7f, 0.3f, 0.3f));
    auto material_left = std::make_shared<Lambertian>(Color(0.0f, 0.0f, 1.0f));
    auto material_right = std::make_shared<Lambertian>(Color(0.8f, 0.6f, 0.2f));
    
    // Objects
    objects.push_back(std::make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    objects.push_back(std::make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f, material_center));
    objects.push_back(std::make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
    objects.push_back(std::make_shared<Sphere>(Point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));
    
    return objects;
}

SceneConfig SimpleScene::get_config() {
    SceneConfig config;
    config.aspect_ratio = 16.0f / 9.0f;
    config.image_width = 400;
    config.samples_per_pixel = 100;
    config.max_depth = 50;
    
    config.camera_pos = Point3(0, 0, 0);
    config.camera_target = Point3(0, 0, -1);
    config.camera_up = Vec3(0, 1, 0);
    config.camera_fov = 90.0f;
    
    return config;
}

const char* SimpleScene::get_name() {
    return "Simple Scene (4 spheres)";
}