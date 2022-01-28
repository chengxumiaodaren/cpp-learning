#include <array>
#include <iostream>

void test_two_dim_array() {
    int multi_array_native[3][4];
    std::array<std::array<int, 3>, 4> multi_array;
    ///< 但其实它俩不相同，应该是
    std::array<std::array<int, 4>, 3> multi_arrays;
}

template <typename T, size_t R, size_t C>
using TwoDimMatrix = std::array<std::array<T, C>, R>;

void test_two_dim_matrix() {
    constexpr size_t row = 2;
    constexpr size_t col = 3;
    TwoDimMatrix<int, row, col> mat;
    int k = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            mat[i][j] = ++k;
        }
    }

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            std::cout << mat[i][j] << " ";
        }
    }
}

///< 利用模板元编程
template <typename T, size_t R, size_t... C>
struct Matrix {
    using Col = typename Matrix<T, C...>::type;
    using type = std::array<Col, R>;
};

template <typename T, size_t R>
struct Matrix<T, R> {
    using type = std::array<T, R>;
};

void test_multi_array() {
    constexpr size_t a = 2;
    constexpr size_t b = 3;
    constexpr size_t c = 4;
    constexpr size_t d = 5;
    Matrix<int, a, b, c, d>::type mat;
    int u = 0;
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < b; ++j) {
            for (int k = 0; k < c; ++k) {
                for (int m = 0; m < d; ++m) {
                    mat[i][j][k][m] = ++u;
                }
            }
        }
    }
    for (int i = 0; i < a; ++i) {
        for (int j = 0; j < b; ++j) {
            for (int k = 0; k < c; ++k) {
                for (int m = 0; m < d; ++m) {
                    std::cout << mat[i][j][k][m] << " ";
                }
            }
        }
    }
}

int main() {
    test_two_dim_matrix();
    std::cout << "\n";
    test_multi_array();
}
