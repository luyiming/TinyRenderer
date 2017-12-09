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

    void load_texture(const char* filename, TGAImage &texture);

public:
    Model(const char *model_file, const char* diffuse_map);
    ~Model();
    int num_vertices();
    int num_faces();
    std::vector<vertex_t> get_face(int i);
    color_t get_texture(float u, float v);
};

#endif //__MODEL_H__
