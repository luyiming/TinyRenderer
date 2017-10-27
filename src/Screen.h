#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include "utils.h"

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
    void drawPixel(int x, int y, color_t color = COLOR_WHITE) {
        glBegin(GL_POINTS);
        // glColor3b(, (rgb >> 8) & 0xff, rgb & 0xff);
        glColor3f((float)color.r / 255, (float)color.g / 255, (float)color.b / 255);

        glVertex2f((float)2 * x / screenWidth - 1.0f, (float)2 * y / screenHeight - 1.0f);
        glEnd();
    }
    
    void drawLine(int x1, int y1, int x2, int y2, color_t color = COLOR_WHITE);
    void drawLineDDA(int x1, int y1, int x2, int y2, color_t color = COLOR_WHITE);
    void drawLineBresenham(int x1, int y1, int x2, int y2, color_t color = COLOR_WHITE);
    void drawLineTrival(int x1, int y1, int x2, int y2, color_t color = COLOR_WHITE);

    void drawCircle(int x1, int y1, int r, color_t color = COLOR_WHITE);
    void drawCircleTrival(int x1, int y1, int r, color_t color = COLOR_WHITE);
    void drawCircleBresenham(int x1, int y1, int r, color_t color = COLOR_WHITE);

    // rx: x轴方向长半轴 (rx, 0)
    // ry: y轴方向短半轴 (0, ry)
    void drawEllipse(int x1, int y1, int rx, int ry, color_t color = COLOR_WHITE);

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
