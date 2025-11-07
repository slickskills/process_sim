#pragma once

#include "Grid1D.hpp"
#include <Eigen/Sparse>

namespace hpsim {

/**
 * @brief Main simulation controller for the 1D heat equation solver
 * 
 * Manages the simulation lifecycle, including initialization,
 * time stepping, and boundary conditions.
 */
class Simulator {
public:
    /**
     * @brief Construct a new Simulator
     * 
     * @param length Rod length in meters
     * @param alpha Thermal diffusivity in m²/s
     * @param points Number of grid points
     * @param dt Time step in seconds
     */
    Simulator(double length, double alpha, int points, double dt);

    /**
     * @brief Set up initial temperature profile and boundary conditions
     */
    void initialize_conditions();

    /**
     * @brief Run the simulation for a specified number of time steps
     * 
     * @param num_steps Number of time steps to simulate
     */
    void run_simulation(int num_steps);

    /**
     * @brief Get the computational grid
     * 
     * @return const Grid1D& Reference to the grid object
     */
    const Grid1D& get_grid() const;

    /**
     * @brief Get the current simulation time
     * 
     * @return double Current time in seconds
     */
    double get_time() const;

    /**
     * @brief Write the current temperature profile to a CSV file
     * 
     * @param filename Path to the output CSV file
     * @return true if write successful, false otherwise
     */
    bool write_results_to_csv(const std::string& filename) const;

private:
    /**
     * @brief Perform a single time step update using Crank-Nicolson scheme
     * 
     * Solves the tridiagonal system AT(n+1) = BT(n) + b using Eigen's sparse solver
     * where A and B are tridiagonal matrices with:
     * - A main diagonal: 1 + 2r
     * - A off-diagonals: -r
     * - B main diagonal: 1 - 2r
     * - B off-diagonals: r
     * and r = α*dt/(2*dx^2)
     */
    void update_step_crank_nicolson();

    // Set up the sparse tridiagonal matrices for Crank-Nicolson
    void setup_matrices();

private:
    Grid1D grid_;              // The computational grid
    double time_step_s_;       // Time step size [s]
    double current_time_s_;    // Current simulation time [s]
    double r_factor_;          // Pre-calculated Fourier number

    // Sparse matrices for Crank-Nicolson scheme
    Eigen::SparseMatrix<double> matrix_A_;  // LHS matrix
    Eigen::SparseMatrix<double> matrix_B_;  // RHS matrix
    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver_; // Sparse LU solver
};

} // namespace hpsim