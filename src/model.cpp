#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
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
            m_vertices.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vec4 n;
            iss >> n.x >> n.y >> n.z;
            m_norm.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            texcoord_t texcoord;
            iss >> texcoord.u >> texcoord.v;
            m_uv.push_back(texcoord);
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
            m_faces.push_back(f);
        }
    }
    std::cout << "# v# " << m_vertices.size() << " f# " << m_faces.size() << " vt# " 
        << m_uv.size() << " vn# " << m_norm.size() << std::endl;

    load_texture(diffuse_map, diffuse_texture);
}

Model::~Model() {
}

int Model::num_vertices() {
    return (int)m_vertices.size();
}

int Model::num_faces() {
    return (int)m_faces.size();
}

std::vector<vertex_t> Model::get_face(int i) {
    std::vector<vertex_t> res;
    vertex_t vertex;
    for (int j = 0; j < (int)m_faces[i].size(); j++) {
        vertex.pos = m_vertices[m_faces[i][j].vertex];
        vertex.norm = m_norm[m_faces[i][j].norm];
        vertex.tex = m_uv[m_faces[i][j].uv];
        vertex.use_tex = true;
        res.push_back(vertex);
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
    TGAColor color = diffuse_texture.get(std::lround(u * diffuse_texture.get_width()), std::lround(v * diffuse_texture.get_height()));
    return color_t(color.R() / 255.0f, color.G() / 255.0f, color.B() / 255.0f);
}

