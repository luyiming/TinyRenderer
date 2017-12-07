#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) {
    std::ifstream fin(filename);
    if (fin.fail()) {
        std::cerr << "Fail to load model: " << filename << std::endl;
        return;
    }
    std::string line;
    while (!fin.eof()) {
        std::getline(fin, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec4 v;
            iss >> v.x >> v.y >> v.z;
            v.w = 1.0f;
            vertices.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
    std::cout << "# v# " << vertices.size() << " f# "  << faces.size() << std::endl;
}

Model::~Model() {
}

int Model::num_vertices() {
    return (int)vertices.size();
}

int Model::num_faces() {
    return (int)faces.size();
}

std::vector<int> Model::get_face(int i) {
    return faces[i];
}

vec4 Model::get_vertex(int i) {
    return vertices[i];
}

