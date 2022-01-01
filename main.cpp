#include <iostream>
#include <string>
#include <array>
#include <random>
#include <bits/stdc++.h>
#include <omp.h>
#include <stdexcept>


std::mt19937 rng;


template<typename T, std::size_t rows, std::size_t columns>
using matrix = std::array<std::array<T, columns>, rows>;


int randint(int from, int to) {
    return std::uniform_int_distribution<int>(from, to)(rng);
}


double randdouble(double from, double to) {
    return std::uniform_real_distribution<double>(from, to)(rng);
}


//inline void transpose4x4_SSE(float *A, float *B, const int lda, const int ldb) {
//__m128 row1 = _mm_load_ps(&A[0*lda]);
//__m128 row2 = _mm_load_ps(&A[1*lda]);
//__m128 row3 = _mm_load_ps(&A[2*lda]);
//__m128 row4 = _mm_load_ps(&A[3*lda]);
//_MM_TRANSPOSE4_PS(row1, row2, row3, row4);
//_mm_store_ps(&B[0*ldb], row1);
//_mm_store_ps(&B[1*ldb], row2);
//_mm_store_ps(&B[2*ldb], row3);
//_mm_store_ps(&B[3*ldb], row4);
//}

inline void transpose_block_SSE4x4(float *A, float *B, const int n, const int m, const int lda, const int ldb ,const int block_size) {
#pragma omp parallel for
    for(int i=0; i<n; i+=block_size) {
        for(int j=0; j<m; j+=block_size) {
            int max_i2 = i+block_size < n ? i + block_size : n;
            int max_j2 = j+block_size < m ? j + block_size : m;
            for(int i2=i; i2<max_i2; i2+=4) {
                for(int j2=j; j2<max_j2; j2+=4) {
                    //transpose4x4_SSE(&A[i2*lda +j2], &B[j2*ldb + i2], lda, ldb);
                }
            }
        }
    }
}


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> add_matrices \
(matrix<double, rows, columns> mat1, matrix<double, rows, columns> mat2) {
    matrix<double, rows, columns> result_matrix;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            result_matrix[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
    return result_matrix;
}


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> set_above_main_diagonal \
(const matrix<double, rows, columns> &mat, int num) {
    matrix<double, rows, columns> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = i+1; j < columns; ++j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> set_under_main_diagonal \
(const matrix<double, rows, columns> &mat, const int num) {
    matrix<double, rows, columns> new_mat = mat;
    for (int i = rows-1; i > 0; --i) {
        for (int j = i-1; j >= 0; --j) {
            new_mat[i][j] = num;
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> set_main_diagonal \
(const matrix<double, rows, columns> &mat, const int num) {
    matrix<double, rows, columns> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            new_mat[i][j] = i == j ? num : new_mat[i][j];
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> set_aux_diagonal \
(const matrix<double, rows, columns> &mat, const double num) {
    matrix<double, rows, columns> new_mat = mat;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            new_mat[i][j] = i == columns-1-j ? num : new_mat[i][j];
        }
    }
    return new_mat;
}


template<std::size_t rows, std::size_t columns>
void show_mat(const matrix<double, rows, columns> mat) {
    for (std::array<double, columns> row : mat) {
        for (double item : row) {
            std::cout << item << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}


template<std::size_t rows, std::size_t columns>
void fill_mat_(matrix<double, rows, columns> &mat, double n = -1337.) {
    if (n == -1337.) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                mat[i][j] = randdouble(0, 9);
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


template<std::size_t rows, std::size_t columns>
void fill_identity_mat_(matrix<double, rows, columns> &mat) {
    if (rows != columns) {
        throw std::invalid_argument \
        ("Row number must equal col number for identity matrices");
    }
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            mat[i][j] = i == j ? 1 : 0;
        }
    }
}


template<std::size_t rows1, std::size_t columns1, \
std::size_t rows2, std::size_t columns2>
matrix<double, rows1, columns2> multiply_matrices \
(matrix<double, rows1, columns1> mat1, matrix<double, rows2, columns2> mat2) {
    assert(rows2 == columns1);
    matrix<double, rows1, columns2> result_matrix;
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


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> multiply_matrix \
(matrix<double, rows, columns> mat, double num) {
    matrix<double, rows, columns> result_matrix;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            result_matrix[i][j] = mat[i][j]*num;
        }
    }
    
    return result_matrix;
}


template<std::size_t rows, std::size_t columns>
matrix<double, rows, columns> matrix_pow \
(matrix<double, rows, columns> mat, int num) {
    if (num < 1) {
        throw std::invalid_argument("A power cannot be less than 1");
    }
    --(--num);
    matrix<double, rows, columns> result_matrix = \
        multiply_matrices(mat, mat);
    
    for (int i = 0; i < num; ++i) {
        show_mat(result_matrix);
        result_matrix = multiply_matrices(result_matrix, mat);
    }
    return result_matrix;
}


template<std::size_t rows, std::size_t columns>
int matrix_det(matrix<double, rows, columns> mat) {
    if (rows != columns) {
        throw std::invalid_argument("Only a square matrices have a det");
    }
    
    double tmp, det;
    
    matrix<double, rows, columns> tmp_mat = mat;
    for (int k = 0; k < rows - 1; k++) {
        for (int i = k + 1; i < rows; i++) {
            tmp = -tmp_mat[i][k] / tmp_mat[k][k];
            for (int j = 0; j < rows; j++) {
                tmp_mat[i][j] += tmp_mat[k][j] * tmp;
            }
        }
    }
    
    det = 1;
    for (int i = 0; i < rows; ++i) {
        det *= tmp_mat[i][i];
    }
    
    return det;
}


void assert_test() {
    const matrix<double, 3, 3> square3x3 = \
    {{{1,2,3}, {4,5,6}, {7,8,9}}};
    const matrix<double, 4, 4> square4x4 = \
    {{{1,2,3,4}, {1,2,3,4}, {1,2,3,4}, {1,2,3,4}}};
    const matrix<double, 5, 3> ver_rect = \
    {{{1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}}};
    const matrix<double, 3, 5> hor_rect = \
    {{{1,2,3,4,5}, {1,2,3,4,5}, {1,2,3,4,5}}};
    
    const matrix<double, 3, 5> mult1 = \
    {{{2, 2, 2, 2, 2}, {2, 2, 2, 2, 2}, {2, 2, 2, 2, 2}}};
    const matrix<double, 5, 3> mult2 = \
    {{{4, 4, 4}, {4, 4, 4}, {4, 4, 4}, {4, 4, 4}, {4, 4, 4}}};
    
    const matrix<double, 3, 3> sum = \
    {{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}};
    
    matrix<double, 3, 3> filler_mat = \
    {{{2, 2, 2}, {2, 2, 2}, {2, 2, 2}}};
    
    const matrix<double, 3, 3> res_above_square3x3 = \
    {{{1,0,0}, {4,5,0}, {7,8,9}}};
    const matrix<double, 4, 4> res_above_square4x4 = \
    {{{1,0,0,0}, {1,2,0,0}, {1,2,3,0}, {1,2,3,4}}};
    const matrix<double, 5, 3> res_above_ver_rect = \
    {{{1,0,0}, {1,2,0}, {1,2,3}, {1,2,3}, {1,2,3}}};
    const matrix<double, 3, 5> res_above_hor_rect = \
    {{{1,0,0,0,0}, {1,2,0,0,0}, {1,2,3,0,0}}};
    
    const matrix<double, 3, 3> res_main_square3x3 = \
    {{{0,2,3}, {4,0,6}, {7,8,0}}};
    const matrix<double, 4, 4> res_main_square4x4 = \
    {{{0,2,3,4}, {1,0,3,4}, {1,2,0,4}, {1,2,3,0}}};
    const matrix<double, 5, 3> res_main_ver_rect = \
    {{{0,2,3}, {1,0,3}, {1,2,0}, {1,2,3}, {1,2,3}}};
    const matrix<double, 3, 5> res_main_hor_rect = \
    {{{0,2,3,4,5}, {1,0,3,4,5}, {1,2,0,4,5}}};
    
    const matrix<double, 3, 3> res_under_square3x3 = \
    {{{1,2,3}, {0,5,6}, {0,0,9}}};
    const matrix<double, 4, 4> res_under_square4x4 = \
    {{{1,2,3,4}, {0,2,3,4}, {0,0,3,4}, {0,0,0,4}}};
    const matrix<double, 5, 3> res_under_ver_rect = \
    {{{1,2,3}, {0,2,3}, {0,0,3}, {0,0,0}, {0,0,0}}};
    const matrix<double, 3, 5> res_under_hor_rect = \
    {{{1,2,3,4,5}, {0,2,3,4,5}, {0,0,3,4,5}}};
    
    const matrix<double, 3, 3> res_mult = \
    {{{40, 40, 40}, {40, 40, 40,}, {40, 40, 40}}};
    
    const matrix<double, 3, 3> res_mult_num = \
    {{{3, 3, 3}, {3, 3, 3}, {3, 3, 3}}};
    
    const matrix<double, 3, 3> res_fill = \
    {{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}};
    
    const matrix<double, 3, 3> res_sum = \
    {{{2, 2, 2}, {2, 2, 2}, {2, 2, 2}}};
    
    const matrix<double, 3, 3> res_pow = \
    {{{468, 576, 684}, {1062, 1305, 1548}, {1656, 2034, 2412}}};
    
    const matrix<double, 5, 3> res_transpose = \
    {{{1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}, {5, 5, 5}}};
    
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
    
    assert(multiply_matrix(sum, 3) == res_mult_num);
    
    assert(multiply_matrices(mult1, mult2) == res_mult);
    
    assert(add_matrices(sum, sum) == res_sum);
    
    assert(matrix_pow(square3x3, 3) == res_pow);
    
    fill_mat_(filler_mat, 1);
    assert(filler_mat == res_fill);
}


int main(int argc, char *argv[]) {
    assert_test();
    
    //TODO: matrix rank
    //TODO: random double generation
    //TODO: assert det
    //TODO: transpose, reversed matrix
    //TODO: auxillary diagonal functions
    //TODO: assert and check power matrix
    //TODO: func multiply dims exceptions
    //TODO: create const matrices such as 1, 0, frob etc.
    //TODO: matrix det (with block LU + openmp)
    //TODO: all functions -> openmp
    //TODO: try and add cuda support
    //TODO: optimize all functions
    //TODO: add assert for set_aux_diagonal
    //TODO: add assert for identity_mat
    
    //TODO: work with sets, fuzzy sets, set operations,
    // discrete math, graphs, graphics implementation?
    // draw graphs as tables
    
    matrix<double, 3, 5> mat1;
    matrix<double, 5, 3> mat2;
    
    fill_mat_(mat1, 2.);
    fill_mat_(mat2, 4.);
    
    std::cout << "mat1:\n";
    show_mat(mat1);
    
    std::cout << "mat2:\n";
    show_mat(mat2);
    
    std::cout << "aux diagonal -> 2\n";
    show_mat(set_aux_diagonal(mat1, 0.1));
    
    matrix<double, 5, 5> imat;
    fill_identity_mat_(imat);
    std::cout << "5x5 identity matrix:\n";
    show_mat(imat);
    
    matrix<double, 3, 3> mat = \
    {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    std::cout << "mat det: " << matrix_det(mat) << "\n";
    
    return 0;
}