#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <iostream>
#include "renderer.h"

using namespace std;

class Screen {
public:
    Screen(int width, int height, const char* title) {
        screenWidth = width;
        screenHeight = height;
        windowTitle = title;
    }

    ~Screen() {
        SDL_DestroyWindow(currentWindow);
        SDL_Quit();
    }

    bool Init();

    // (0, 0) is bottom-left corner, (screenWidth, screenHeight) is top-right corner
    void drawPixel(int x, int y, color_t color = { 1.0f, 1.0f, 1.0f }) {
        if (x < 0 || x > screenWidth || y < 0 || y > screenHeight) {
            std::cerr << "pixel index out of range: " << x << ", " << y << std::endl;
            return;
        }
        glBegin(GL_POINTS);
        // glColor3b(, (rgb >> 8) & 0xff, rgb & 0xff);
        glColor3f(color.r, color.g, color.b);

        glVertex2f((float)2 * x / screenWidth - 1.0f, (float)2 * y / screenHeight - 1.0f);
        glEnd();
    }

    void drawLine(int x1, int y1, int x2, int y2, color_t color = { 1.0f, 1.0f, 1.0f });
    void drawCircle(int x1, int y1, int r, color_t color = { 1.0f, 1.0f, 1.0f });
    void drawEllipse(int x1, int y1, int rx, int ry, color_t color = { 1.0f, 1.0f, 1.0f });

    // 计算点P在三角形ABC中的重心坐标, 返回值(u, v, w)，可以表示为P=uA+vB+wC
    vec4 barycentric(vec4 A, vec4 B, vec4 C, vec4 P);
    void drawTriangle(vec4 v0, vec4 v1, vec4 v2, IShader &shader, float *zbuffer);
    void drawTriangle(vec4 v0, vec4 v1, vec4 v2, color_t color = { 1.0f, 1.0f, 1.0f });
    vec4 world2screen(vec4 v);

    void render() { SDL_GL_SwapWindow(currentWindow); }

private:
    int screenWidth;
    int screenHeight;
    const char* windowTitle;

public:
    SDL_Window *currentWindow;
    SDL_GLContext openglContext;
};

#endif // SCREEN_H
