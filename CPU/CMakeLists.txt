cmake_minimum_required(VERSION 3.16)
project(Raytracer VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

# Include directories
include_directories(include)

# Collect all source files (including main.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable with all source files
add_executable(raytracer ${SOURCES})

# Link math library on Unix systems
if(UNIX)
    target_link_libraries(raytracer m)
endif()

# Platform-specific compiler flags
if(MSVC)
    target_compile_options(raytracer PRIVATE /W4)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra)
endif()

# Optional: Custom target for convenience
add_custom_target(run
    COMMAND raytracer > output.ppm
    DEPENDS raytracer
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running raytracer and saving output to output.ppm"
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")