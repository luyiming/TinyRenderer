#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *model_file, const char* diffuse_map) {
    std::ifstream fin(model_file);
    if (fin.fail()) {
        std::cerr << "Fail to load model: " << model_file << std::endl;
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
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vec4 n;
            iss >> n.x >> n.y >> n.z;
            norm.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            vec4 uv_;
            iss >> uv_.x >> uv_.y;
            uv.push_back(uv_);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<vertex_index_t> f;
            vertex_index_t pair;
            iss >> trash;
            while (iss >> pair.vertex >> trash >> pair.uv >> trash >> pair.norm) {
                pair.vertex--; // in wavefront obj all indices start at 1, not zero
                pair.uv--;
                pair.norm--;
                f.push_back(pair);
            }
            faces.push_back(f);
        }
    }
    std::cout << "# v# " << vertices.size() << " f# " << faces.size() << " vt# " 
        << uv.size() << " vn# " << norm.size() << std::endl;

    load_texture(diffuse_map, diffuse_texture);
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
    std::vector<int> res;
    for (int j = 0; j < (int)faces[i].size(); j++) {
        res.push_back(faces[i][j].vertex);
    }
    return res;
}

void Model::load_texture(const char * filename, TGAImage &texture)
{
    std::cout << "texture file " << filename << " loading " << (texture.read_tga_file(filename) ? "ok" : "failed") << std::endl;
    texture.flip_vertically();
}

color_t Model::get_texture(float u, float v)
{
    TGAColor color = diffuse_texture.get(u * diffuse_texture.get_width(), v * diffuse_texture.get_height());
    return color_t(color.R() / 255.0, color.G() / 255.0, color.B() / 255.0);
}

vec4 Model::get_vertex(int i) {
    return vertices[i];
}

