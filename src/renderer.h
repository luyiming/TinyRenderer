#ifndef RENDERER_H
#define RENDERER_H

#include "matrix.h"
#include <vector>

// 0.0 <= r/g/b <= 1.0
struct color_t {
    float r, g, b;
    color_t() :r(1.0f), g(1.0f), b(1.0f) {}
    color_t(float r, float g, float b) :r(r), g(g), b(b) {}
    color_t& operator *= (const float &f);
};

color_t operator * (color_t color, float f);
color_t operator * (float f, color_t color);

struct texcoord_t { 
    float u, v; 
};

struct vertex_t { 
    vec4 pos;
    bool use_tex;
    texcoord_t tex;
    color_t color;
    vec4 norm; 
};

struct device_t {
    int width, height;
    float *zbuffer;
};

struct IShader {
    virtual ~IShader() {}
    virtual bool fragment(vec4 bar, color_t &color) = 0;
};

typedef void (*draw_pixel_fn_type)(int x, int y, color_t color);
extern draw_pixel_fn_type draw_pixel_fn;

// 计算点P在三角形ABC中的重心坐标, 返回值(u, v, w)，可以表示为P=uA+vB+wC
vec4 barycentric(vec4 A, vec4 B, vec4 C, vec4 P);

void draw_triangle(vec4 v0, vec4 v1, vec4 v2, IShader &shader, device_t &device);

bool clip_vertices(vec4 &v);

#endif // RENDERER_H
