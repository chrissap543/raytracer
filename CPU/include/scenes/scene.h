#pragma once
#include "core/hittable.h"
#include "rendering/camera.h"
#include <vector>
#include <memory>

// Scene configuration struct
struct SceneConfig {
    float aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 50;
    
    Point3 camera_pos = Point3(0, 0, 0);
    Point3 camera_target = Point3(0, 0, -1);
    Vec3 camera_up = Vec3(0, 1, 0);
    float camera_fov = 90.0f;
    
    int get_image_height() const {
        return static_cast<int>(image_width / aspect_ratio);
    }
};

// Abstract scene base class
class Scene {
public:
    virtual ~Scene() = default;
    virtual std::vector<std::shared_ptr<Hittable>> create_objects() = 0;
    virtual SceneConfig get_config() = 0;
    virtual const char* get_name() = 0;
};