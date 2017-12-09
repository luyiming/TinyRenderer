#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "renderer.h"
#include "tgaimage.h"

class Model {
private:
    struct vertex_index_t {
        int vertex, uv, norm;
    };
    std::vector<vec4> m_vertices;
    std::vector<std::vector<vertex_index_t> > m_faces;
    std::vector<vec4> m_norm;
    std::vector<texcoord_t> m_uv;
    TGAImage diffuse_texture;

public:
    Model(const char *model_file, const char* diffuse_map);
    ~Model();
    int num_vertices();
    int num_faces();
    vec4 get_vertex(int i);
    std::vector<int> get_face(int i);
    void load_texture(const char* filename, TGAImage &texture);
    color_t get_texture(float u, float v);
    texcoord_t uv(int iface, int nthvert);
    vec4 norm(int iface, int nthvert);
};

#endif //__MODEL_H__
