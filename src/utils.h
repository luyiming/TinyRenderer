#ifndef UTILS_H
#define UTILS_H

template<typename T>
void swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

#endif // UTILS_H
