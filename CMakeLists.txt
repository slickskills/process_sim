# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Define the project name and language
project(HighPerfSim CXX)

# Set the C++ standard to C++17 (a good modern baseline)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 1. Fetch and Configure Eigen (Header-Only Library) ---
# Use CMake's FetchContent to automatically download the Eigen source
# This avoids needing to manually install Eigen on the system.
include(FetchContent)
FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
)
FetchContent_MakeAvailable(Eigen)

# --- 2. Configure OpenMP for Parallelization ---
# Find the OpenMP components
find_package(OpenMP REQUIRED)

# --- 3. Define the Core Simulator Target ---
# This is the main simulation executable
add_executable(simulator_core 
    src/main.cpp
    src/Simulator.cpp
)

# Add include directories
target_include_directories(simulator_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the dependencies: Eigen and OpenMP
target_link_libraries(simulator_core PUBLIC Eigen3::Eigen ${OpenMP_CXX_FLAGS})

# Add compiler flags for OpenMP activation
if(OpenMP_CXX_FLAGS)
    target_compile_options(simulator_core PUBLIC ${OpenMP_CXX_FLAGS})
endif()

# --- 4. Define the Test Runner Target (Module 1.2 Verification) ---
# This target uses the verification file and links the required libraries.
add_executable(test_runner test/test_eigen.cpp)
# The test target is a standalone executable (it defines its own main)
# and should not link to the `simulator_core` executable. Remove the
# invalid link-to-executable. If shared logic is needed by multiple
# targets, extract it into a library target and link that instead.

# Inherit the OpenMP and Eigen settings for the test runner
target_link_libraries(test_runner PUBLIC Eigen3::Eigen ${OpenMP_CXX_FLAGS})

# Add include directories to test target
target_include_directories(test_runner PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(OpenMP_CXX_FLAGS)
    target_compile_options(test_runner PUBLIC ${OpenMP_CXX_FLAGS})
endif()

# --- 5. Mesh reader and 2D heat example ---
add_library(mesh_reader STATIC
    src/mesh/GmshReader.cpp
)
target_include_directories(mesh_reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mesh_reader PUBLIC Eigen3::Eigen)

add_library(solvers2d STATIC
    src/solvers/HeatSolver2D.cpp
)
target_include_directories(solvers2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(solvers2d PUBLIC Eigen3::Eigen)

add_executable(heat2d_example src/heat2d_example.cpp)
target_include_directories(heat2d_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(heat2d_example PUBLIC mesh_reader solvers2d Eigen3::Eigen ${OpenMP_CXX_FLAGS})

