#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>

#if 0
template <class T>
class Matrix {
 public:
  Matrix() {}
  Matrix(int width, int height) : width(width), height(height) {
    data.resize(width * height);
    data.fill(0);
  }
  ~Matrix() { data.clear(); }

  inline T get(int x, int y) const { return data[y * width + x]; }

  inline void set(int x, int y, T val) { data[y * width + x] = val; }

 private:
  QVector<T> data;
  int width, height;
};
#else
template <class T>
class Matrix {
 public:
  Matrix() {}
  Matrix(int width, int height) : width(width), height(height) {
    //    data = new T(width * height);
    // I know it's ugly, but the above doesn't seem to work
    data = (T*)malloc(width * height * sizeof(T));
  }
  ~Matrix() {}

  inline T get(int x, int y) const { return data[y * width + x]; }

  inline void set(int x, int y, T val) { data[y * width + x] = val; }

 private:
  T* data = nullptr;
  int width, height;
};
#endif

#endif  // MATRIX_H
