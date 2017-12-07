#ifdef _WIN32
#include <Windows.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>
#include <cmath>
#include "Screen.h"
#include "renderer.h"

using namespace std;

bool Screen::Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Use Legacy OpenGL 2.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Create window
    currentWindow = SDL_CreateWindow(windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (currentWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    openglContext = SDL_GL_CreateContext(currentWindow);
    if (openglContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }
    // Disable Vsync
    // SDL_GL_SetSwapInterval(0);
    // cout << "- Disable Vsync" << endl;


    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error initializing OpenGL! %d\n", error);
        return false;
    }

    return true;
}

void Screen::drawLine(int x1, int y1, int x2, int y2, color_t color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (drawPixel(x1, y1, color), x1 != x2 || y1 != y2) {
        int e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 <  dy) { err += dx; y1 += sy; }
    }
}

void Screen::drawCircle(int x1, int y1, int r, color_t color) {
    int p = 5 - 4 * r;
    int x = 0, y = r;
    while (x <= y) {
        drawPixel(x1 + x, y1 + y, color);
        drawPixel(x1 + x, y1 - y, color);
        drawPixel(x1 - x, y1 - y, color);
        drawPixel(x1 - x, y1 + y, color);
        drawPixel(x1 + y, y1 + x, color);
        drawPixel(x1 + y, y1 - x, color);
        drawPixel(x1 - y, y1 - x, color);
        drawPixel(x1 - y, y1 + x, color);

        if (p < 0) {
            p = p + 8 * x + 12;
        }
        else {
            p = p + 8 * (x + 1) + 12 - 8 * y - 8;
            y--;
        }
        x++;
    }
}

void Screen::drawEllipse(int x1, int y1, int rx, int ry, color_t color) {
    if (rx == ry) {
        drawCircle(x1, y1, rx, color);
        return;
    }

    bool xy_flip = false;
    if (ry > rx) {
        swap(rx, ry);
        xy_flip = true;
    }

    int x = 0, y = ry;
    int rx2 = rx * rx, ry2 = ry * ry;
    int p = 4 * ry * ry - 4 * rx2 * ry + rx2;

    // area 1
    while (ry2 * x < rx2 * y) {
        if (xy_flip) {
            drawPixel(x1 + y, y1 + x, color);
            drawPixel(x1 + y, y1 - x, color);
            drawPixel(x1 - y, y1 - x, color);
            drawPixel(x1 - y, y1 + x, color);
        }
        else {
            drawPixel(x1 + x, y1 + y, color);
            drawPixel(x1 + x, y1 - y, color);
            drawPixel(x1 - x, y1 - y, color);
            drawPixel(x1 - x, y1 + y, color);
        }

        if (p < 0) {
            p = p + ry2 * (8 * x + 12);
        }
        else {
            p = p + ry2 * (8 * x + 12) - rx2 * (8 * y - 8);
            y--;
        }
        x++;
    }

    // area 2
    p = ry2 * (2 * x + 1) * (2 * x + 1) + 4 * rx2 * (y - 1) * (y - 1) - 4 * rx2 * ry2;
    while (y >= 0) {
        if (xy_flip) {
            drawPixel(x1 + y, y1 + x, color);
            drawPixel(x1 + y, y1 - x, color);
            drawPixel(x1 - y, y1 - x, color);
            drawPixel(x1 - y, y1 + x, color);
        }
        else {
            drawPixel(x1 + x, y1 + y, color);
            drawPixel(x1 + x, y1 - y, color);
            drawPixel(x1 - x, y1 - y, color);
            drawPixel(x1 - x, y1 + y, color);
        }

        if (p > 0) {
            p = p - rx * rx * (8 * y - 12);
        }
        else {
            p = p + ry * ry * (8 * x + 8) - rx * rx * (8 * y - 12);
            x++;
        }
        y--;
    }
}

