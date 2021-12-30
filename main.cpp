#include <iostream>
#include <string>
#include <array>
#include <random>
#include <bits/stdc++.h>
#include <omp.h>


std::mt19937 rng;


template<typename T, std::size_t rows, std::size_t columns>
using matrix = std::array<std::array<T, columns>, rows>;


int randint(int from, int to) {
    return std::uniform_int_distribution<int>(from, to)(rng);
}


template<std::size_t rows, std::size_t columns>
matrix<int, rows, columns> set_above_main_diagonal \
(const matrix<int, rows, columns> &mat, int num) {
    matrix<int, rows, columns> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = i+1; j < columns; ++j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
matrix<int, rows, columns> set_under_main_diagonal \
(const matrix<int, rows, columns> &mat, const int num) {
    matrix<int, rows, columns> new_mat = mat;
    for (int i = rows-1; i > 0; --i) {
        for (int j = i-1; j >= 0; --j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
matrix<int, rows, columns> set_main_diagonal \
(const matrix<int, rows, columns> &mat, const int num) {
    matrix<int, rows, columns> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            new_mat[i][j] = i == j ? num : new_mat[i][j];
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
void show_mat(const matrix<int, rows, columns> mat) {
    for (std::array<int, columns> row : mat) {
        for (int item : row) {
            std::cout << item << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}


template<std::size_t rows, std::size_t columns>
void fill_mat_(matrix<int, rows, columns> &mat, int n = -1337) {
    if (n == -1337) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                mat[i][j] = randint(0, 9);
            }
        }
    } else {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                mat[i][j] = n;
            }
        }
    }
}


template<std::size_t rows1, std::size_t columns1, \
std::size_t rows2, std::size_t columns2>
matrix<int, rows1, columns2> multiply_matrices \
(matrix<int, rows1, columns1> mat1, matrix<int, rows2, columns2> mat2) {
    matrix<int, rows1, columns2> result_matrix;
# pragma omp parallel shared ( mat1, mat2, result_matrix, \
rows1, columns1, rows2, columns2 ) private ( i, j, k )
    {
        fill_mat_(result_matrix, 0);
# pragma omp for
        for(int i = 0; i < rows1; ++i) {
            for(int j = 0; j < columns2; ++j) {
                for(int k = 0; k < columns1; ++k) {
                    result_matrix[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
    }
    return result_matrix;
}


void assert_test() {
    const matrix<int, 3, 3> square3x3 = \
    {{{1,2,3}, {4,5,6}, {7,8,9}}};
    const matrix<int, 4, 4> square4x4 = \
    {{{1,2,3,4}, {1,2,3,4}, {1,2,3,4}, {1,2,3,4}}};
    const matrix<int, 5, 3> ver_rect = \
    {{{1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}}};
    const matrix<int, 3, 5> hor_rect = \
    {{{1,2,3,4,5}, {1,2,3,4,5}, {1,2,3,4,5}}};
    
    const matrix<int, 3, 3> res_above_square3x3 = \
    {{{1,0,0}, {4,5,0}, {7,8,9}}};
    const matrix<int, 4, 4> res_above_square4x4 = \
    {{{1,0,0,0}, {1,2,0,0}, {1,2,3,0}, {1,2,3,4}}};
    const matrix<int, 5, 3> res_above_ver_rect = \
    {{{1,0,0}, {1,2,0}, {1,2,3}, {1,2,3}, {1,2,3}}};
    const matrix<int, 3, 5> res_above_hor_rect = \
    {{{1,0,0,0,0}, {1,2,0,0,0}, {1,2,3,0,0}}};
    
    const matrix<int, 3, 3> res_main_square3x3 = \
    {{{0,2,3}, {4,0,6}, {7,8,0}}};
    const matrix<int, 4, 4> res_main_square4x4 = \
    {{{0,2,3,4}, {1,0,3,4}, {1,2,0,4}, {1,2,3,0}}};
    const matrix<int, 5, 3> res_main_ver_rect = \
    {{{0,2,3}, {1,0,3}, {1,2,0}, {1,2,3}, {1,2,3}}};
    const matrix<int, 3, 5> res_main_hor_rect = \
    {{{0,2,3,4,5}, {1,0,3,4,5}, {1,2,0,4,5}}};
    
    const matrix<int, 3, 3> res_under_square3x3 = \
    {{{1,2,3}, {0,5,6}, {0,0,9}}};
    const matrix<int, 4, 4> res_under_square4x4 = \
    {{{1,2,3,4}, {0,2,3,4}, {0,0,3,4}, {0,0,0,4}}};
    const matrix<int, 5, 3> res_under_ver_rect = \
    {{{1,2,3}, {0,2,3}, {0,0,3}, {0,0,0}, {0,0,0}}};
    const matrix<int, 3, 5> res_under_hor_rect = \
    {{{1,2,3,4,5}, {0,2,3,4,5}, {0,0,3,4,5}}};
    
    assert(set_above_main_diagonal(square3x3, 0) == res_above_square3x3);
    assert(set_above_main_diagonal(square4x4, 0) == res_above_square4x4);
    assert(set_above_main_diagonal(hor_rect, 0) == res_above_hor_rect);
    assert(set_above_main_diagonal(ver_rect, 0) == res_above_ver_rect);
    
    assert(set_main_diagonal(square3x3, 0) == res_main_square3x3);
    assert(set_main_diagonal(square4x4, 0) == res_main_square4x4);
    assert(set_main_diagonal(hor_rect, 0) == res_main_hor_rect);
    assert(set_main_diagonal(ver_rect, 0) == res_main_ver_rect);
    
    assert(set_under_main_diagonal(square3x3, 0) == res_under_square3x3);
    assert(set_under_main_diagonal(square4x4, 0) == res_under_square4x4);
    assert(set_under_main_diagonal(hor_rect, 0) == res_under_hor_rect);
    assert(set_under_main_diagonal(ver_rect, 0) == res_under_ver_rect);
    
    //TODO add assert for fill_mat_ and multiply_matrices
}


int main(int argc, char *argv[]) {
    assert_test();
    
    matrix<int, 3, 5> mat1;
    matrix<int, 5, 3> mat2;
    
    fill_mat_(mat1, 2);
    fill_mat_(mat2, 4);
    
    std::cout << "mat1:\n";
    show_mat(mat1);
    
    std::cout << "mat2:\n";
    show_mat(mat2);
    
    std::cout << "mat1 * mat2\n";
    show_mat(multiply_matrices(mat1, mat2));
    
    return 0;
}