#include <iostream>
#include <string>
#include <array>
#include <random>
#include <bits/stdc++.h>


std::mt19937 rng;


int randint(int from, int to) {
    return std::uniform_int_distribution<int>(from, to)(rng);
}


template<std::size_t columns, std::size_t rows>
std::array<std::array<int, columns>, rows> set_above_main_diagonal \
(const std::array<std::array<int, columns>, rows> &mat, int num) {
    std::array<std::array<int, columns>, rows> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = i+1; j < columns; ++j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t columns, std::size_t rows>
std::array<std::array<int, columns>, rows> set_under_main_diagonal \
(const std::array<std::array<int, columns>, rows> &mat, const int num) {
    std::array<std::array<int, columns>, rows> new_mat = mat;
    for (int i = rows-1; i > 0; --i) {
        for (int j = i-1; j >= 0; --j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t columns, std::size_t rows>
std::array<std::array<int, columns>, rows> set_main_diagonal \
(const std::array<std::array<int, columns>, rows> &mat, const int num) {
    std::array<std::array<int, columns>, rows> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            new_mat[i][j] = i == j ? num : new_mat[i][j];
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
void show_mat(const std::array<std::array<int, columns>, rows> mat) {
    for (std::array<int, columns> row : mat) {
        for (int item : row) {
            std::cout << item << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}


template<std::size_t rows, std::size_t columns>
void fill_mat_(std::array<std::array<int, columns>, rows> &mat) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            mat[i][j] = randint(0, 10);
        }
    }
}


void assert_test() {
    const std::array<std::array<int, 3>, 3> square3x3 = \
    {{{1,2,3}, {4,5,6}, {7,8,9}}};
    const std::array<std::array<int, 4>, 4> square4x4 = \
    {{{1,2,3,4}, {1,2,3,4}, {1,2,3,4}, {1,2,3,4}}};
    const std::array<std::array<int, 3>, 5> ver_rect = \
    {{{1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}}};
    const std::array<std::array<int, 5>, 3> hor_rect = \
    {{{1,2,3,4,5}, {1,2,3,4,5}, {1,2,3,4,5}}};
    
    const std::array<std::array<int, 3>, 3> res_above_square3x3 = \
    {{{1,0,0}, {4,5,0}, {7,8,9}}};
    const std::array<std::array<int, 4>, 4> res_above_square4x4 = \
    {{{1,0,0,0}, {1,2,0,0}, {1,2,3,0}, {1,2,3,4}}};
    const std::array<std::array<int, 3>, 5> res_above_ver_rect = \
    {{{1,0,0}, {1,2,0}, {1,2,3}, {1,2,3}, {1,2,3}}};
    const std::array<std::array<int, 5>, 3> res_above_hor_rect = \
    {{{1,0,0,0,0}, {1,2,0,0,0}, {1,2,3,0,0}}};
    
    const std::array<std::array<int, 3>, 3> res_main_square3x3 = \
    {{{0,2,3}, {4,0,6}, {7,8,0}}};
    const std::array<std::array<int, 4>, 4> res_main_square4x4 = \
    {{{0,2,3,4}, {1,0,3,4}, {1,2,0,4}, {1,2,3,0}}};
    const std::array<std::array<int, 3>, 5> res_main_ver_rect = \
    {{{0,2,3}, {1,0,3}, {1,2,0}, {1,2,3}, {1,2,3}}};
    const std::array<std::array<int, 5>, 3> res_main_hor_rect = \
    {{{0,2,3,4,5}, {1,0,3,4,5}, {1,2,0,4,5}}};
    
    const std::array<std::array<int, 3>, 3> res_under_square3x3 = \
    {{{1,2,3}, {0,5,6}, {0,0,9}}};
    const std::array<std::array<int, 4>, 4> res_under_square4x4 = \
    {{{1,2,3,4}, {0,2,3,4}, {0,0,3,4}, {0,0,0,4}}};
    const std::array<std::array<int, 3>, 5> res_under_ver_rect = \
    {{{1,2,3}, {0,2,3}, {0,0,3}, {0,0,0}, {0,0,0}}};
    const std::array<std::array<int, 5>, 3> res_under_hor_rect = \
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
}


int main(int argc, char *argv[]) {
    //DEF array of arrays to arr(n, m)??????
    assert_test();
    
    const int rows = 5, columns = 3;
    
    std::array<std::array<int, columns>, rows> mat;
    
    fill_mat_(mat);
    
    std::cout << "initial matrix:\n";
    show_mat(mat);
    
    std::cout << "main diagonal changed:\n";
    mat = set_main_diagonal(mat, 0);
    show_mat(mat);
    
    std::cout << "above main diagonal changed:\n";
    mat = set_above_main_diagonal(mat, 1);
    show_mat(mat);
    
    std::cout << "under main diagonal changed:\n";
    mat = set_under_main_diagonal(mat, 2);
    show_mat(mat);
    
    return 0;
}