# CUDA Raytracer

This is our project for the DD2360 course!

The implementation for CPU and CUDA can be found in the corresponding folders.
To compile the code, just use the Makefile with the command `make` which creates the executable called `ray-tracing`.
The raytracer can be run without further input parameters. The resulting image is created in ppm format and is output via the standard output and should be redirect to a file. 

For profiling purposes, the raytracer accept multiple parameters which can be handed over as follows: 
```sh
./ray-tracing [profiling_output.csv] [width] [samples] [depth] [numberGrid] > [image.ppm]
```
- `[profiling_output.csv]` is a csv file to collect the profiling input parameters together with the computation time.
- `[width]` is the width of the rendered image and defines the resolution. The image has a fixed ration of 16:9
- `[samples]` is the number of samples (rays) per pixel
- `[depth]` is the maximum number of reflections of each ray
- `[numberGrid]` defines the number of grid points. On each grid point a small sphere with a peudo-random offset, size and color is generated. The parameter determines half of the edge length of the square grid
- `[image.ppm]` is the file to store the rendered image

For larger profiling sessions, the script `./scripts/profiling_cpu.sh` or `make profiling` can be used. By adjusting the file `profiling_input_parameters.sh` the range of the parameters can be defined.

The output can be checked using a .ppm viewer (like https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html) or by using the self-written python script to convert the ppm file to an jpg file (`python3 ./scripts/convert.py --ppm_file [path_ppm] --jpg-file [path_jpg]`)

## Acknowledgements
The following ray tracer implementation is based on the *Ray Tracing in One Weekend Book* by Peter Shirley, Trevor Black, and Steve Hollasch. The initial code for the ray tracer was from the [InOneWeekend](https://github.com/RayTracing/InOneWeekend.git) GitHub repository. The following project code was modified and written by Niklas Thomas and Sebastian Thaeron.
