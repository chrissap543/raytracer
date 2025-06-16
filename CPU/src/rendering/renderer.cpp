#include "rendering/renderer.h"
#include "rendering/camera.h"
#include "core/kdtree.h"
#include "core/hittable_list.h"
#include "materials/material.h"
#include "utils/color.h"
#include "math/ray.h"
#include <iostream>
#include <random>
#include <limits>

void Renderer::render_scene(std::unique_ptr<Scene> scene, bool use_kdtree) {
    // Get scene configuration
    SceneConfig config = scene->get_config();
    int image_height = config.get_image_height();
    
    std::cerr << "Rendering: " << scene->get_name() << "\n";
    std::cerr << "Resolution: " << config.image_width << "x" << image_height << "\n";
    std::cerr << "Samples: " << config.samples_per_pixel << "\n";
    std::cerr << "Acceleration: " << (use_kdtree ? "KD-Tree" : "Linear List") << "\n";
    
    // Create objects
    auto objects = scene->create_objects();
    std::cerr << "Objects: " << objects.size() << "\n";
    
    // Create acceleration structure
    std::unique_ptr<Hittable> world;
    if (use_kdtree) {
        auto kdtree = std::make_unique<KDTree>();
        kdtree->build(objects);
        world = std::move(kdtree);
    } else {
        auto list = std::make_unique<HittableList>();
        for (const auto& obj : objects) {
            list->add(obj);
        }
        world = std::move(list);
    }
    
    // Create camera
    Camera cam(config.camera_pos, config.camera_target, config.camera_up, 
               config.camera_fov, config.aspect_ratio);
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Start rendering
    auto render_start = std::chrono::high_resolution_clock::now();
    
    // Output PPM header
    std::cout << "P3\n" << config.image_width << ' ' << image_height << "\n255\n";
    
    // Render pixels
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        
        for (int i = 0; i < config.image_width; ++i) {
            Color pixel_color(0, 0, 0);
            
            // Anti-aliasing samples
            for (int s = 0; s < config.samples_per_pixel; ++s) {
                float u = (i + dis(gen)) / (config.image_width - 1);
                float v = (j + dis(gen)) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color = pixel_color + ray_color(r, *world, config.max_depth);
            }
            
            write_color(std::cout, pixel_color, config.samples_per_pixel);
        }
    }
    
    auto render_end = std::chrono::high_resolution_clock::now();
    
    // Print performance statistics
    std::cerr << "\n";
    print_render_stats(render_start, render_end, 
                      config.image_width * image_height, 
                      config.samples_per_pixel);
}

Color Renderer::ray_color(const Ray& ray, const Hittable& world, int depth) {
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

void Renderer::print_render_stats(
    const std::chrono::high_resolution_clock::time_point& start_time,
    const std::chrono::high_resolution_clock::time_point& end_time,
    int total_pixels,
    int samples_per_pixel) {
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    double seconds = duration.count() / 1000.0;
    
    long long total_rays = static_cast<long long>(total_pixels) * samples_per_pixel;
    double rays_per_second = total_rays / seconds;
    
    std::cerr << "Render completed in " << seconds << " seconds\n";
    std::cerr << "Total rays: " << total_rays << "\n";
    std::cerr << "Rays per second: " << static_cast<long long>(rays_per_second) << "\n";
    std::cerr << "Done.\n";
}