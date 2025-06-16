#include "utils/color.h"
#include <cmath>
#include <algorithm>

void write_color(std::ostream& out, Color pixel_color, int samples_per_pixel) {
    float r = pixel_color.x;
    float g = pixel_color.y;
    float b = pixel_color.z;
    
    // Divide the color by the number of samples and gamma-correct for gamma=2.0
    float scale = 1.0f / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    // Write the translated [0,255] value of each color component
    out << static_cast<int>(256 * std::clamp(r, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0f, 0.999f)) << '\n';
}