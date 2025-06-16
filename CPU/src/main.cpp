#include <iostream>
#include <memory>
#include <string>

#include "rendering/renderer.h"
#include "scenes/simple_scene.h"
#include "scenes/complex_scene.h"

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [scene] [options]\n";
    std::cerr << "\nScenes:\n";
    std::cerr << "  simple  - Simple scene with 4 spheres (default)\n";
    std::cerr << "  complex - Complex scene with 500+ spheres\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  --list     - Use linear list instead of kd-tree\n";
    std::cerr << "  --kdtree   - Use kd-tree acceleration (default)\n";
    std::cerr << "\nExamples:\n";
    std::cerr << "  " << program_name << " simple > simple.ppm\n";
    std::cerr << "  " << program_name << " complex --list > complex_slow.ppm\n";
    std::cerr << "  " << program_name << " complex --kdtree > complex_fast.ppm\n";
}

int main(int argc, char* argv[]) {
    // Default settings
    std::string scene_type = "simple";
    bool use_kdtree = true;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "simple" || arg == "complex") {
            scene_type = arg;
        } else if (arg == "--list") {
            use_kdtree = false;
        } else if (arg == "--kdtree") {
            use_kdtree = true;
        } else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }
    
    // Create the appropriate scene
    std::unique_ptr<Scene> scene;
    if (scene_type == "simple") {
        scene = std::make_unique<SimpleScene>();
    } else if (scene_type == "complex") {
        scene = std::make_unique<ComplexScene>();
    } else {
        std::cerr << "Unknown scene type: " << scene_type << "\n";
        print_usage(argv[0]);
        return 1;
    }
    
    // Render the scene
    try {
        Renderer::render_scene(std::move(scene), use_kdtree);
    } catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}