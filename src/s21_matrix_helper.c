#include "s21_matrix.h"

matrix_t init_matrix() {
  matrix_t m;
  m.rows = 0;
  m.columns = 0;
  m.matrix = NULL;
  return m;
}

double **allocate_matrix(int rows, int columns) {
  double **result = malloc(sizeof(double *) * rows);

  if (result) {
    for (int i = 0; i < rows; i++) {
      result[i] = malloc(sizeof(double) * columns);
      if (!result[i]) {
        break;
      }
    }
  }
  return result;
}

void free_matrix(int rows, double **array) {
  if (array) {
    for (int i = 0; i < rows; i++) {
      if (array[i]) {
        free(array[i]);
      }
    }
    free(array);
  }
}

int check_matrix(matrix_t *matrix) {
  int result = FALSE;

  if (matrix && matrix->matrix) {
    result = TRUE;
  }
  return result;
}

int check_eq_size(matrix_t *m1, matrix_t *m2) {
  int result = FALSE;

  if (m1->columns == m2->columns && m1->rows == m2->rows) {
    result = TRUE;
  }
  return result;
}

void fill_matrix_zero(matrix_t *matrix) {
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      matrix->matrix[i][j] = 0;
    }
  }
}

void fill_matrix_increment(matrix_t *matrix, int value) {
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      matrix->matrix[i][j] = value++;
    }
  }
}

// requires calling `srand(time(NULL));` in main to actually produce random
// numbers. without the random seed, it'll give a fixed array of numbers in the
// specified range.
void fill_matrix_range(matrix_t *matrix, double minvalue, double maxvalue) {
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      matrix->matrix[i][j] = generate_value_in_range(minvalue, maxvalue);
    }
  }
}

double generate_value_in_range(double min, double max) {
  double range = max - min;
  return min + (rand() / (RAND_MAX / range));
}

void print_matrix(matrix_t *matrix) {
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      printf("%lf ", matrix->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int create_minor_elements_matrix(matrix_t *origin, matrix_t *result,
                                 int index_rows, int index_columns) {
  int elem_rows = 0;
  int elem_columns = 0;
  int error_code =
      s21_create_matrix(origin->rows - 1, origin->columns - 1, result);

  if (error_code == OK) {
    for (int i = 0; i < origin->rows; i++) {
      for (int j = 0; j < origin->columns; j++) {
        if (i != index_rows && j != index_columns) {
          result->matrix[elem_rows][elem_columns] = origin->matrix[i][j];
          elem_columns++;
        }
      }
      if (i != index_rows) {
        elem_rows++;
      }
      elem_columns = 0;
    }
  }
  return error_code;
}
