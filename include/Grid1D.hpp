/**
 * @file Grid1D.hpp
 * @brief Defines the data structure for the 1D simulation grid state and parameters.
 *
 * This structure holds the physical properties and the current temperature state
 * as an Eigen vector for high-performance numerical operations.
 */
#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <vector>

namespace hpsim {

/**
 * @class Grid1D
 * @brief Represents the physical state of the 1D rod.
 */
class Grid1D {
public:
    // --- Physical Parameters ---
    double length_m = 1.0;          ///< Length of the rod in meters
    double time_step_s = 0.01;      ///< Time step (delta t)
    double thermal_diffusivity;     ///< Alpha (constant in the heat equation)
    int num_points = 100;           ///< Number of discrete grid points (N)

    // Derived geometric property
    double delta_x;                 ///< Spatial step size (delta x)

    // --- State Vector ---
    Eigen::VectorXd temperature_state; ///< Current temperature values at N points

    /**
     * @brief Constructor that initializes derived properties and the state vector.
     * @param len Length of the domain.
     * @param alpha Thermal diffusivity constant.
     * @param n Number of grid points.
     */
    Grid1D(double len, double alpha, int n)
        : length_m(len), thermal_diffusivity(alpha), num_points(n)
    {
        // Calculate spatial step size
        delta_x = length_m / (num_points - 1);

        // Initialize the temperature state vector (N elements) to zero
        temperature_state = Eigen::VectorXd::Zero(num_points);
    }

    /**
     * @brief Getter for the state vector size.
     * @return The number of points N.
     */
    int size() const {
        return num_points;
    }
};

} // namespace hpsim