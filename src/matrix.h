#ifndef __MATRIX_H__
#define __MATRIX_H__

class vec4 {
public:
    float x, y, z, w;

public:
    vec4() :x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    vec4(float x, float y, float z) :x(x), y(y), z(z), w(1.0) {}
    vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
    float length();
    vec4& normalize(); // 矢量归一化
    vec4& homogenize(); // 将齐次坐标归一

    float& operator[](int i);
    vec4& operator+=(const vec4& rhs);
    vec4& operator-=(const vec4& rhs);
    vec4& operator*=(const vec4& rhs); // 逐元素乘法
    vec4& operator*=(const float rhs);
};

vec4 operator+(vec4 lhs, const vec4 & rhs);
vec4 operator-(vec4 lhs, const vec4 & rhs);
vec4 operator*(vec4 lhs, const vec4 & rhs);
vec4 operator*(const float lhs, vec4 rhs);
vec4 operator*(vec4 lhs, const float rhs);
float dot_product(const vec4 & lhs, const vec4 & rhs);
vec4 cross_product(const vec4 & lhs, const vec4 & rhs);


class mat44 {
public:
    float m[4][4];

public:
    static mat44 identity();

public:
    void set_identity();
    void set_zero();


    mat44& operator+=(const mat44& rhs);
    mat44& operator-=(const mat44& rhs);
    mat44& operator*=(const float &rhs);
};

mat44 operator+(mat44 lhs, const mat44 & rhs); // 矩阵加法
mat44 operator-(mat44 lhs, const mat44 & rhs); // 矩阵减法
mat44 operator*(const mat44 & lhs, const mat44 & rhs); // 矩阵乘法
vec4 operator*(const mat44 & matrix, const vec4 & vector); // 矩阵乘向量


mat44 translate(float x, float y, float z);             // 平移变换矩阵
mat44 scale(float x, float y, float z);                 // 缩放矩阵
mat44 rotate(float x, float y, float z, float theta);   // 旋转矩阵
mat44 viewport(int x, int y, int w, int h);
mat44 lookat(vec4 eye, vec4 center, vec4 up);
mat44 ortho(float left, float right, float top, float bottom, float Near, float Far);

#endif // __MATRIX_H__