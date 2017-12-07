#include "renderer.h"
#include <cmath>

float vec4::length()
{
    return sqrt(x * x + y * y + z * z);
}

float vec4::dot_product(const vec4 & rhs) const
{
    return x*rhs.x + y*rhs.y + z*rhs.z;
}

vec4 vec4::cross_product(const vec4 & rhs) const
{
    return vec4(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x, 1.0);
}

void vec4::normalize()
{
    float len = length();
    if (len != 0.0f) {
        float inv = 1.0f / len;
        x *= inv;
        y *= inv;
        z *= inv;
    }
}

vec4 operator+(const vec4 & lhs, const vec4 & rhs)
{
    return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, 1.0);
}

vec4 operator-(const vec4 & lhs, const vec4 & rhs)
{
    return vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, 1.0);
}

float dot_product(const vec4 & lhs, const vec4 & rhs)
{
    return lhs.dot_product(rhs);
}

vec4 cross_product(const vec4 & lhs, const vec4 & rhs)
{
    return lhs.cross_product(rhs);
}

mat44 operator+(const mat44 & lhs, const mat44 & rhs)
{
    mat44 mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            mat.m[i][j] = lhs.m[i][j] + rhs.m[i][j];
    }
    return mat;
}

mat44 operator-(const mat44 & lhs, const mat44 & rhs)
{
    mat44 mat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            mat.m[i][j] = lhs.m[i][j] - rhs.m[i][j];
    }
    return mat;
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

color_t operator*(color_t color, float f)
{
    color *= f;
    return color;
}

color_t operator*(float f, color_t color)
{
    return color * f;
}

mat44 & mat44::operator*=(const float & rhs)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] *= rhs;
    return *this;
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

void mat44::set_translate(float x, float y, float z)
{
    set_identity();
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
}

void mat44::set_scale(float x, float y, float z)
{
    set_identity();
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
}

void mat44::set_rotate(float x, float y, float z, float theta)
{
    float qsin = sin(theta * 0.5f);
    float qcos = cos(theta * 0.5f);
    vec4 vec(x, y, z, 1.0f);
    vec.normalize();
    float w = qcos;
    x = vec.x * qsin;
    y = vec.y * qsin;
    z = vec.z * qsin;
    m[0][0] = 1 - 2 * y * y - 2 * z * z;
    m[0][1] = 2 * x * y - 2 * w * z;
    m[0][2] = 2 * x * z + 2 * w * y;
    m[1][0] = 2 * x * y + 2 * w * z;
    m[1][1] = 1 - 2 * x * x - 2 * z * z;
    m[1][2] = 2 * y * z - 2 * w * x;
    m[2][0] = 2 * x * z - 2 * w * y;
    m[2][1] = 2 * y * z + 2 * w * x;
    m[2][2] = 1 - 2 * x * x - 2 * y * y;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[0][3] = m[1][3] = m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

color_t & color_t::operator*=(const float & f)
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}
