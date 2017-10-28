#include "Painter.h"

void Painter::drawAllShapes(Screen *screen) {
    for (size_t i = 0; i < points.size(); i++) {
        screen->drawPixel(points[i].x, screenHeight - points[i].y, points[i].color);
    }
    for (size_t i = 0; i < lines.size(); i++) {
        screen->drawLine(lines[i].p1.x, screenHeight - lines[i].p1.y, lines[i].p2.x, screenHeight - lines[i].p2.y, lines[i].color);
    }
    for (size_t i = 0; i < circles.size(); i++) {
        screen->drawCircle(circles[i].p.x, screenHeight - circles[i].p.y, circles[i].radius, circles[i].color);
    }
    for (size_t i = 0; i < ellipses.size(); i++) {
        screen->drawEllipse(ellipses[i].p.x, screenHeight - ellipses[i].p.y, ellipses[i].rx, ellipses[i].ry, ellipses[i].color);
    }

    if (inDisplay) {
        if (displayMode == M_Point) {
            screen->drawPixel(displayPoint.x, screenHeight - displayPoint.y, displayPoint.color);
        }
        if (displayMode == M_Line) {
            screen->drawLine(displayLine.p1.x, screenHeight - displayLine.p1.y, displayLine.p2.x, screenHeight - displayLine.p2.y, displayLine.color);
        }
        if (displayMode == M_Circle) {
            screen->drawCircle(displayCircle.p.x, screenHeight - displayCircle.p.y, displayCircle.radius, displayCircle.color);
        }
    }
}

bool Painter::handleEvent(SDL_Event e, Screen *screen) {
    bool quit = false;

    static int startX = 0, startY = 0;

    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
            quit = true;
        }
        else if (e.key.keysym.sym == SDLK_1 || e.key.keysym.sym == SDLK_KP_1) {
            displayMode = M_Point;
            inDisplay = true;
            printf("--- Enter Point Mode ---\n");
            SDL_GetMouseState(&startX, &startY);
        }
        else if (e.key.keysym.sym == SDLK_2 || e.key.keysym.sym == SDLK_KP_2) {
            if (displayMode != M_Line)
                inDisplay = false;
            printf("--- Enter Line Mode ---\n");
            displayMode = M_Line;
        }
        else if (e.key.keysym.sym == SDLK_3 || e.key.keysym.sym == SDLK_KP_3) {
            if (displayMode != M_Circle)
                inDisplay = false;
            printf("--- Enter Circle Mode ---\n");
            displayMode = M_Circle;
        }
        else if (e.key.keysym.sym == SDLK_c) {
            if (displayMode != M_Point)
                inDisplay = false;
            points.clear();
            lines.clear();
            circles.clear();
            ellipses.clear();
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            if (inDisplay == false) {
                inDisplay = true;
            }
            else if (inDisplay == true) {
                if (displayMode == M_Point) {
                    points.push_back(Point(e.button.x, e.button.y));
                    printf("Point:  (%d, %d)\n", e.button.x, e.button.y);
                }
                else if (displayMode == M_Line) {
                    lines.push_back(Line(Point(startX, startY), Point(e.button.x, e.button.y)));
                    printf("Line:   (%d, %d) - (%d, %d)\n", startX, startY, e.button.x, e.button.y);
                }
                else if (displayMode == M_Circle) {
                    int radius = (int)sqrt((e.motion.x - startX) * (e.motion.x - startX) + (e.motion.y - startY) * (e.motion.y - startY));
                    circles.push_back(Circle(Point(startX, startY), radius));
                    printf("Circle: (%d, %d) - %d\n", e.button.x, e.button.y, radius);
                    inDisplay = false;
                }
            }
            //printf("%d %d\n", e.button.x, e.button.y);
            startX = e.button.x;
            startY = e.button.y;

            if (displayMode == M_Point) {
                displayPoint.x = e.motion.x;
                displayPoint.y = e.motion.y;
            }
            else if (displayMode == M_Line) {
                displayLine.p1.x = startX;
                displayLine.p1.y = startY;
                displayLine.p2.x = e.motion.x;
                displayLine.p2.y = e.motion.y;
            }
            else if (displayMode == M_Circle) {
                int r = (int)sqrt((e.motion.x - startX) * (e.motion.x - startX) + (e.motion.y - startY) * (e.motion.y - startY));
                displayCircle.p.x = startX;
                displayCircle.p.y = startY;
                displayCircle.radius = r;
            }
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) {
            if (displayMode != M_Point)
                inDisplay = false;
        }
    }
    else if (e.type == SDL_MOUSEMOTION) {
        if (displayMode == M_Point) {
            displayPoint.x = e.motion.x;
            displayPoint.y = e.motion.y;
        }
        else if (displayMode == M_Line) {
            displayLine.p1.x = startX;
            displayLine.p1.y = startY;
            displayLine.p2.x = e.motion.x;
            displayLine.p2.y = e.motion.y;
        }
        else if (displayMode == M_Circle) {
            int r = (int)sqrt((e.motion.x - startX) * (e.motion.x - startX) + (e.motion.y - startY) * (e.motion.y - startY));
            displayCircle.p.x = startX;
            displayCircle.p.y = startY;
            displayCircle.radius = r;
        }
    }
    return quit;
}