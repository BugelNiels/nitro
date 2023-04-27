#pragma once

#include <QVector>

#define MIN(A, B) (A) < (B) ? (A) : (B)
#define MAX(A, B) (A) > (B) ? (A) : (B)

namespace nitro {
    template<class T>
    class Matrix {
    public:
        Matrix() = default;

        Matrix(const Matrix<T> &other) {
            matWidth = other.matWidth;
            matHeight = other.matHeight;
            data.resize(matWidth * matHeight);
            for (int i = 0; i < numElems(); i++) {
                data[i] = other.data[i];
            }
        }

        Matrix(int width, int height) : matWidth(width), matHeight(height) { data.resize(matWidth * matHeight); }

        ~Matrix() = default;

        inline T get(int x, int y) const { return data[y * matWidth + x]; }

        inline T get(int idx) const { return data[idx]; }

        inline void set(int x, int y, T val) { data[y * matWidth + x] = val; }

        inline void set(int idx, T val) { data[idx] = val; }

        inline int width() const { return matWidth; }

        inline int height() const { return matHeight; }

        inline int numElems() const { return width() * height(); }

    private:
        QVector<T> data;
        int matWidth{}, matHeight{};
    };

}  // namespace nitro
