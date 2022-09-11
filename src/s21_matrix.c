#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error_code = OK;

  if (!result || rows < 1 || columns < 1) {
    error_code = INCORRECT_MATRIX;
  } else {
    *result = init_matrix();
    result->rows = rows;
    result->columns = columns;
    result->matrix = allocate_matrix(rows, columns);

    if (result->matrix) {
      fill_matrix_zero(result);
    } else {
      error_code = INCORRECT_MATRIX;
      free_matrix(rows, result->matrix);
    }
  }
  return error_code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    free_matrix(A->rows, A->matrix);
    *A = init_matrix();
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int result = FALSE;

  if (A && B && A->matrix && B->matrix && check_eq_size(A, B)) {
    int flag = 1;
    for (int i = 0; i < A->rows && flag == 1; i++) {
      for (int j = 0; j < A->columns; j++) {
        double diff_abs = fabs(A->matrix[i][j] - B->matrix[i][j]);

        if (diff_abs > 1e-7) {
          flag--;
          break;
        }
      }
    }

    if (flag) {
      result = TRUE;
    }
  }
  return result;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A) && check_matrix(B)) {
    if (check_eq_size(A, B)) {
      error_code = s21_create_matrix(A->rows, A->columns, result);
    }
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return error_code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A) && check_matrix(B)) {
    if (check_eq_size(A, B)) {
      error_code = s21_create_matrix(A->rows, A->columns, result);
    }
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return error_code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A)) {
    error_code = s21_create_matrix(A->rows, A->columns, result);
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return error_code;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A) && check_matrix(B)) {
    if (A->columns == B->rows) {
      error_code = s21_create_matrix(A->rows, B->columns, result);
    }
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }
  return error_code;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A)) {
    error_code = s21_create_matrix(A->columns, A->rows, result);
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
  }
  return error_code;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A)) {
    if (A->rows == A->columns) {
      error_code = s21_create_matrix(A->rows, A->columns, result);
    }
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    for (int i = 0; i < result->rows && error_code == OK; i++) {
      for (int j = 0; j < result->columns && error_code == OK; j++) {
        double determinant = 0;
        matrix_t buffer = init_matrix();

        error_code = create_minor_elements_matrix(A, &buffer, i, j);
        error_code = s21_determinant(&buffer, &determinant);
        result->matrix[i][j] = pow(-1, i + j) * determinant;
        s21_remove_matrix(&buffer);
      }
    }
  }
  return error_code;
}

int s21_determinant(matrix_t *A, double *result) {
  int error_code = CALCULATION_ERROR;

  if (check_matrix(A)) {
    if (A->rows == A->columns && result) {
      *result = 0;
      error_code = OK;

      if (A->rows == 1) {
        *result = A->matrix[0][0];
      } else if (A->rows == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] -
                  A->matrix[0][1] * A->matrix[1][0];
      } else {
        matrix_t buffer = init_matrix();
        int sign = 1;

        for (int i = 0; i < A->columns && error_code == OK; i++) {
          error_code = create_minor_elements_matrix(A, &buffer, 0, i);
          double minor_result = 0;
          error_code = s21_determinant(&buffer, &minor_result);

          *result += A->matrix[0][i] * minor_result * sign;
          sign = -1 * sign;
          s21_remove_matrix(&buffer);
        }
      }
    }
  } else {
    error_code = INCORRECT_MATRIX;
  }
  return error_code;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int error_code = CALCULATION_ERROR;
  double determinant = 0;

  if (check_matrix(A)) {
    if (A->rows == A->columns) {
      error_code = s21_determinant(A, &determinant);
    }
  } else {
    error_code = INCORRECT_MATRIX;
  }

  if (error_code == OK) {
    if (fabs(determinant) > 1e-7) {
      matrix_t m_complements = init_matrix();
      matrix_t m_transposed = init_matrix();

      error_code = s21_calc_complements(A, &m_complements);
      error_code = s21_transpose(&m_complements, &m_transposed);
      error_code = s21_mult_number(&m_transposed, 1.0 / determinant, result);

      s21_remove_matrix(&m_complements);
      s21_remove_matrix(&m_transposed);
    } else {
      error_code = CALCULATION_ERROR;
    }
  }
  return error_code;
}
