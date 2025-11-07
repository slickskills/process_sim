# Process Simulation

A C++ project for simulating physical processes, focusing on heat diffusion and related phenomena. The project uses Eigen for efficient matrix operations and OpenMP for parallel processing.

## Features

- Heat diffusion simulation in 1D and 2D
- Eigen-based matrix operations
- OpenMP parallel processing support
- CMake build system

## Prerequisites

- C++ compiler with C++11 support
- CMake (3.10 or higher)
- Eigen 3.4.0 (automatically downloaded by CMake)
- OpenMP support

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Running Tests

```bash
# Build and run tests
cd build
make test_runner
./test_runner
```

## Project Structure

- `src/` - Source files
- `include/` - Header files
- `test/` - Test files
- `CMakeLists.txt` - CMake build configuration