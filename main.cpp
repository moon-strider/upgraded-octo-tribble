#include <iostream>
#include <string>
#include <array>
#include <experimental/random>


template<std::size_t SIZE>
void set_above_main_diagonal(std::array<std::array<unsigned char, SIZE>, SIZE> &mat, int num) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = i+1; j < SIZE; ++j) {
            mat[i][j] = num;
        }
    }
}

//TODO: return a new matrix instead of modifying the old one
template<std::size_t SIZE>
void set_under_main_diagonal(std::array<std::array<unsigned char, SIZE>, SIZE> &mat, int num) {
    for (int i = SIZE-1; i > 0; --i) {
        for (int j = i-1; j >= 0; --j) {
            mat[i][j] = num;
        }
    }
}


template<std::size_t SIZE>
void set_main_diagonal(std::array<std::array<unsigned char, SIZE>, SIZE> &mat, int num) {
    for (int i = 0; i < mat.size(); ++i) {
        mat[i][i] = num;
    }
}


template<std::size_t SIZE>
void show_mat(const std::array<std::array<unsigned char, SIZE>, SIZE> mat) {
    for (std::array<unsigned char, SIZE> row : mat) {
        for (int item : row) {
            std::cout << item << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    
    const int n = 3;
    
    std::array<std::array<unsigned char, n>, n> mat = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = std::experimental::randint(0, 100);
        }
    }
    
    show_mat(mat);
    set_main_diagonal(mat, 0);
    show_mat(mat);
    set_above_main_diagonal(mat, 1);
    show_mat(mat);
    set_under_main_diagonal(mat, 2);
    show_mat(mat);
    
    return 0;
}