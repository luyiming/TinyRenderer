#ifndef RENDERER_H
#define RENDERER_H

class vec4 {
public:
    float x, y, z, w;

public:
    vec4() :x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    vec4(float x, float y, float z) :x(x), y(y), z(z), w(1.0) {}
    vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
    float length();
    float dot_product(const vec4 & rhs) const; // 矢量点乘
    vec4 cross_product(const vec4 & rhs) const; // 矢量叉乘
    void normalize(); // 矢量归一化
    void homogenize(); // 将齐次坐标归一

    vec4& operator*=(const vec4& rhs); // 逐元素乘法
};

vec4 operator+(const vec4 & lhs, const vec4 & rhs);
vec4 operator*(vec4 lhs, const vec4 & rhs);
vec4 operator-(const vec4 & lhs, const vec4 & rhs);
float dot_product(const vec4 & lhs, const vec4 & rhs);
vec4 cross_product(const vec4 & lhs, const vec4 & rhs);

class mat44 {
public:
    float m[4][4];

public:
    mat44& operator *= (const float &rhs);
    void set_identity();
    void set_zero();
    void set_translate(float x, float y, float z); // 平移变换矩阵
    void set_scale(float x, float y, float z); // 缩放矩阵
    void set_rotate(float x, float y, float z, float theta); // 旋转矩阵
};

mat44 operator+(const mat44 & lhs, const mat44 & rhs); // 矩阵加法
mat44 operator-(const mat44 & lhs, const mat44 & rhs); // 矩阵减法
mat44 operator*(const mat44 & lhs, const mat44 & rhs); // 矩阵乘法

vec4 operator*(const mat44 & matrix, const vec4 & vector); // 矩阵乘向量

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
    texcoord_t tex; 
    color_t color; 
    vec4 norm; 
};

struct IShader {
    virtual ~IShader() {}
    virtual vec4 vertex(int iface, int nthvert) = 0;
    virtual bool fragment(vec4 bar, color_t &color) = 0;
};

#endif // RENDERER_H
