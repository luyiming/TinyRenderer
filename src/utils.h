#ifndef UTILS_H
#define UTILS_H

template<typename T>
void swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

struct color_t {
    unsigned char r, g, b;
    color_t(unsigned char r, unsigned char g, unsigned char b) {
        this->r = r; 
        this->g = g; 
        this->b = b;
    }
};

#define RGB(rgb) (color_t((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff))
#define COLOR_WHITE RGB(0xffffff)
#define COLOR_RED   RGB(0xff0000)
#define COLOR_GREEN RGB(0x00ff00)
#define COLOR_BLUE  RGB(0x0000ff)

#endif // UTILS_H
