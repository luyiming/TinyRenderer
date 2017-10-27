#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL\GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
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

void Screen::drawLine(int x1, int y1, int x2, int y2) {
    drawLineTrival(x1, y1, x2, y2);
    drawLineDDA(x1, y1, x2, y2);
}

void Screen::drawLineTrival(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(y1, y2);
        for (; y1 <= y2; y1++)
            drawPixel(x1, y1);
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
            y1 = floor(k * x1 + b + 0.5);
            drawPixel(x1, y1);
            x1++;
        }
    }
    else {
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        while (y1 <= y2) {
            x1 = floor((y1 - b) / k + 0.5);
            drawPixel(x1, y1);
            y1++;
        }
    }
}

void Screen::drawLineDDA(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(y1, y2);
        for (; y1 <= y2; y1++)
            drawPixel(x1, y1, 0xff0000);
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
            drawPixel(x, floor(y + 0.5), 0xff0000);
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
            drawPixel(floor(x + 0.5), y, 0xff0000);
            y += dy;
            x += 1 / m;
        }
    }
}
