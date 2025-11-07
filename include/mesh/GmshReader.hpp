#pragma once

#include <string>
#include "mesh/Mesh.hpp"

namespace hpsim {

/// Lightweight Gmsh ASCII (.msh v2) reader. Reads nodes and triangular elements (elem type 2).
/// Very small subset of format implemented — enough for simple example meshes exported from Gmsh.
class GmshReader {
public:
    /// Read a .msh file (ASCII, version 2.x) into mesh. Returns true on success.
    static bool read_msh_v2(const std::string &filename, Mesh2D &mesh, std::string *err = nullptr);
};

} // namespace hpsim
