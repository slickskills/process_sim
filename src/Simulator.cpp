/**
 * @file Simulator.cpp
 * @brief Implementation of the Simulator class, managing the simulation lifecycle.
 */

#include "Simulator.hpp"
#include <iostream>
#include <fstream>
#include <cmath> // For std::sin in boundary/initial conditions

namespace hpsim {

Simulator::Simulator(double length, double alpha, int points, double dt)
    // Initialize the grid object using the provided parameters
    : grid_(length, alpha, points),
      time_step_s_(dt),
      current_time_s_(0.0)
{
    // Ensure the time step is set on the grid object as well (for consistency)
    grid_.time_step_s = dt;
    std::cout << "Simulator initialized for a 1D rod.\n";
    std::cout << "  - Grid Points (N): " << grid_.num_points << "\n";
    std::cout << "  - Spatial Step (dx): " << grid_.delta_x << " m\n";
    std::cout << "  - Time Step (dt): " << grid_.time_step_s << " s\n";

    // Pre-calculate the Fourier Number (r = alpha * dt / dx^2)
    double dx_sq = grid_.delta_x * grid_.delta_x;
    r_factor_ = grid_.thermal_diffusivity * grid_.time_step_s / dx_sq;

    std::cout << "  - Fourier Number (r): " << r_factor_ << "\n";
}

void Simulator::initialize_conditions() {
    // --- Initial Conditions: Set a simple sine wave profile ---
    // T(x, 0) = sin(pi * x / L)
    for (int i = 0; i < grid_.num_points; ++i) {
        double x = i * grid_.delta_x;
        // Use a simple initial condition for verification: a peak in the middle
        grid_.temperature_state(i) = std::sin(M_PI * x / grid_.length_m);
    }

    // --- Boundary Conditions (Fixed Temperature/Dirichlet) ---
    // T(0, t) = 0, T(L, t) = 0
    // These are already handled by the sine wave and will be enforced by the solver.
    grid_.temperature_state(0) = 0.0;
    grid_.temperature_state(grid_.num_points - 1) = 0.0;

    std::cout << "Initial conditions set (Sinusoidal profile).\n";
}

void Simulator::setup_matrices() {
    const int N = grid_.num_points;
    
    // Initialize sparse matrices with estimated non-zeros per row
    matrix_A_.resize(N, N);
    matrix_B_.resize(N, N);
    
    // Reserve space for 3 non-zeros per row (tridiagonal)
    matrix_A_.reserve(Eigen::VectorXi::Constant(N, 3));
    matrix_B_.reserve(Eigen::VectorXi::Constant(N, 3));
    
    // Fill matrices using triplets
    std::vector<Eigen::Triplet<double>> tripletsA, tripletsB;
    tripletsA.reserve(3*N);
    tripletsB.reserve(3*N);
    
    // Calculate coefficients
    const double r = r_factor_; // r = α*dt/(2*dx^2)
    
    // First row (boundary condition)
    tripletsA.emplace_back(0, 0, 1.0);
    tripletsB.emplace_back(0, 0, 1.0);
    
    // Interior points
    for(int i = 1; i < N-1; ++i) {
        // Matrix A (LHS)
        tripletsA.emplace_back(i, i-1, -r);
        tripletsA.emplace_back(i, i, 1.0 + 2.0*r);
        tripletsA.emplace_back(i, i+1, -r);
        
        // Matrix B (RHS)
        tripletsB.emplace_back(i, i-1, r);
        tripletsB.emplace_back(i, i, 1.0 - 2.0*r);
        tripletsB.emplace_back(i, i+1, r);
    }
    
    // Last row (boundary condition)
    tripletsA.emplace_back(N-1, N-1, 1.0);
    tripletsB.emplace_back(N-1, N-1, 1.0);
    
    // Set matrix elements from triplets
    matrix_A_.setFromTriplets(tripletsA.begin(), tripletsA.end());
    matrix_B_.setFromTriplets(tripletsB.begin(), tripletsB.end());
    
    // Analyze pattern and prepare for solving
    solver_.analyzePattern(matrix_A_);
    solver_.factorize(matrix_A_);
}

void Simulator::update_step_crank_nicolson() {
    // Compute RHS: B * T(n)
    Eigen::VectorXd rhs = matrix_B_ * grid_.temperature_state;
    
    // Solve system A * T(n+1) = rhs for T(n+1)
    grid_.temperature_state = solver_.solve(rhs);
    
    // Update time
    current_time_s_ += time_step_s_;
}

void Simulator::run_simulation(int num_steps) {
    std::cout << "\nStarting simulation for " << num_steps << " steps...\n";

    // Set up matrices for Crank-Nicolson scheme
    setup_matrices();

    for (int step = 1; step <= num_steps; ++step) {
        // Perform Crank-Nicolson update
        update_step_crank_nicolson();
        
        if (step % 100 == 0) {
            std::cout << "Step: " << step << " | Time: " << current_time_s_ << " s\n";
        }
    }

    std::cout << "Simulation finished. Final time: " << current_time_s_ << " s\n";
}

const Grid1D& Simulator::get_grid() const {
    return grid_;
}

double Simulator::get_time() const {
    return current_time_s_;
}

bool Simulator::write_results_to_csv(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return false;
    }

    // Write CSV header
    file << "X_Position,Temperature\n";

    // Write data points
    for (int i = 0; i < grid_.num_points; ++i) {
        double x = i * grid_.delta_x;
        file << x << "," << grid_.temperature_state(i) << "\n";
    }

    file.close();
    std::cout << "Results written to " << filename << "\n";
    return true;
}

} // namespace hpsim