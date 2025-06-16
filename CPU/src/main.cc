#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include <sys/time.h>
#include <fstream>

// Function to get current cpu time
double cpuSeconds() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}

// Function to calculate the color of the rays in a recursive way
vec3 color(const ray& r, hitable *world, int depth, int max_depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < max_depth && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
             return attenuation*color(scattered, world, depth+1, max_depth);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

// Function to create a pseudo-random scene
// consisting out of 4 constant spehres and numbergrid number of pseudo-random
// spheres
hitable *random_scene(int number_grid) {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    float offset[] = {0.1, -0.42, 0.17, -0.27, 0.29, 0.17, -0.35, 0.11, -0.29};
    float color_offset[] = {0.1, 0.54, 0.87, 0.34, 0.75, 0.18, 0.99, 0.48, 0.74};
    int color_length = 9;
    for (int a = -1*number_grid; a < number_grid; a++) {
        for (int b = -1*number_grid; b < number_grid; b++) {
            vec3 center(a + offset[i%9],0.2,b - offset[i%9]);
                if (i%3==0) {  // diffuse
                    list[i++] = new sphere(center, 0.2 + i%3*0.1, new lambertian(vec3(color_offset[i%color_length], color_offset[(i+2)%color_length], color_offset[(i+5)%color_length])));
                }
                else if (i%3==1) { // metal
                    list[i++] = new sphere(center, 0.2 + i%3*0.1,
                            new metal(vec3(color_offset[(i+3)%color_length], color_offset[i%color_length], color_offset[(i+8)%color_length]),  color_offset[(i+5)%color_length]));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2 + i%3*0.1, new dielectric(1.5));
                }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new hitable_list(list,i);
}

int main(int argc, char* argv[]) {
    int nx = 1200; // number of pixels in x direction
    int ny = 800; // number of pixels in y direction
    int ns = 10; // number of samples per pixel
    int max_depth = 50; // number of reflections
    int number_grid = 11; // number of grid points for little spheres

    if (argc > 1) {
        if (argc != 6) {
            std::cout << "Error in input parameters. \n Shoul be file path, width, samples per pixel, max depth, number of grid points\n" << std::endl;
            return 1;
        }
        nx = std::atoi(argv[2]);
        ny = float(nx) * 0.5625;
        ns = std::atoi(argv[3]);
        max_depth = std::atoi(argv[4]);
        number_grid = std::atoi(argv[5]);
    }

    // Create ppm header
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[5];
    float R = cos(M_PI/4);
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    // create pseudo-random scene
    hitable *world = random_scene(number_grid);

    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    // defining cam settings
    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

    double dStart = cpuSeconds();

    // Calculating all rays for all pixels in a sequential way
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world,0,max_depth);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    // Measure computation time
    double dElaps = cpuSeconds() - dStart;
    std::clog << ("Duration of render: %f s", dElaps);

    // Free memory
    for(int i = 0; i < 5; i++)
        free(list[i]);
    free(world);

    // Store profiling data
    if (argc > 1) {
        std::fstream file;
        std::string filename = argv[1];
        // Check if file exists
        file.open(filename, std::ios::in);
        bool fileExists = file.is_open();
        file.close();

        // Open the file in append mode
        file.open(filename, std::ios::app);
        if (!fileExists) {
            // If file doesn't exists, write a header first
            file << "Width,Samples,Depth,NumberGrid,Time\n";
        }

        // Append new profiling data to the file
        file << nx << "," << ns << "," << max_depth << "," << number_grid << "," << dElaps << "\n";

        file.close();
    }
}
