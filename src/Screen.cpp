#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL\GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>
#include <cmath>
#include "Screen.h"
#include "utils.h"

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

    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error initializing OpenGL! %s\n", gluErrorString(error));
        return false;
    }

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error initializing OpenGL! %s\n", gluErrorString(error));
        return false;
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error initializing OpenGL! %s\n", gluErrorString(error));
        return false;
    }

    return true;
}

void Screen::drawLine(int x1, int y1, int x2, int y2, color_t color) {
    drawLineBresenham(x1, y1, x2, y2, color);
    drawLineTrival(x1, y1, x2, y2, color);
}

void Screen::drawLineTrival(int x1, int y1, int x2, int y2, color_t color) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(y1, y2);
        for (; y1 <= y2; y1++)
            drawPixel(x1, y1, color);
        return;
    }

    double k = (double)(y2 - y1) / (x2 - x1);
    double b = y1 - k * x1;

    if (-1 < k && k < 1) {
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        while (x1 <= x2) {
            y1 = (int)floor(k * x1 + b + 0.5);
            drawPixel(x1, y1, color);
            x1++;
        }
    }
    else {
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        while (y1 <= y2) {
            x1 = (int)floor((y1 - b) / k + 0.5);
            drawPixel(x1, y1, color);
            y1++;
        }
    }
}

void Screen::drawLineDDA(int x1, int y1, int x2, int y2, color_t color) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(y1, y2);
        for (; y1 <= y2; y1++)
            drawPixel(x1, y1, color);
        return;
    }

    double m = (double)(y2 - y1) / (x2 - x1);

    if (-1 < m && m < 1) {
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        double x = x1, y = y1, dx = 1.0;
        while (x <= x2) {
            drawPixel((int)x, (int)floor(y + 0.5), color);
            x += dx;
            y += m;
        }
    }
    else {
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        double x = x1, y = y1, dy = 1.0;
        while (y <= y2) {
            drawPixel((int)floor(x + 0.5), (int)y, color);
            y += dy;
            x += 1 / m;
        }
    }
}

void Screen::drawLineBresenham(int x1, int y1, int x2, int y2, color_t color) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(y1, y2);
        for (; y1 <= y2; y1++)
            drawPixel(x1, y1, color);
        return;
    }
    else if (y1 == y2) {
        if (x1 > x2)
            swap(x1, x2);
        for (; x1 <= x2; x1++)
            drawPixel(x1, y1, color);
        return;
    }

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;

    if (dx == dy) {
        for (; x1 <= x2; x1++, y1++)
            drawPixel(x1, y1, color);
        return;
    }
    else if (dx == -dy) {
        for (; x1 <= x2; x1++, y1--)
            drawPixel(x1, y1, color);
        return;
    }

    bool xy_flip = false;

    if (dy > dx || dy < -dx) { // abs(k) > 1
        swap(x1, y1);
        swap(x2, y2);
        xy_flip = true;
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        dx = x2 - x1;
        dy = y2 - y1;
    }

    if (0 < dy && dy < dx) { // abs(k) < 1
        int p = 2 * dy - dx;
        int x = x1, y = y1;
        if (xy_flip)
            drawPixel(y, x, color);
        else
            drawPixel(x, y, color);
        while (x < x2) {
            x++;
            if (p < 0) {
                p = p + 2 * dy;
            }
            else {
                p = p + 2 * dy - 2 * dx;
                y++;
            }
            if (xy_flip)
                drawPixel(y, x, color);
            else
                drawPixel(x, y, color);
        }
    }
    else if (-dx < dy) {
        int p = 2 * dy + dx;
        int x = x1, y = y1;
        if (xy_flip)
            drawPixel(y, x, color);
        else
            drawPixel(x, y, color);
        while (x < x2) {
            x++;
            if (p < 0) {
                p = p + 2 * dy + 2 * dx;
                y--;
            }
            else {
                p = p + 2 * dy;
            }
            if (xy_flip)
                drawPixel(y, x, color);
            else
                drawPixel(x, y, color);
        }
    }
}

void Screen::drawCircle(int x1, int y1, int r, color_t color) {
    //drawCircleTrival(x1, y1, r);
    drawCircleBresenham(x1, y1, r, color);
}

void Screen::drawCircleTrival(int x1, int y1, int r, color_t color) {
    double theta = 0.0;
    int x = r, y = 0;
    while (theta <= 45 * 3.1415926 / 180) {
        drawPixel(x1 + x, y1 + y, color);
        drawPixel(x1 + x, y1 - y, color);
        drawPixel(x1 - x, y1 - y, color);
        drawPixel(x1 - x, y1 + y, color);
        drawPixel(x1 + y, y1 + x, color);
        drawPixel(x1 + y, y1 - x, color);
        drawPixel(x1 - y, y1 - x, color);
        drawPixel(x1 - y, y1 + x, color);
        theta += 1 / (double)r;
        x = (int)floor(r * cos(theta) + 0.5);
        y = (int)floor(r * sin(theta) + 0.5);
    }
}

void Screen::drawCircleBresenham(int x1, int y1, int r, color_t color) {
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
