#include "solvers/HeatSolver2D.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <set>

namespace hpsim {

void HeatSolver2D::initialize(const Mesh2D &mesh, double diffusion_coeff) {
    mesh_ = mesh;
    D_ = diffusion_coeff;
}

void HeatSolver2D::set_dirichlet(const std::vector<int> &node_indices, const std::vector<double> &values) {
    dirichlet_nodes_ = node_indices;
    dirichlet_values_ = values;
}

// Helper: compute triangle area and gradients of linear shape functions
static double triangle_area(const Node2D &n1, const Node2D &n2, const Node2D &n3) {
    double x1 = n1.x, y1 = n1.y;
    double x2 = n2.x, y2 = n2.y;
    double x3 = n3.x, y3 = n3.y;
    return 0.5 * std::abs(x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2));
}

void HeatSolver2D::assemble_matrices() {
    using Trip = Eigen::Triplet<double>;
    int N = mesh_.num_nodes();

    std::vector<Trip> tripM;
    std::vector<Trip> tripK;

    tripM.reserve(mesh_.num_elems() * 9);
    tripK.reserve(mesh_.num_elems() * 9);

    for (const auto &el : mesh_.elems) {
        const Node2D &n1 = mesh_.nodes[el.nodes[0]];
        const Node2D &n2 = mesh_.nodes[el.nodes[1]];
        const Node2D &n3 = mesh_.nodes[el.nodes[2]];

        double area = triangle_area(n1,n2,n3);
        if (area <= 0) continue;

        // Compute gradients of barycentric shape functions (constant per element)
        // grad N1 = [ (y2 - y3), (x3 - x2) ] / (2A)
        Eigen::Matrix<double,2,3> grads; // columns are grad Ni
        grads(0,0) = (n2.y - n3.y);
        grads(1,0) = (n3.x - n2.x);
        grads(0,1) = (n3.y - n1.y);
        grads(1,1) = (n1.x - n3.x);
        grads(0,2) = (n1.y - n2.y);
        grads(1,2) = (n2.x - n1.x);
        grads /= (2.0*area);

        // Element stiffness Ke = D * area * (grads^T * grads)
        Eigen::Matrix3d Ke = Eigen::Matrix3d::Zero();
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                Ke(i,j) = D_ * area * (grads.col(i).dot(grads.col(j)));

        // Element mass Me = (area/12) * [2 1 1; 1 2 1; 1 1 2]
        Eigen::Matrix3d Me;
        Me << 2,1,1,
              1,2,1,
              1,1,2;
        Me *= (area / 12.0);

        // Assemble into global triplets
        for (int i = 0; i < 3; ++i) {
            int Ii = el.nodes[i];
            for (int j = 0; j < 3; ++j) {
                int Jj = el.nodes[j];
                tripK.emplace_back(Ii, Jj, Ke(i,j));
                tripM.emplace_back(Ii, Jj, Me(i,j));
            }
        }
    }

    M_.resize(N,N);
    K_.resize(N,N);
    M_.setFromTriplets(tripM.begin(), tripM.end());
    K_.setFromTriplets(tripK.begin(), tripK.end());
}

// Replace rows/cols corresponding to Dirichlet nodes and set diag = 1.0
void HeatSolver2D::apply_dirichlet(Eigen::SparseMatrix<double> &mat, Eigen::VectorXd &rhs) {
    if (dirichlet_nodes_.empty()) return;
    int N = static_cast<int>(mat.rows());
    std::set<int> Dnodes(dirichlet_nodes_.begin(), dirichlet_nodes_.end());

    // Rebuild triplets excluding rows/cols in Dnodes
    std::vector<Eigen::Triplet<double>> trips;
    trips.reserve(mat.nonZeros());

    for (int k = 0; k < mat.outerSize(); ++k) {
        for (Eigen::SparseMatrix<double>::InnerIterator it(mat,k); it; ++it) {
            int i = it.row();
            int j = it.col();
            if (Dnodes.count(i) || Dnodes.count(j)) continue;
            trips.emplace_back(i,j,it.value());
        }
    }

    // Add diagonal 1.0 for Dirichlet nodes
    for (size_t idx = 0; idx < dirichlet_nodes_.size(); ++idx) {
        int node = dirichlet_nodes_[idx];
        trips.emplace_back(node, node, 1.0);
    }

    mat.setZero();
    mat.resize(N,N);
    mat.setFromTriplets(trips.begin(), trips.end());

    // Modify rhs
    for (size_t idx = 0; idx < dirichlet_nodes_.size(); ++idx) {
        int node = dirichlet_nodes_[idx];
        double val = dirichlet_values_.empty() ? 0.0 : dirichlet_values_[idx];
        rhs(node) = val;
    }
}

void HeatSolver2D::step_implicit(Grid2D &grid, double dt) {
    int N = mesh_.num_nodes();
    if (N == 0) return;

    // Build system matrix A = M + dt * K
    // If reuse_factorization_ is enabled and dt matches the cached dt, reuse previous factorization.
    Eigen::VectorXd rhs = M_ * grid.state;

    if (!reuse_factorization_ || !factorization_valid_ || dt != cached_dt_) {
        // recreate and factorize
        A_ = M_;
        A_.makeCompressed();
        A_ += (dt * K_);

        // Apply Dirichlet BCs to A and rhs before factorization
        apply_dirichlet(A_, rhs);

        solver_.analyzePattern(A_);
        solver_.factorize(A_);

        // mark cache
        cached_dt_ = dt;
        factorization_valid_ = true;
    } else {
        // reuse existing factorization: still need to update rhs and apply Dirichlet modifications to rhs
        // Note: A_ is already the correct matrix in this case
        apply_dirichlet(A_, rhs);
    }

    Eigen::VectorXd newstate = solver_.solve(rhs);
    grid.state = newstate;
}

} // namespace hpsim
