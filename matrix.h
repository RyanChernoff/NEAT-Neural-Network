#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>


typedef struct matrix_header *matrix_t;

/**
 * @brief creates a new matrix
 * @param rows the number of rows
 * @param cols the number of columns
 */
//Precondition: rows > 0 and cols > 0
//Postcondition: Result != NULL
matrix_t matrix_new(size_t rows, size_t cols);

/**
 * @brief returns the number of rows of a matrix
 * @param M the matrix to query
 */
//Precondition: M != NULL
size_t matrix_get_rows(matrix_t M);

/**
 * @brief returns the number of columns in a matrix
 * @param M the matrix to query
 */
//Precondition: M != NULL
size_t matrix_get_cols(matrix_t M);

/**
 * @brief gets the value of the matrix at the given row and column
 * @param M the matrix to query
 * @param row the row to query
 * @param col the column to query
 */
//Precondition: M != NULL, rows < matrix_get_rows(M), and cols < matrix_get_cols(M)
double matrix_get(matrix_t M, size_t row, size_t col);

/**
 * @brief changes the value of a matrix at the given row and column
 * @param M the matrix to change
 * @param row the row to change
 * @param col the column to change
 */
//Precondition: M != NULL, rows < matrix_get_rows(M), and cols < matrix_get_cols(M)
void matrix_set(matrix_t M, size_t row, size_t col, double val);

/**
 * @brief creates a new matrix consisting of AB using matrix multiplication
 * @param A the matrix on the left
 * @param B the matrix on the right
 */
//Must free result
//Precondition: A != NULL, B != NULL, and matrix_get_cols(A) == matrix_get_rows(B)
//Postcondition: Result != NULL
matrix_t matrix_mult(matrix_t A, matrix_t B);

/**
 * @brief prints out the contents of a matrix
 * @param M the matrix to print
 */
//Precondition: M != NULL
void matrix_print(matrix_t M);

/**
 * @brief frees a matrix
 * @param M the matrix to free
 */
//Precondition: M != NULL
//Postcondition: M is freed
void matrix_free(matrix_t M);

#endif