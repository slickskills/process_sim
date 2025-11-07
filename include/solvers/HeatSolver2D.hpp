#pragma once

#include <vector>
#include <Eigen/Sparse>
#include "mesh/Mesh.hpp"
#include "Grid2D.hpp"

namespace hpsim {

class HeatSolver2D {
public:
    HeatSolver2D() = default;

    /// Initialize solver with mesh and constant diffusion coefficient
    void initialize(const Mesh2D &mesh, double diffusion_coeff);

    /// Set Dirichlet nodes and their prescribed values
    void set_dirichlet(const std::vector<int> &node_indices, const std::vector<double> &values);

    /// Assemble system matrices (mass and stiffness)
    void assemble_matrices();

    /// Advance one implicit Euler timestep: state <- (M + dt*K)^{-1} * M * state
    void step_implicit(Grid2D &grid, double dt);

private:
    Mesh2D mesh_;
    double D_ = 1.0; // diffusion coefficient

    Eigen::SparseMatrix<double> M_; // mass
    Eigen::SparseMatrix<double> K_; // stiffness
    Eigen::SparseMatrix<double> A_; // system matrix for current dt (M + dt*K)

    std::vector<int> dirichlet_nodes_;
    std::vector<double> dirichlet_values_;

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver_;

    // Option to reuse the factorization of A = M + dt*K when dt is constant across steps.
    bool reuse_factorization_ = false;
    bool factorization_valid_ = false;
    double cached_dt_ = -1.0;

public:
    /// Enable or disable reuse of factorization (default: false)
    void set_reuse_factorization(bool reuse) { reuse_factorization_ = reuse; }

    void apply_dirichlet(Eigen::SparseMatrix<double> &mat, Eigen::VectorXd &rhs);
};

} // namespace hpsim
