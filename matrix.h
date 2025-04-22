#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>


typedef struct matrix_header *matrix_t;

//Precondition: rows > 0 and cols > 0
//Postcondition: Result != NULL
matrix_t matrix_new(size_t rows, size_t cols);

//Precondition: M != NULL
size_t matrix_get_rows(matrix_t M);

//Precondition: M != NULL
size_t matrix_get_cols(matrix_t M);

//Precondition: M != NULL, rows < matrix_get_rows(M), and cols < matrix_get_cols(M)
double matrix_get(matrix_t M, size_t row, size_t col);

//Precondition: M != NULL, rows < matrix_get_rows(M), and cols < matrix_get_cols(M)
void matrix_set(matrix_t M, size_t row, size_t col, double val);

//Must free result
//Precondition: A != NULL, B != NULL, and matrix_get_cols(A) == matrix_get_rows(B)
//Postcondition: Result != NULL
matrix_t matrix_mult(matrix_t A, matrix_t B);

//Precondition: M != NULL
void matrix_print(matrix_t M);

//Precondition: M != NULL
//Postcondition: M is freed
void matrix_free(matrix_t M);

#endif