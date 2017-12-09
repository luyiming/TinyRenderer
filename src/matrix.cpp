#include "matrix.h"
#include <cmath>
#include <cstdio>
#include <cassert>

float vec4::length()
{
    return std::sqrt(x * x + y * y + z * z);
}

vec4& vec4::normalize()
{
    float len = length();
    if (len != 0.0f) {
        float inv = 1.0f / len;
        x *= inv;
        y *= inv;
        z *= inv;
    }
    return *this;
}

vec4& vec4::homogenize()
{
    if (w != 0) {
        float inv = 1.0f / w;
        x *= inv;
        y *= inv;
        z *= inv;
        w = 1.0f;
    }
    return *this;
}

float & vec4::operator[](int i)
{
    switch (i)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: return w;
    default:
        printf("index out of range %d\n", i);
        assert(0);
        return x;
    }
}

vec4 & vec4::operator+=(const vec4 & rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

vec4 & vec4::operator-=(const vec4 & rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

vec4 & vec4::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
}

vec4 & vec4::operator*=(const vec4 & rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
}

vec4 operator+(vec4 lhs, const vec4 & rhs)
{
    lhs += rhs;
    return lhs;
}

vec4 operator-(vec4 lhs, const vec4 & rhs)
{
    lhs -= rhs;
    return lhs;
}

vec4 operator*(vec4 lhs, const vec4 & rhs)
{
    lhs *= rhs;
    return lhs;
}

vec4 operator*(const float lhs, vec4 rhs)
{
    rhs *= lhs;
    return rhs;
}

vec4 operator*(vec4 lhs, const float rhs)
{
    lhs *= rhs;
    return lhs;
}

float dot_product(const vec4 & lhs, const vec4 & rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

vec4 cross_product(const vec4 & lhs, const vec4 & rhs)
{
    return vec4(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x, 1.0f);
}

mat44 & mat44::operator+=(const mat44 & rhs)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] += rhs.m[i][j];
    return *this;
}

mat44 & mat44::operator-=(const mat44 & rhs)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] -= rhs.m[i][j];
    return *this;
}

mat44 & mat44::operator*=(const float & rhs)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] *= rhs;
    return *this;
}

mat44 operator+(mat44 lhs, const mat44 & rhs)
{
    lhs += rhs;
    return lhs;
}

mat44 operator-(mat44 lhs, const mat44 & rhs)
{
    lhs -= rhs;
    return lhs;
}

mat44 operator*(const mat44 & lhs, const mat44 & rhs)
{
    mat44 mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat.m[j][i] = (lhs.m[j][0] * rhs.m[0][i]) +
                (lhs.m[j][1] * rhs.m[1][i]) +
                (lhs.m[j][2] * rhs.m[2][i]) +
                (lhs.m[j][3] * rhs.m[3][i]);
        }
    }
    return mat;
}

vec4 operator*(const mat44 & mat, const vec4 & vec)
{
    vec4 result;
    result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w;
    result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w;
    result.z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w;
    result.w = mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w;
    return result;
}

mat44 mat44::identity()
{
    mat44 mat;
    mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.0f;
    mat.m[0][1] = mat.m[0][2] = mat.m[0][3] = 0.0f;
    mat.m[1][0] = mat.m[1][2] = mat.m[1][3] = 0.0f;
    mat.m[2][0] = mat.m[2][1] = mat.m[2][3] = 0.0f;
    mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f;
    return mat;
}

void mat44::set_identity()
{
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

void mat44::set_zero()
{
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = m[1][1] = m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0.0f;
}

mat44 translate(float x, float y, float z)
{
    mat44 mat = mat44::identity();
    mat.m[0][3] = x;
    mat.m[1][3] = y;
    mat.m[2][3] = z;
    return mat;
}

mat44 scale(float x, float y, float z)
{
    mat44 mat = mat44::identity();
    mat.m[0][0] = x;
    mat.m[1][1] = y;
    mat.m[2][2] = z;
    return mat;
}

mat44 rotate(float x, float y, float z, float theta)
{
    mat44 mat = mat44::identity();
    float qsin = sin(theta * 0.5f);
    float qcos = cos(theta * 0.5f);
    vec4 vec(x, y, z);
    vec.normalize();
    float w = qcos;
    x = vec.x * qsin;
    y = vec.y * qsin;
    z = vec.z * qsin;
    mat.m[0][0] = 1 - 2 * y * y - 2 * z * z;
    mat.m[0][1] = 2 * x * y - 2 * w * z;
    mat.m[0][2] = 2 * x * z + 2 * w * y;
    mat.m[1][0] = 2 * x * y + 2 * w * z;
    mat.m[1][1] = 1 - 2 * x * x - 2 * z * z;
    mat.m[1][2] = 2 * y * z - 2 * w * x;
    mat.m[2][0] = 2 * x * z - 2 * w * y;
    mat.m[2][1] = 2 * y * z + 2 * w * x;
    mat.m[2][2] = 1 - 2 * x * x - 2 * y * y;
    mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f;
    mat.m[0][3] = mat.m[1][3] = mat.m[2][3] = 0.0f;
    mat.m[3][3] = 1.0f;
    return mat;
}

mat44 viewport(int x, int y, int w, int h)
{
    mat44 m;
    m.set_identity();
    m.m[0][3] = x + w / 2.f;
    m.m[1][3] = y + h / 2.f;
    m.m[2][3] = 255.0f / 2.f;

    m.m[0][0] = w / 2.f;
    m.m[1][1] = h / 2.f;
    m.m[2][2] = 255.0f / 2.f;
    return m;
}

mat44 lookat(vec4 eye, vec4 center, vec4 up) {
    vec4 z = eye - center;
    z.normalize();
    vec4 x = cross_product(up, z);
    x.normalize();
    vec4 y = cross_product(z, x);
    y.normalize();
    mat44 Minv, Tr;
    Minv.set_identity();
    Tr.set_identity();
    for (int i = 0; i < 3; i++) {
        Minv.m[0][i] = x[i];
        Minv.m[1][i] = y[i];
        Minv.m[2][i] = z[i];
        Tr.m[i][3] = -center[i];
    }
    return Minv * Tr;
}

mat44 ortho(float left, float right, float top, float bottom, float Near, float Far)
{
    mat44 mat;
    mat.set_identity();
    mat.m[0][0] = 2.0f / (right - left);
    mat.m[1][1] = 2.0f / (top - bottom);
    mat.m[2][2] = -2.0f / (Far - Near); // we use right hand coordinate system, so Far < Near
    mat.m[0][3] = -(right + left) / (right - left);
    mat.m[1][3] = -(top + bottom) / (top - bottom);
    mat.m[2][3] = -(Far + Near) / (Far - Near);

    return mat;
}
