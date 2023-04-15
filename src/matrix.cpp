#include "matrix.h"

template <class T>
Matrix<T>::Matrix() {}

template <class T>
Matrix<T>::Matrix(int width, int height) : width(width), height(height) {
  data.resize(width * height);
  data.fill(0);
}

template <class T>
Matrix<T>::~Matrix() {
  data.clear();
}

template <class T>
T Matrix<T>::get(int x, int y) const {
  return data[y * width + x];
}

template <class T>
void Matrix<T>::set(int x, int y, T val) {
  data[y * width + x] = val;
}
