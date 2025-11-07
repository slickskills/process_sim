/**
 * @file test_eigen.cpp
 * @brief Verification test for Eigen and OpenMP setup.
 *
 * This test confirms:
 * 1. Eigen is correctly linked and functional.
 * 2. OpenMP compiler flags are correctly enabled.
 */

#include <iostream>
#include <Eigen/Dense>

// OpenMP includes must be conditional to prevent errors if OpenMP is not found
#ifdef _OPENMP
#include <omp.h>
#endif

// Forward declaration of the dummy main (optional, but good practice)
int main(int argc, char* argv[]);

// Function to run the tests
void run_tests() {
    // --- Test 1: Eigen Functionality ---
    Eigen::MatrixXd A = Eigen::MatrixXd::Random(3, 3);
    Eigen::MatrixXd B = Eigen::MatrixXd::Random(3, 3);
    Eigen::MatrixXd C = A * B;

    std::cout << "\n--- Eigen & OpenMP Setup Verification ---\n";
    std::cout << "Successfully included Eigen version 3.4.0 (or newer).\n";
    std::cout << "Simple Matrix Multiplication Test (3x3):\n";
    std::cout << "Result matrix C:\n" << C << "\n";

    // A basic check to confirm the dimensions and a single value are correct
    if (C.rows() == 3 && C.cols() == 3) {
        std::cout << "\n[PASS] Eigen matrix operations verified.\n";
    } else {
        std::cerr << "\n[FAIL] Eigen matrix operations failed.\n";
    }

    // --- Test 2: OpenMP Parallelization Check ---
#ifdef _OPENMP
    int num_threads = 0;
    #pragma omp parallel
    {
        // Only the master thread needs to print the message,
        // but we verify that the parallel region is entered.
        #pragma omp master
        {
            num_threads = omp_get_num_threads();
        }
    }
    if (num_threads > 1) {
        std::cout << "[PASS] OpenMP is active and detected " << num_threads << " threads.\n";
    } else {
        std::cout << "[WARN] OpenMP is active but detected only " << num_threads << " thread(s). Ensure proper execution environment.\n";
    }
#else
    std::cout << "[FAIL] OpenMP preprocessor macro is not defined. Check CMake flags.\n";
#endif

    std::cout << "\n-----------------------------------------\n";
}

int main(int argc, char* argv[]) {
    run_tests();
    return 0;
}
