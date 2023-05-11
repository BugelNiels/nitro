#pragma once

#include <QVector>

#define MIN(A, B) (A) < (B) ? (A) : (B)
#define MAX(A, B) (A) > (B) ? (A) : (B)

namespace nitro {
    template<class T>
    class Matrix {
    public:
        Matrix() : matWidth(0), matHeight(0) {};

//        Matrix(const Matrix<T> &other) : matWidth(other.width()), matHeight(other.height()) {
//            data.resize(matWidth * matHeight);
//            for (int i = 0; i < numElems(); i++) {
//                data[i] = other.data[i];
//            }
//        }

        Matrix(int width, int height) : matWidth(width), matHeight(height) { data.resize(matWidth * matHeight); }

        ~Matrix() = default;

//        Matrix &operator=(const Matrix &) = default;

        inline void fill(T val) { data.fill(val); }

        inline T get(int x, int y) const { return data[y * matWidth + x]; }

        inline T get(int idx) const { return data[idx]; }

        inline void set(int x, int y, T val) { data[y * matWidth + x] = val; }

        inline void set(int idx, T val) { data[idx] = val; }

        inline int width() const { return matWidth; }

        inline int height() const { return matHeight; }

        inline int numElems() const { return width() * height(); }

        inline T *rawData() { return data.data(); }

    private:
        QVector<T> data;
        int matWidth;
        int matHeight;
    };

}  // namespace nitro
