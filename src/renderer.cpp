#include "renderer.h"
#include <cmath>
#include <cstdio>
#include <cassert>
#include <algorithm>

draw_pixel_fn_type draw_pixel_fn = NULL;

color_t operator*(color_t color, float f)
{
    color *= f;
    return color;
}

color_t operator*(float f, color_t color)
{
    return color * f;
}

color_t operator+(color_t lhs, const color_t & rhs)
{
    lhs += rhs;
    return lhs;
}

color_t & color_t::operator*=(const float & f)
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

color_t & color_t::operator+=(const color_t & rhs)
{
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
}

vec4 barycentric(vec4 A, vec4 B, vec4 C, vec4 P)
{
    float d00 = (B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y);
    float d01 = (B.x - A.x)*(C.x - A.x) + (B.y - A.y)*(C.y - A.y);
    float d11 = (C.x - A.x)*(C.x - A.x) + (C.y - A.y)*(C.y - A.y);
    float d20 = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);
    float d21 = (P.x - A.x)*(C.x - A.x) + (P.y - A.y)*(C.y - A.y);
    float denom = d00 * d11 - d01 * d01;

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return vec4(u, v, w);
}

void draw_triangle(vec4 v0, vec4 v1, vec4 v2, IShader &shader, device_t &device)
{
    // find bounding box
    float x0 = v0.x, y0 = v0.y;
    float x1 = v1.x, y1 = v1.y;
    float x2 = v2.x, y2 = v2.y;
    int bx0 = std::lround(std::max(std::min({ v0.x, v1.x, v2.x }), 0.0f));
    int by0 = std::lround(std::max(std::min({ v0.y, v1.y, v2.y }), 0.0f));
    int bx1 = std::lround(std::min(std::max({ v0.x, v1.x, v2.x }), (float)device.width - 1.0f));
    int by1 = std::lround(std::min(std::max({ v0.y, v1.y, v2.y }), (float)device.height - 1.0f));
    for (int x = bx0; x <= bx1; x++) {
        for (int y = by0; y <= by1; y++) {

            vec4 P((float)x, (float)y, 0);

            vec4 bar = barycentric(v0, v1, v2, P);
            if (bar.x < 0 || bar.y < 0 || bar.z < 0) // out of triangle
                continue;

            color_t color;
            bool discard = shader.fragment(bar, color);
            if (discard)
                continue;

            P.z = bar.x * v0.z + bar.y * v1.z + bar.z * v2.z;
            int idx = x + y*device.width;
            if (device.zbuffer[idx] < P.z) {
                device.zbuffer[idx] = P.z;
                draw_pixel_fn(x, y, color);
            }
        }
    }
}

bool clip_vertices(vec4 &v)
{
    if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1 || v.z < -1 || v.z > 1)
        return true;
    return false;
}
