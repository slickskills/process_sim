#include "mesh/GmshReader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

namespace hpsim {

bool GmshReader::read_msh_v2(const std::string &filename, Mesh2D &mesh, std::string *err) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        if (err) *err = "Could not open file: " + filename;
        return false;
    }

    std::string line;
    enum class State { None, Nodes, Elements } state = State::None;

    // Gmsh node id -> local index
    std::unordered_map<int,int> node_map;

    while (std::getline(in, line)) {
        if (line == "$Nodes") {
            state = State::Nodes;
            // read number of nodes
            if (!std::getline(in, line)) break;
            int n = std::stoi(line);
            mesh.nodes.reserve(n);
            for (int i = 0; i < n; ++i) {
                if (!std::getline(in, line)) break;
                std::istringstream iss(line);
                int id; double x,y,z;
                iss >> id >> x >> y >> z;
                Node2D nd;
                nd.id = id;
                nd.x = x;
                nd.y = y;
                node_map[id] = static_cast<int>(mesh.nodes.size());
                mesh.nodes.push_back(nd);
            }
            // consume $EndNodes
            while (std::getline(in, line)) {
                if (line == "$EndNodes") break;
            }
            state = State::None;
        }
        else if (line == "$Elements") {
            state = State::Elements;
            if (!std::getline(in, line)) break;
            int ne = std::stoi(line);
            mesh.elems.reserve(ne);
            for (int i = 0; i < ne; ++i) {
                if (!std::getline(in, line)) break;
                std::istringstream iss(line);
                int id, type, numTags;
                iss >> id >> type >> numTags;
                std::vector<int> tags(numTags);
                for (int t = 0; t < numTags; ++t) iss >> tags[t];

                if (type == 2) { // 3-node triangle
                    int n1, n2, n3;
                    iss >> n1 >> n2 >> n3;
                    // map gmsh ids to local indices
                    if (node_map.find(n1) == node_map.end() || node_map.find(n2) == node_map.end() || node_map.find(n3) == node_map.end()) {
                        if (err) *err = "Element references unknown node id";
                        return false;
                    }
                    TriElement el;
                    el.id = id;
                    el.nodes[0] = node_map[n1];
                    el.nodes[1] = node_map[n2];
                    el.nodes[2] = node_map[n3];
                    if (!tags.empty()) el.physical_tag = tags[0];
                    mesh.elems.push_back(el);
                } else {
                    // skip other element types for now (lines, quads, etc.)
                    // consume remaining ints on the line if any
                    // nothing to do
                }
            }
            while (std::getline(in, line)) {
                if (line == "$EndElements") break;
            }
            state = State::None;
        }
    }

    return true;
}

} // namespace hpsim
