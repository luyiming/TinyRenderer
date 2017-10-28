#include "Screen.h"
#include "Painter.h"
#include <cstdio>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[]) {
    Screen *screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "TinyRenderer");
    screen->Init();
    Painter painter;

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Enable text input
    SDL_StartTextInput();

    painter.init();

    painter.addPoint(Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

    painter.addLine(Line(Point(10, 120), Point(160, 480)));
    painter.addLine(Line(Point(10, 120), Point(160, 420)));
    painter.addLine(Line(Point(10, 120), Point(160, 360)));
    painter.addLine(Line(Point(10, 120), Point(160, 300)));
    painter.addLine(Line(Point(10, 120), Point(160, 240)));
    painter.addLine(Line(Point(10, 120), Point(160, 180)));
    painter.addLine(Line(Point(10, 120), Point(160, 120)));
    painter.addLine(Line(Point(10, 120), Point(160, 60)));
    painter.addLine(Line(Point(10, 120), Point(160, 0)));
    painter.addLine(Line(Point(10, 360), Point(160, 480)));
    painter.addLine(Line(Point(10, 360), Point(160, 420)));
    painter.addLine(Line(Point(10, 360), Point(160, 360)));
    painter.addLine(Line(Point(10, 360), Point(160, 300)));
    painter.addLine(Line(Point(10, 360), Point(160, 240)));
    painter.addLine(Line(Point(10, 360), Point(160, 180)));
    painter.addLine(Line(Point(10, 360), Point(160, 120)));
    painter.addLine(Line(Point(10, 360), Point(160, 60)));
    painter.addLine(Line(Point(10, 360), Point(160, 0)));
    painter.addLine(Line(Point(10, 120), Point(10, 360), COLOR_GREEN));
    painter.addLine(Line(Point(10, 240), Point(320, 240)));

    painter.addCircle(Circle(Point(320, 240), 100));

    painter.addEllipse(Ellipsed(Point(320, 240), 200, 50, COLOR_BLUE));
    painter.addEllipse(Ellipsed(Point(320, 240), 50, 200));
    painter.addEllipse(Ellipsed(Point(320, 240), 150, 200));

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (painter.handleEvent(e, screen)) {
                quit = true;
            }
        }

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        painter.drawAllShapes(screen);

        screen->render();
    }

    // Disable text input
    SDL_StopTextInput();

    return 0;
}