void Screen::drawTriangle(vec4 v0, vec4 v1, vec4 v2, float *zbuffer, color_t color)
{

    // find bounding box
    int x0 = v0.x, y0 = v0.y;
    int x1 = v1.x, y1 = v1.y;
    int x2 = v2.x, y2 = v2.y;
    int bx0 = max(min(min(v0.x, v1.x), v2.x), 0);
    int by0 = max(min(min(v0.y, v1.y), v2.y), 0);
    int bx1 = min(max(max(v0.x, v1.x), v2.x), screenWidth - 1);
    int by1 = min(max(max(v0.y, v1.y), v2.y), screenHeight - 1);
    for (int x = bx0; x < bx1; x++) {
        for (int y = by0; y <= by1; y++) {
            // 重心坐标 barycentric
            // P = (1-u-v)A + uB + vC
            float d00 = (float)(x1 - x0)*(x1 - x0) + (float)(y1 - y0)*(y1 - y0);
            float d01 = (float)(x1 - x0)*(x2 - x0) + (float)(y1 - y0)*(y2 - y0);
            float d11 = (float)(x2 - x0)*(x2 - x0) + (float)(y2 - y0)*(y2 - y0);
            float d20 = (float)(x - x0)*(x1 - x0) + (float)(y - y0)*(y1 - y0);
            float d21 = (float)(x - x0)*(x2 - x0) + (float)(y - y0)*(y2 - y0);
            float denom = d00 * d11 - d01 * d01;

            float inv = (float)(y1 - y2)*(x0 - x2) + (float)(x2 - x1)*(y0 - y2);
            float u = (d11 * d20 - d01 * d21) / denom;
            float v = (d00 * d21 - d01 * d20) / denom;
            float w = 1.0f - u - v;
            if (u < 0 || v < 0 || w < 0)
                continue;
            
            float z = w*v0.z + u*v1.z + v*v2.z;
            int idx = x + y*screenWidth;
            if (zbuffer[idx] < z) {
                zbuffer[idx] = z;
                drawPixel(x, y, color);
            }
        }
    }

    // 扫描线算法
    /*
    // 三个顶点按y轴排序
    if (y0 > y1) {
        swap(x0, x1); swap(y0, y1);
    }
    if (y0 > y2) {
        swap(x0, x2); swap(y0, y2);
    }
    if (y1 > y2) {
        swap(x1, x2); swap(y1, y2);
    }

    if (y0 == y2)
        return;

    float k02 = (float)(x2 - x0) / (y2 - y0);
    if (y0 != y1) {
        float k01 = (float)(x1 - x0) / (y1 - y0);
        // 填充下三角形
        for (int y = y0; y < y1; y++) {
            int xstart = k02*(y - y0) + x0;
            int xstop = k01*(y - y0) + x0;
            if (xstart > xstop) swap(xstart, xstop);
            for (int x = xstart; x <= xstop; x++) {
                drawPixel(x, y, color);
            }
        }
    }
    if (y1 != y2) {
        // 填充上三角形
        float k12 = (float)(x2 - x1) / (y2 - y1);
        for (int y = y1; y <= y2; y++) {
            int xstart = k02*(y - y0) + x0;
            int xstop = k12*(y - y1) + x1;
            if (xstart > xstop) swap(xstart, xstop);
            for (int x = xstart; x <= xstop; x++) {
                drawPixel(x, y, color);
            }
        }
    }
    */
}

void Screen::drawTriangle(vec4 v0, vec4 v1, vec4 v2, color_t color)
{
    int x0 = v0.x, y0 = v0.y;
    int x1 = v1.x, y1 = v1.y;
    int x2 = v2.x, y2 = v2.y;
    if (y0 > y1) {
        swap(x0, x1); swap(y0, y1);
    }
    if (y0 > y2) {
        swap(x0, x2); swap(y0, y2);
    }
    if (y1 > y2) {
        swap(x1, x2); swap(y1, y2);
    }

    if (y0 == y2)
        return;

    float k02 = (float)(x2 - x0) / (y2 - y0);
    if (y0 != y1) {
        float k01 = (float)(x1 - x0) / (y1 - y0);
        // 填充下三角形
        for (int y = y0; y < y1; y++) {
            int xstart = k02*(y - y0) + x0;
            int xstop = k01*(y - y0) + x0;
            if (xstart > xstop) swap(xstart, xstop);
            for (int x = xstart; x <= xstop; x++) {
                drawPixel(x, y, color);
            }
        }
    }
    if (y1 != y2) {
        // 填充上三角形
        float k12 = (float)(x2 - x1) / (y2 - y1);
        for (int y = y1; y <= y2; y++) {
            int xstart = k02*(y - y0) + x0;
            int xstop = k12*(y - y1) + x1;
            if (xstart > xstop) swap(xstart, xstop);
            for (int x = xstart; x <= xstop; x++) {
                drawPixel(x, y, color);
            }
        }
    }
}
