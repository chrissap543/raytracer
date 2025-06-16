#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <cuda_runtime_api.h>
#include <iostream>
#include <cuda_runtime.h>
#include <curand_kernel.h>

/* Utility Functions */

__device__ inline double random_double(curandState* random_state) {
    // Returns a random double in range [0,1).
	return curand_uniform_double(random_state);
}

__device__ inline float random_float(curandState* random_state) {
    // Returns a random double in range [0,1).
	return curand_uniform(random_state);
}

void checkCudaError(const char* message) {
	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess) {
		std::cerr << "CUDA Error: " << cudaGetErrorString(error) 
                  << "\n\t" << message << std::endl;
		cudaDeviceReset();
        exit(99);
	}
}

#endif
