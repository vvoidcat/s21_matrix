#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2
#define MEMORY_ERROR 3

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// main
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// helpers
matrix_t init_matrix();
double **allocate_matrix(int rows, int columns);
void free_matrix(int rows, double **array);
int check_matrix(matrix_t *matrix);
int check_eq_size(matrix_t *m1, matrix_t *m2);
void fill_matrix_zero(matrix_t *matrix);
void fill_matrix_increment(matrix_t *matrix, int value);
void fill_matrix_range(matrix_t *matrix, double minvalue, double maxvalue);
double generate_value_in_range(double min, double max);
void print_matrix(matrix_t *matrix);
int create_minor_elements_matrix(matrix_t *origin, matrix_t *result,
                                 int index_rows, int index_columns);

#endif  // SRC_S21_MATRIX_H_
