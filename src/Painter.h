#ifndef PAINTER_H
#define PAINTER_H

#include <vector>
#include "Screen.h"
#include <cstdio>

using namespace std;

struct Point {
    int x, y;
    color_t color = COLOR_WHITE;
    Point() {
        x = y = 0;
    }
    Point(int x, int y, color_t color = COLOR_WHITE) {
        this->x = x;
        this->y = y;
        this->color = color;
    }
};

struct Line {
    Point p1, p2;
    color_t color = COLOR_WHITE;
    Line() {
        p1 = p2 = Point();
    }
    Line(Point p1, Point p2, color_t color = COLOR_WHITE) {
        this->p1 = p1;
        this->p2 = p2;;
        this->color = color;
    }
};

struct Circle {
    Point p;
    int radius;
    color_t color = COLOR_WHITE;
    Circle() {
        p = Point();
        radius = 0;
    }
    Circle(Point p, int radius, color_t color = COLOR_WHITE) {
        this->p = p;
        this->radius = radius;
        this->color = color;
    }
};

struct Ellipsed {
    Point p;
    int rx, ry;
    color_t color = COLOR_WHITE;
    Ellipsed() {
        p = Point();
        rx = ry = 0;
    }
    Ellipsed(Point p, int rx, int ry, color_t color = COLOR_WHITE) {
        this->p = p;
        this->rx = rx;
        this->ry = ry;
        this->color = color;
    }
};



class Painter {
private:
    enum DisplayMode { M_None, M_Point, M_Line, M_Circle };
    bool inDisplay = true;
    DisplayMode displayMode = M_Point;
    Point displayPoint;
    Line displayLine;
    Circle displayCircle;

private:
    vector<Point> points;
    vector<Line> lines;
    vector<Circle> circles;
    vector<Ellipsed> ellipses;
    int screenWidth = 640;
    int screenHeight = 480;

public:
    void addPoint(Point point) { points.push_back(point); }
    void addLine(Line line) { lines.push_back(line); }
    void addCircle(Circle circle) { circles.push_back(circle); }
    void addEllipse(Ellipsed ellipse) { ellipses.push_back(ellipse); }

public:
    Painter() {}
    bool handleEvent(SDL_Event e, Screen *screen);
    void drawAllShapes(Screen *screen);
    void init() {
        printf("Keyboard:\n");
        printf("  1: Point Mode\n");
        printf("  2: Line Mode\n");
        printf("  3: Circle Mode\n");
        printf("  c: Clear All\n");
        printf("  q: Quit\n");

        printf("Mouse\n");
        printf("  Left Click:  Start Drawing\n");
        printf("  Right Click: Stop Drawing\n");

        printf("\n--- Enter Point Mode ---\n");
    }
};

#endif // PAINTER_H
