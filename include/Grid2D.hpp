#pragma once

#include <vector>
#include <Eigen/Dense>
#include "mesh/Mesh.hpp"

namespace hpsim {

class Grid2D {
public:
    Grid2D() = default;
    Grid2D(const Mesh2D &mesh)
        : mesh_(mesh)
    {
        int N = mesh_.num_nodes();
        state = Eigen::VectorXd::Zero(N);
    }

    void resize_for_mesh(const Mesh2D &mesh) {
        mesh_ = mesh;
        state = Eigen::VectorXd::Zero(mesh_.num_nodes());
    }

    const Mesh2D& mesh() const { return mesh_; }
    Eigen::VectorXd state; // nodal values

private:
    Mesh2D mesh_;
};

} // namespace hpsim
