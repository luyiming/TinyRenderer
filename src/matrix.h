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
    vec4& normalize(); // ʸ����һ��
    vec4& homogenize(); // ����������һ

    float& operator[](int i);
    vec4& operator+=(const vec4& rhs);
    vec4& operator-=(const vec4& rhs);
    vec4& operator*=(const vec4& rhs); // ��Ԫ�س˷�
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

mat44 operator+(mat44 lhs, const mat44 & rhs); // ����ӷ�
mat44 operator-(mat44 lhs, const mat44 & rhs); // �������
mat44 operator*(const mat44 & lhs, const mat44 & rhs); // ����˷�
vec4 operator*(const mat44 & matrix, const vec4 & vector); // ���������


mat44 translate(float x, float y, float z);             // ƽ�Ʊ任����
mat44 scale(float x, float y, float z);                 // ���ž���
mat44 rotate(float x, float y, float z, float theta);   // ��ת����
mat44 viewport(int x, int y, int w, int h);
mat44 lookat(vec4 eye, vec4 center, vec4 up);
mat44 ortho(float left, float right, float top, float bottom, float Near, float Far);

#endif // __MATRIX_H__