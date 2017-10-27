#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

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
    void drawPixel(int x, int y, int rgb = 0xffffff) {
        glBegin(GL_POINTS);
        // glColor3b(, (rgb >> 8) & 0xff, rgb & 0xff);
        glColor3f(((rgb >> 16) & 0xff) / (double)255, ((rgb >> 8) & 0xff) / (double)255, (rgb >> 16 & 0xff) / (double)255);
        glVertex2f((float)2 * x / screenWidth - 1.0f, (float)2 * y / screenHeight - 1.0f);
        glEnd();
    }
    
    void drawLine(int x1, int y1, int x2, int y2);
    void drawLineDDA(int x1, int y1, int x2, int y2);
    void drawLineTrival(int x1, int y1, int x2, int y2);

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
