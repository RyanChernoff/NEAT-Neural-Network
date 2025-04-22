#include <stdio.h>
#include <stdlib.h>

struct matrix_header {
  size_t rows;
  size_t cols;
  double *data;
};
typedef struct matrix_header matrix;

matrix *matrix_new(size_t rows, size_t cols){
  matrix *M = malloc(sizeof(matrix));
  M->rows = rows;
  M->cols = cols;
  M->data = calloc(rows*cols, sizeof(double));
  return M;
}

size_t matrix_get_rows(matrix *M){
  return M->rows;
}

size_t matrix_get_cols(matrix *M){
  return M->cols;
}

double matrix_get(matrix *M, size_t row, size_t col){
  return M->data[row*M->cols + col];
}

void matrix_set(matrix *M, size_t row, size_t col, double val){
  M->data[row*M->cols + col] = val;
}

matrix *matrix_mult(matrix *A, matrix *B){
  matrix *M = malloc(sizeof(matrix));
  M->rows = A->rows;
  M->cols = B->cols;
  M->data = malloc(A->rows*B->cols*sizeof(double));
  for(size_t i = 0; i < A->rows; i++){
    for(size_t j = 0; j < B->cols; j++){
      double sum = 0;
      for(size_t k = 0; k < A->cols; k++){
        sum += matrix_get(A, i, k)*matrix_get(B, k , j);
      }
      matrix_set(M, i, j, sum);
    }
  }
  return M;
}

void matrix_print(matrix *M){
  for(size_t i = 0; i < M->rows; i++){
    for(size_t j = 0; j < M->cols; j++){
      printf("%f ", matrix_get(M, i, j));
    }
    printf("\n");
  }
}

void matrix_free(matrix *M){
  free(M->data);
  free(M);
}