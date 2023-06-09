#pragma once

#include <QVector>

#define MIN(A, B) (A) < (B) ? (A) : (B)
#define MAX(A, B) (A) > (B) ? (A) : (B)

namespace nitro {
    template<class T>
    class Matrix {
    public:
        Matrix() : matWidth(0), matHeight(0) {};

        Matrix(int width, int height) : matWidth(width), matHeight(height) { data.resize(matWidth * matHeight); }

        ~Matrix() = default;

        inline void fill(T val) { data.fill(val); }

        inline T get(int x, int y) const { return data[y * matWidth + x]; }

        inline T get(int idx) const { return data[idx]; }

        inline void set(int x, int y, T val) { data[y * matWidth + x] = val; }

        inline void set(int idx, T val) { data[idx] = val; }

        [[nodiscard]] inline int width() const { return matWidth; }

        [[nodiscard]] inline int height() const { return matHeight; }

        [[nodiscard]] inline int numElems() const { return width() * height(); }

        inline T *rawData() { return data.data(); }

    private:
        QVector<T> data;
        int matWidth;
        int matHeight;
    };

}  // namespace nitro
