#include <iostream>
#include "mesh/GmshReader.hpp"
#include "Grid2D.hpp"
#include "solvers/HeatSolver2D.hpp"
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: heat2d_example mesh.msh [dt] [steps]\n";
        return 1;
    }

    std::string meshfile = argv[1];
    double dt = 1e-3;
    int steps = 100;
    bool reuse_factorization = false;
    // usage: heat2d_example mesh.msh [dt] [steps] [--reuse-factorization]
    if (argc >= 3) dt = std::stod(argv[2]);
    if (argc >= 4) steps = std::stoi(argv[3]);
    for (int i = 4; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--reuse-factorization") reuse_factorization = true;
    }

    hpsim::Mesh2D mesh;
    std::string err;
    if (!hpsim::GmshReader::read_msh_v2(meshfile, mesh, &err)) {
        std::cerr << "Failed to read mesh: " << err << std::endl;
        return 2;
    }

    hpsim::Grid2D grid(mesh);

    // Initialize a hot spot in center
    double xmin, xmax, ymin, ymax;
    mesh.bounding_box(xmin,xmax,ymin,ymax);
    double cx = 0.5*(xmin + xmax);
    double cy = 0.5*(ymin + ymax);

    for (int i = 0; i < mesh.num_nodes(); ++i) {
        double dx = mesh.nodes[i].x - cx;
        double dy = mesh.nodes[i].y - cy;
        double r2 = dx*dx + dy*dy;
        grid.state(i) = std::exp(-50.0 * r2);
    }

    hpsim::HeatSolver2D solver;
    solver.initialize(mesh, /*diffusion*/ 0.1);
    solver.assemble_matrices();
    solver.set_reuse_factorization(reuse_factorization);

    // Dirichlet: nodes on bounding box edges -> 0
    std::vector<int> dbc_nodes;
    std::vector<double> dbc_vals;
    for (int i = 0; i < mesh.num_nodes(); ++i) {
        if (mesh.nodes[i].x == xmin || mesh.nodes[i].x == xmax || mesh.nodes[i].y == ymin || mesh.nodes[i].y == ymax) {
            dbc_nodes.push_back(i);
            dbc_vals.push_back(0.0);
        }
    }
    solver.set_dirichlet(dbc_nodes, dbc_vals);

    for (int step = 0; step < steps; ++step) {
        solver.step_implicit(grid, dt);
        if (step % 10 == 0) std::cout << "Step " << step << "\n";
    }

    // Write results to CSV
    std::string out = "heat2d_result.csv";
    std::ofstream f(out);
    f << "x,y,u\n";
    for (int i = 0; i < mesh.num_nodes(); ++i) {
        f << mesh.nodes[i].x << "," << mesh.nodes[i].y << "," << grid.state(i) << "\n";
    }
    f.close();
    std::cout << "Wrote results to " << out << "\n";

    return 0;
}
