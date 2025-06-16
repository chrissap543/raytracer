#pragma once
#include "math/vec3.h"
#include "scenes/scene.h"
#include "core/hittable.h"
#include <memory>
#include <chrono>

class Renderer {
public:
    // Render a scene and output to stdout
    static void render_scene(std::unique_ptr<Scene> scene, bool use_kdtree = true);
    
    // Ray color calculation
    static Color ray_color(const Ray& ray, const Hittable& world, int depth);
    
private:
    // Performance timing
    static void print_render_stats(
        const std::chrono::high_resolution_clock::time_point& start_time,
        const std::chrono::high_resolution_clock::time_point& end_time,
        int total_pixels,
        int samples_per_pixel
    );
};