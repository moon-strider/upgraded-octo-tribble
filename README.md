# OpenMP Matrix Operations Library

This is a simple C++ library that provides functions for performing matrix operations using OpenMP for parallel computing.

### Requirements

To use this library, you will need:

- A C++ compiler that supports OpenMP.
- The OpenMP library.

### Contents

This library contains the following:
- matrix: this function represents a matrix with the specified rows and columns.
- transpose_block_SSE4x4: accepts two matrices, a number of rows, a number of columns, and a block size. It transposes the first matrix and stores the result in the second matrix using the specified block size.
- add_matrices: accepts two matrices and adds them element-wise.
- set_above_main_diagonal, set_above_sec_diagonal, set_under_main_diagonal, set_under_sec_diagonal, set_main_diagonal, and set_sec_diagonal: accept a matrix and set the specified elements on the diagonal, below/above the diagonal, or the secondary diagonal.
- show_mat: accepts a matrix and prints it to standard output.
- fill_mat_: accepts a matrix and fills it with random numbers.
- fill_identity_mat_: accepts a matrix and fills it with an identity matrix.
- multiply_matrices: accepts two matrices and multiplies them.
- multiply_matrix: accepts a matrix and a number, and multiplies them element-wise.
- matrix_pow: accepts a matrix and an exponent, and raises the matrix to the specified power.
- matrix_det: accepts a matrix and calculates its determinant.
- matrix_rank: accepts a matrix and calculates its rank.
- assert_test: tests all implemented functions.

The main function initializes two matrices and demonstrates the use of the matrix functions by calling several of them on the matrices. It also displays the rank of a matrix of size 3x3. There are also several TODO comments indicating areas for improvement.
