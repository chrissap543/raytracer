#include <curand_kernel.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include "float.h"
#include <cuda_runtime_api.h>
#include <curand_uniform.h>

#include "vec3.cuh"
#include "ray.cuh"
#include "sphere.cuh"
#include "hitable_list.cuh"
#include "camera.cuh"
#include "material.cuh"
#include "utility.cuh"

#include <iostream>
#include <sys/time.h>
#include <fstream>

/* Auxiliary Functions */

// Utility function to get current CPU time in seconds
double cpuSeconds() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec*1.e-6);
}

// Device function to recursively calculate the color of a ray
__device__ vec3 color(const ray& r, hitable **world, int depth, int max_depth, curandState* rand_state) {
    hit_record rec;
    if ((*world)->hit(r, 0.001, MAXFLOAT, rec)) { 
        ray scattered;
        vec3 attenuation;
        if (depth < max_depth && rec.mat_ptr->scatter(r, rec, attenuation, scattered, rand_state)) {
             return attenuation*color(scattered, world, depth+1, max_depth, rand_state);
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

// Device function to create a peudo-random world scene defined in GPU memory
__device__ hitable* random_scene(int number_grid, hitable **list, curandState *random_state) {
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    float offset[] = {0.1, -0.42, 0.17, -0.27, 0.29, 0.17, -0.35, 0.11, -0.29};
    float color_offset[] = {0.1, 0.54, 0.87, 0.34, 0.75, 0.18, 0.99, 0.48, 0.74};
    int color_length = 9;
    for (int a = -1*number_grid; a < number_grid; a++) {
        for (int b = -1*number_grid; b < number_grid; b++) {
            vec3 center(a + offset[i%9],0.2,b - offset[i%9]);
            //if ((center-vec3(4,0.2,0)).length() > 0.9) {
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
            //}
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, 4*number_grid*number_grid+4);
}

/* CUDA Kernel Functions */

// Kernel functon to create cuda random states foreach launched thread,
// each has a unqiue seed
__global__ void random_render_init(int width, int height, curandState *random_state_list) {
	int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
	if ((i >= width) || (j >= height)) return;
	int pixel_index = j*width + i;
	curand_init(1984+pixel_index, 0, 0, &random_state_list[pixel_index]);
}

// Kernel function to create world scene and camera defined in GPU memory
__global__ void create_world(int width, int height, int number_grid, hitable **list, camera **world_camera, hitable **world, curandState *random_state) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		hitable **temp_list = new hitable*[5];
		temp_list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
		temp_list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
		temp_list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
		temp_list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
		temp_list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
		*world = new hitable_list(temp_list,5);
		*world = random_scene(number_grid, list, random_state);
		
		vec3 lookfrom(13,2,3);
		vec3 lookat(0,0,0);
		float dist_to_focus = 10.0;
		float aperture = 0.1;
		*world_camera = new camera(lookfrom, lookat, vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);
	}
}

// Kernel function to render all rays of a pixel
__global__ void render(vec3 *frame_buffer, int width, int height, int samples, int max_depth, camera **world_camera, hitable **world, curandState *random_state_list) {
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	if ((i >= width) || (j >= height)) return;
	int pixel_index = j*width + i;
	curandState current_random_state = random_state_list[pixel_index];
	vec3 col(0, 0, 0);
	for (int k = 0; k < samples; k++) {
		float u = float(i + random_double(&current_random_state)) / float(width);
		float v = float(j + random_double(&current_random_state)) / float(height);
		ray r = (*world_camera)->get_ray(u, v, &current_random_state);
		vec3 p = r.point_at_parameter(2.0);
		col += color(r, world,0, max_depth, &current_random_state);
	}
	random_state_list[pixel_index] = current_random_state; 
	col /= float(samples);
	col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
	frame_buffer[pixel_index] = col;
}

// Kernel function to free the memory defined in GPU memory
__global__ void free_world(int hitable_number, hitable **list, camera **world_camera, hitable **world) {
	for (int i = 0; i < hitable_number; i++) {
		delete ((sphere *)list[i])->mat_ptr;
        delete list[i];
	}
	delete *world;
	delete *world_camera;
}

int main(int argc, char* argv[]) {
    int nx = 1200;				// Number of pixels in x direction
    int ny = 800; 				// Number of pixels in y direction
    int ns = 10; 				// Number of samples per pixel
    int max_depth = 50; 		// Number of reflections
    int number_grid = 11; 		// Number of grid points for little spheres

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

	size_t stackSize = 8192;
	cudaDeviceSetLimit(cudaLimitStackSize, stackSize);

	// CUDA memory allocation
	int pixels = nx * ny;
	vec3* frame_buffer;
	cudaMallocManaged(&frame_buffer, pixels * sizeof(vec3));
	checkCudaError("CUDA Malloc frame buffer failed");

	curandState* device_random_state;
	cudaMalloc(&device_random_state, sizeof(curandState));
	checkCudaError("CUDA Malloc random state failed");

	curandState* device_random_state_list;
	cudaMalloc(&device_random_state_list, pixels * sizeof(curandState));
	checkCudaError("CUDA Malloc random state list failed");

    int hitable_number = 4*number_grid*number_grid+4;
	hitable **device_list;
	cudaMalloc(&device_list, hitable_number * sizeof(hitable *));
	checkCudaError("CUDA Malloc list failed");

	hitable **device_world;
	cudaMalloc(&device_world, sizeof(hitable *));
	checkCudaError("CUDA Malloc world failed");

	camera **device_camera;
	cudaMalloc(&device_camera, sizeof(camera *));
	checkCudaError("CUDA Malloc camera failed");

	// Create world kernel launch
	cudaDeviceSynchronize();
	checkCudaError("CUDA Sync random world init failed");
	create_world<<<1, 1>>>(nx, ny, number_grid, device_list, device_camera, device_world, device_random_state);
	checkCudaError("CUDA create world kernel failed");
	cudaDeviceSynchronize();
	checkCudaError("CUDA Sync create world failed");

    // setup vars for kernel
	int threads = 8;
	dim3 block_size(threads, threads, 1);
	dim3 grid_size(nx/block_size.x + 1, ny/block_size.y + 1, 1);

	double dStart = cpuSeconds();

	// CUDA render kernel calls
	std::clog << "Init" << std::endl;
	random_render_init<<<grid_size, block_size>>>(nx, ny, device_random_state_list);
	checkCudaError("CUDA random render init kernel failed");
	cudaDeviceSynchronize();
	checkCudaError("CUDA Sync random render init failed");
	std::clog << "Render" << std::endl;
	render<<<grid_size, block_size>>>(frame_buffer, nx, ny, ns, max_depth, device_camera, device_world, device_random_state_list);
	checkCudaError("CUDA render kernel failed");
	cudaDeviceSynchronize();
	checkCudaError("CUDA Sync render failed");
	// Output frame buffer as ppm image
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			int pixel_index = j*nx+i;
			int ir = int(255.99*frame_buffer[pixel_index].r()); 
			int ig = int(255.99*frame_buffer[pixel_index].g()); 
			int ib = int(255.99*frame_buffer[pixel_index].b()); 
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	double dElaps = cpuSeconds() - dStart;
	std::clog << "Duration of render: " << dElaps << "s" << std::endl;


	// Free memory
	std::clog << "Free the world" << std::endl;
	free_world<<<1,1>>>(hitable_number, device_list, device_camera, device_world);
	checkCudaError("CUDA free world kernel failed");
	cudaDeviceSynchronize();
	checkCudaError("CUDA Sync free world failed");
	cudaFree(device_world);
	checkCudaError("CUDA Free world failed");
	cudaFree(device_camera);
	checkCudaError("CUDA Free camera failed");
	cudaFree(device_list);
	checkCudaError("CUDA Free list failed");
	cudaFree(device_random_state);
	checkCudaError("CUDA Free random state failed");
	cudaFree(device_random_state_list);
	checkCudaError("CUDA Free random state list failed");
	cudaFree(frame_buffer);
	checkCudaError("CUDA Free frame buffer failed");

	cudaDeviceReset();

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

