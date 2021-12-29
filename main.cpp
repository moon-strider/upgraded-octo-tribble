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
        new_mat[i][i] = num;
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


int main(int argc, char *argv[]) {
    
    const int rows = 4, columns = 3;
    
    std::array<std::array<int, columns>, rows> mat;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            mat[i][j] = randint(0, 10);
        }
    }
    
    show_mat(mat);
    mat = set_main_diagonal(mat, 0);
    show_mat(mat);
    mat = set_above_main_diagonal(mat, 1);
    show_mat(mat);
    mat = set_under_main_diagonal(mat, 2);
    show_mat(mat);
    
    return 0;
}