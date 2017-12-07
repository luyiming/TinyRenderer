#ifndef RENDERER_H
#define RENDERER_H

class vec4 {
public:
    float x = 0.0, y = 0.0, z = 0.0, w = 1.0;

public:
    vec4() {}
    vec4(float x, float y, float z) :x(x), y(y), z(z), w(1.0) {}
    vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
    float length();
    float dot_product(const vec4 & rhs) const; // ʸ�����
    vec4 cross_product(const vec4 & rhs) const; // ʸ�����
    void normalize(); // ʸ����һ��
};

vec4 operator+(const vec4 & lhs, const vec4 & rhs);
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
    void set_translate(float x, float y, float z); // ƽ�Ʊ任����
    void set_scale(float x, float y, float z); // ���ž���
    void set_rotate(float x, float y, float z, float theta); // ��ת����
};

mat44 operator+(const mat44 & lhs, const mat44 & rhs); // ����ӷ�
mat44 operator-(const mat44 & lhs, const mat44 & rhs); // �������
mat44 operator*(const mat44 & lhs, const mat44 & rhs); // ����˷�

vec4 operator*(const mat44 & matrix, const vec4 & vector); // ���������

// 0.0 <= r/g/b <= 1.0
struct color_t { 
    float r, g, b;
    color_t(float r, float g, float b) :r(r), g(g), b(b) {}
    color_t& operator *= (const float &f);
};

color_t operator * (color_t color, float f);
color_t operator * (float f, color_t color);

#endif // RENDERER_H
