#include <iostream>
#include <string>
#include <array>
#include <random>
#include <bits/stdc++.h>


std::mt19937 rng;


int randint(int from, int to){
    return std::uniform_int_distribution<int>(from, to)(rng);
}


template<std::size_t SIZE>
std::array<std::array<int, SIZE>, SIZE> set_above_main_diagonal \
(const std::array<std::array<int, SIZE>, SIZE> &mat, int num) {
    std::array<std::array<int, SIZE>, SIZE> new_mat = mat;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = i+1; j < SIZE; ++j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t SIZE>
std::array<std::array<int, SIZE>, SIZE> set_under_main_diagonal \
(const std::array<std::array<int, SIZE>, SIZE> &mat, const int num) {
    std::array<std::array<int, SIZE>, SIZE> new_mat = mat;
    for (int i = SIZE-1; i > 0; --i) {
        for (int j = i-1; j >= 0; --j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t SIZE>
std::array<std::array<int, SIZE>, SIZE> set_main_diagonal \
(const std::array<std::array<int, SIZE>, SIZE> &mat, const int num) {
    std::array<std::array<int, SIZE>, SIZE> new_mat = mat;
    for (int i = 0; i < mat.size(); ++i) {
        new_mat[i][i] = num;
    }
    return new_mat;
}


template<std::size_t SIZE>
void show_mat(const std::array<std::array<int, SIZE>, SIZE> mat) {
    for (std::array<int, SIZE> row : mat) {
        for (int item : row) {
            std::cout << item << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    
    const int n = 3;
    
    std::array<std::array<int, n>, n> mat = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = randint(0, 100);
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