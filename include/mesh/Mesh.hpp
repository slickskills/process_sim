#pragma once

#include <vector>
#include <array>
#include <string>

namespace hpsim {

struct Node2D {
    int id;    // original id from mesh file
    double x;
    double y;
};

struct TriElement {
    int id; // original id
    std::array<int,3> nodes; // indices into Mesh2D::nodes (0-based)
    int physical_tag = 0;
};

class Mesh2D {
public:
    Mesh2D() = default;

    std::vector<Node2D> nodes;     // indexed 0..N-1
    std::vector<TriElement> elems; // triangular elements

    int num_nodes() const { return static_cast<int>(nodes.size()); }
    int num_elems() const { return static_cast<int>(elems.size()); }

    // Simple bounding box query
    void bounding_box(double &xmin, double &xmax, double &ymin, double &ymax) const {
        if (nodes.empty()) { xmin = xmax = ymin = ymax = 0.0; return; }
        xmin = xmax = nodes[0].x;
        ymin = ymax = nodes[0].y;
        for (const auto &n : nodes) {
            if (n.x < xmin) xmin = n.x;
            if (n.x > xmax) xmax = n.x;
            if (n.y < ymin) ymin = n.y;
            if (n.y > ymax) ymax = n.y;
        }
    }
};

} // namespace hpsim
