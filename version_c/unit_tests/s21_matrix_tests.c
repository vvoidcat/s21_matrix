#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/s21_matrix.h"

// TESTS

// create

START_TEST(test_create_matrix_1) {
  matrix_t m0;

  ck_assert_int_eq(s21_create_matrix(2, 2, &m0), OK);
  ck_assert_ptr_ne(m0.matrix, NULL);

  matrix_t m = init_matrix();

  ck_assert_int_eq(s21_create_matrix(m.rows, m.columns, &m), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(-100, -100, &m), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(20, 20, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&m0);
  ck_assert_ptr_eq(m0.matrix, NULL);
}
END_TEST

START_TEST(test_create_matrix_2) {
  matrix_t m = init_matrix();

  for (int i = 1; i < 100; i++) {
    int outv = s21_create_matrix(i, i, &m);
    ck_assert_int_eq(outv, OK);
    ck_assert_int_eq(m.rows, i);
    ck_assert_int_eq(m.columns, i);
    ck_assert_ptr_ne(m.matrix, NULL);
    ck_assert_int_eq(m.matrix[0][0], 0);

    s21_remove_matrix(&m);
    ck_assert_int_eq(m.rows, 0);
    ck_assert_int_eq(m.columns, 0);
    ck_assert_ptr_eq(m.matrix, NULL);
  }
}
END_TEST

// remove

START_TEST(test_remove_matrix_1) {
  matrix_t m = init_matrix();

  s21_remove_matrix(NULL);

  s21_remove_matrix(&m);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
  ck_assert_ptr_eq(m.matrix, NULL);

  m.matrix = allocate_matrix(40, 50);
  m.rows = 40;
  m.columns = 50;

  s21_remove_matrix(&m);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
  ck_assert_ptr_eq(m.matrix, NULL);

  m.rows = -100;
  m.columns = -200;

  s21_remove_matrix(&m);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
  ck_assert_ptr_eq(m.matrix, NULL);

  s21_create_matrix(25, 36, &m);
  s21_remove_matrix(&m);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
  ck_assert_ptr_eq(m.matrix, NULL);
}
END_TEST

START_TEST(test_remove_matrix_2) {
  matrix_t m = init_matrix();

  for (int i = -50; i < 0; i++) {
    int outv = s21_create_matrix(i, i, &m);
    ck_assert_int_eq(outv, INCORRECT_MATRIX);
    ck_assert_int_eq(m.rows, 0);
    ck_assert_int_eq(m.columns, 0);
    ck_assert_ptr_eq(m.matrix, NULL);

    s21_remove_matrix(&m);
    ck_assert_int_eq(m.rows, 0);
    ck_assert_int_eq(m.columns, 0);
    ck_assert_ptr_eq(m.matrix, NULL);
  }
}
END_TEST

// eq

START_TEST(test_eq_matrix_1) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();

  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 20; j++) {
      s21_create_matrix(i, j, &m1);
      s21_create_matrix(j, i, &m2);

      if (i == j) {
        ck_assert_int_eq(s21_eq_matrix(&m1, &m2), TRUE);
      } else {
        ck_assert_int_eq(s21_eq_matrix(&m1, &m2), FALSE);
      }

      s21_remove_matrix(&m1);
      s21_remove_matrix(&m2);
    }
  }
}
END_TEST

START_TEST(test_eq_matrix_2) {
  matrix_t m1 = init_matrix();

  for (int i = 1; i < 20; i++) {
    s21_create_matrix(i, i, &m1);
    fill_matrix_range(&m1, -10.0, 10.0);
    ck_assert_int_eq(s21_eq_matrix(&m1, &m1), TRUE);
    s21_remove_matrix(&m1);
  }
}
END_TEST

START_TEST(test_eq_matrix_3) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();

  for (int i = 1; i < 20; i++) {
    s21_create_matrix(i, i, &m1);
    s21_create_matrix(i, i, &m2);

    fill_matrix_increment(&m1, 23.0);
    fill_matrix_increment(&m2, 2.0);

    ck_assert_int_eq(s21_eq_matrix(&m1, &m2), FALSE);

    s21_remove_matrix(&m1);
    s21_remove_matrix(&m2);
  }
}
END_TEST

START_TEST(test_eq_matrix_4) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();

  ck_assert_int_eq(s21_eq_matrix(&m1, &m2), FALSE);

  s21_create_matrix(10, 30, &m1);
  s21_create_matrix(10, 30, &m2);

  fill_matrix_increment(&m1, 23.0);
  fill_matrix_increment(&m2, 2.0);

  ck_assert_int_eq(s21_eq_matrix(&m1, NULL), FALSE);
  ck_assert_int_eq(s21_eq_matrix(NULL, &m2), FALSE);
  ck_assert_int_eq(s21_eq_matrix(NULL, NULL), FALSE);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

// sum matrix

START_TEST(test_sum_matrix_1) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(10, 30, &m1);
  s21_create_matrix(10, 30, &m2);

  fill_matrix_increment(&m1, 23.0);
  fill_matrix_increment(&m2, 2.0);

  ck_assert_int_eq(s21_sum_matrix(&m1, NULL, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sum_matrix(NULL, &m2, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sum_matrix(NULL, NULL, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sum_matrix(&m1, &m2, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_sum_matrix_2) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 20; j++) {
      s21_create_matrix(i, j, &m1);
      s21_create_matrix(j, i, &m2);

      if (i == j) {
        ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &m_result), OK);
      } else {
        ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &m_result),
                         CALCULATION_ERROR);
      }

      s21_remove_matrix(&m1);
      s21_remove_matrix(&m2);
      s21_remove_matrix(&m_result);
    }
  }
}
END_TEST

START_TEST(test_sum_matrix_3) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  for (int i = 1; i < 20; i++) {
    s21_create_matrix(i, i, &m1);
    s21_create_matrix(i, i, &m2);

    fill_matrix_increment(&m1, 23.0);
    fill_matrix_increment(&m2, 2.0);

    ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &m_result), OK);

    for (int r = 0; r < m1.rows; r++) {
      for (int c = 0; c < m1.columns; c++) {
        ck_assert_int_eq(m_result.matrix[r][c],
                         m1.matrix[r][c] + m2.matrix[r][c]);
      }
    }

    s21_remove_matrix(&m1);
    s21_remove_matrix(&m2);
    s21_remove_matrix(&m_result);
  }
}
END_TEST

// sub matrix

START_TEST(test_sub_matrix_1) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_sub_matrix(&m1, &m2, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(10, 30, &m1);
  s21_create_matrix(10, 30, &m2);

  fill_matrix_increment(&m1, 23.0);
  fill_matrix_increment(&m2, 2.0);

  ck_assert_int_eq(s21_sub_matrix(&m1, NULL, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sub_matrix(NULL, &m2, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sub_matrix(NULL, NULL, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sub_matrix(&m1, &m2, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_sub_matrix_2) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 20; j++) {
      s21_create_matrix(i, j, &m1);
      s21_create_matrix(j, i, &m2);

      if (i == j) {
        ck_assert_int_eq(s21_sub_matrix(&m1, &m2, &m_result), OK);
      } else {
        ck_assert_int_eq(s21_sub_matrix(&m1, &m2, &m_result),
                         CALCULATION_ERROR);
      }

      s21_remove_matrix(&m1);
      s21_remove_matrix(&m2);
      s21_remove_matrix(&m_result);
    }
  }
}
END_TEST

START_TEST(test_sub_matrix_3) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  for (int i = 1; i < 20; i++) {
    s21_create_matrix(i, i, &m1);
    s21_create_matrix(i, i, &m2);

    fill_matrix_range(&m1, 23.0, 70.0);
    fill_matrix_range(&m2, -10.0, 10.0);

    ck_assert_int_eq(s21_sub_matrix(&m1, &m2, &m_result), OK);

    for (int r = 0; r < m1.rows; r++) {
      for (int c = 0; c < m1.columns; c++) {
        ck_assert_int_eq(m_result.matrix[r][c],
                         m1.matrix[r][c] - m2.matrix[r][c]);
      }
    }

    s21_remove_matrix(&m1);
    s21_remove_matrix(&m2);
    s21_remove_matrix(&m_result);
  }
}
END_TEST

// mult matrix

START_TEST(test_mult_matrix_1) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_mult_matrix(&m1, &m2, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(10, 30, &m1);
  s21_create_matrix(30, 10, &m2);

  fill_matrix_increment(&m1, 23.0);
  fill_matrix_increment(&m2, 2.0);

  ck_assert_int_eq(s21_mult_matrix(&m1, NULL, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_mult_matrix(NULL, &m2, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_mult_matrix(NULL, NULL, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_mult_matrix(&m1, &m2, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_mult_matrix_2) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();
  matrix_t m_result = init_matrix();

  s21_create_matrix(2, 2, &m1);
  fill_matrix_increment(&m1, 1);

  s21_create_matrix(2, 2, &m2);
  fill_matrix_increment(&m2, 1);

  int result = s21_mult_matrix(&m1, &m2, &m_result);

  ck_assert_int_eq(m_result.matrix[0][0], 7);
  ck_assert_int_eq(m_result.matrix[0][1], 10);
  ck_assert_int_eq(m_result.matrix[1][0], 15);
  ck_assert_int_eq(m_result.matrix[1][1], 22);
  ck_assert_int_eq(result, OK);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m_result);
}
END_TEST

START_TEST(test_mult_matrix_3) {
  matrix_t m1 = init_matrix();
  matrix_t m2 = init_matrix();

  s21_create_matrix(10, 30, &m1);
  s21_create_matrix(10, 30, &m2);

  ck_assert_int_eq(s21_mult_matrix(&m1, &m2, NULL), CALCULATION_ERROR);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

// mult number

START_TEST(test_mult_number_1) {
  matrix_t m1 = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_mult_number(&m1, 111.34, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(10, 30, &m1);
  fill_matrix_increment(&m1, 23.0);

  ck_assert_int_eq(s21_mult_number(&m1, 2, NULL), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_mult_number(NULL, 4, &m1), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_mult_number(NULL, -45, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&m1);
}
END_TEST

START_TEST(test_mult_number_2) {
  matrix_t m1 = init_matrix();
  matrix_t m_result = init_matrix();

  for (int i = 1; i < 20; i++) {
    s21_create_matrix(i, i, &m1);

    double numb = i - 10;
    fill_matrix_range(&m1, -10.0, 70.0);

    ck_assert_int_eq(s21_mult_number(&m1, numb, &m_result), OK);

    for (int r = 0; r < m1.rows; r++) {
      for (int c = 0; c < m1.columns; c++) {
        ck_assert_int_eq(m_result.matrix[r][c], m1.matrix[r][c] * numb);
      }
    }

    s21_remove_matrix(&m1);
    s21_remove_matrix(&m_result);
  }
}
END_TEST

// transpose

START_TEST(test_transpose_1) {
  matrix_t m1 = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_transpose(&m1, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(10, 30, &m1);
  fill_matrix_increment(&m1, 23.0);

  ck_assert_int_eq(s21_transpose(&m1, NULL), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_transpose(NULL, &m1), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_transpose(NULL, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&m1);
}
END_TEST

START_TEST(test_transpose_2) {
  matrix_t m1 = init_matrix();
  matrix_t m_result = init_matrix();

  s21_create_matrix(2, 3, &m1);
  fill_matrix_increment(&m1, 1);

  int result = s21_transpose(&m1, &m_result);
  ck_assert_int_eq(m_result.matrix[0][0], 1);
  ck_assert_int_eq(m_result.matrix[0][1], 4);
  ck_assert_int_eq(m_result.matrix[1][0], 2);
  ck_assert_int_eq(m_result.matrix[1][1], 5);
  ck_assert_int_eq(m_result.matrix[2][0], 3);
  ck_assert_int_eq(m_result.matrix[2][1], 6);
  ck_assert_int_eq(result, OK);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m_result);
}
END_TEST

// calc complements

START_TEST(test_calc_complements_1) {
  matrix_t m = init_matrix();
  matrix_t m_result = init_matrix();

  s21_create_matrix(3, 3, &m);

  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;
  m.matrix[1][0] = 0;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 2;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  int result = s21_calc_complements(&m, &m_result);

  ck_assert_int_eq(m_result.matrix[0][0], 0);
  ck_assert_int_eq(m_result.matrix[0][1], 10);
  ck_assert_int_eq(m_result.matrix[0][2], -20);
  ck_assert_int_eq(m_result.matrix[1][0], 4);
  ck_assert_int_eq(m_result.matrix[1][1], -14);
  ck_assert_int_eq(m_result.matrix[1][2], 8);
  ck_assert_int_eq(m_result.matrix[2][0], -8);
  ck_assert_int_eq(m_result.matrix[2][1], -2);
  ck_assert_int_eq(m_result.matrix[2][2], 4);
  ck_assert_int_eq(result, OK);

  s21_remove_matrix(&m);
  s21_remove_matrix(&m_result);
}
END_TEST

START_TEST(test_calc_complements_2) {
  matrix_t m = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_calc_complements(&m, NULL), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_calc_complements(NULL, NULL), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_calc_complements(NULL, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(3, 3, &m);

  ck_assert_int_eq(s21_calc_complements(&m, &m_result), OK);

  m.columns = 1;
  ck_assert_int_eq(s21_calc_complements(&m, &m_result), CALCULATION_ERROR);
  s21_remove_matrix(&m_result);

  m.rows = 0;
  m.columns = 0;
  ck_assert_int_eq(s21_calc_complements(&m, &m_result), INCORRECT_MATRIX);

  m.rows = 3;
  m.columns = 3;
  s21_remove_matrix(&m);
  s21_remove_matrix(&m_result);
}
END_TEST

// determinant

START_TEST(test_determinant_1) {
  double determinant = 0;
  matrix_t m = init_matrix();

  s21_create_matrix(3, 3, &m);
  fill_matrix_increment(&m, 1);

  int result = s21_determinant(&m, &determinant);
  ck_assert_int_eq(result, OK);

  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_determinant_2) {
  double determinant = 0;
  matrix_t m = init_matrix();
  matrix_t m2 = init_matrix();

  s21_create_matrix(4, 4, &m);
  s21_create_matrix(1, 1, &m2);

  m.matrix[0][0] = 9;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 2;
  m.matrix[0][3] = 4;

  m.matrix[1][0] = 3;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 4;
  m.matrix[1][3] = 4;

  m.matrix[2][0] = 4;
  m.matrix[2][1] = 4;
  m.matrix[2][2] = 9;
  m.matrix[2][3] = 9;

  m.matrix[3][0] = 1;
  m.matrix[3][1] = 1;
  m.matrix[3][2] = 5;
  m.matrix[3][3] = 1;

  int result = s21_determinant(&m, &determinant);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(determinant, -578);

  m2.matrix[0][0] = 10;
  result = s21_determinant(&m2, &determinant);

  ck_assert_int_eq(result, OK);
  ck_assert_int_eq(determinant, 10);

  s21_remove_matrix(&m);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_determinant_3) {
  matrix_t m = init_matrix();
  double determinant = 0;

  ck_assert_int_eq(s21_determinant(NULL, &determinant), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_determinant(NULL, NULL), INCORRECT_MATRIX);

  s21_create_matrix(2, 2, &m);
  ck_assert_int_eq(s21_determinant(&m, NULL), CALCULATION_ERROR);

  m.rows = 4;
  ck_assert_int_eq(s21_determinant(&m, &determinant), CALCULATION_ERROR);

  m.columns = 0;
  ck_assert_int_eq(s21_determinant(&m, &determinant), CALCULATION_ERROR);

  m.rows = 2;
  m.columns = 2;
  s21_remove_matrix(&m);
}
END_TEST

// inverse

START_TEST(test_inverse_matrix_1) {
  matrix_t m = init_matrix();
  matrix_t m_result = init_matrix();

  s21_create_matrix(3, 3, &m);

  m.matrix[0][0] = 4;
  m.matrix[0][1] = -2;
  m.matrix[0][2] = 1;
  m.matrix[1][0] = 1;
  m.matrix[1][1] = 6;
  m.matrix[1][2] = -2;
  m.matrix[2][0] = 1;
  m.matrix[2][1] = 0;
  m.matrix[2][2] = 0;

  int result = s21_inverse_matrix(&m, &m_result);

  ck_assert_int_eq(m_result.matrix[0][1], 0);
  ck_assert_int_eq(m_result.matrix[0][2], 1);
  ck_assert_int_eq(m_result.matrix[1][0], 1);
  ck_assert_int_eq(m_result.matrix[2][0], 3);
  ck_assert_int_eq(m_result.matrix[2][1], 1);
  ck_assert_int_eq(m_result.matrix[2][2], -13);
  ck_assert_int_eq(result, OK);

  s21_remove_matrix(&m);
  s21_remove_matrix(&m_result);
}
END_TEST

START_TEST(test_inverse_matrix_2) {
  matrix_t m = init_matrix();
  matrix_t m_result = init_matrix();

  ck_assert_int_eq(s21_inverse_matrix(NULL, NULL), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_inverse_matrix(&m, NULL), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_inverse_matrix(&m, &m_result), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_inverse_matrix(NULL, &m_result), INCORRECT_MATRIX);

  s21_create_matrix(3, 3, &m);

  m.matrix[0][0] = 1;
  m.matrix[0][1] = 4;
  m.matrix[0][2] = 1;
  m.matrix[1][0] = 3;
  m.matrix[1][1] = 7;
  m.matrix[1][2] = 2;
  m.matrix[2][0] = 3;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  double determinant = 0;
  s21_determinant(&m, &determinant);
  ck_assert_int_lt(determinant, 1);

  ck_assert_int_eq(s21_inverse_matrix(&m, &m_result), CALCULATION_ERROR);
  s21_remove_matrix(&m_result);

  m.rows = 0;
  s21_create_matrix(3, 3, &m_result);
  ck_assert_int_eq(s21_inverse_matrix(&m, &m_result), CALCULATION_ERROR);

  m.rows = 3;
  s21_remove_matrix(&m);
  s21_remove_matrix(&m_result);
}
END_TEST

// SUITES

Suite *suite_create_matrix() {
  Suite *s = suite_create("suite_create_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");

  tcase_add_test(tc_1, test_create_matrix_1);
  tcase_add_test(tc_2, test_create_matrix_2);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);

  return s;
}

Suite *suite_remove_matrix() {
  Suite *s = suite_create("suite_remove_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");

  tcase_add_test(tc_1, test_remove_matrix_1);
  tcase_add_test(tc_2, test_remove_matrix_2);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);

  return s;
}

Suite *suite_eq_matrix() {
  Suite *s = suite_create("suite_eq_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");
  TCase *tc_3 = tcase_create("tc_3");
  TCase *tc_4 = tcase_create("tc_4");

  tcase_add_test(tc_1, test_eq_matrix_1);
  tcase_add_test(tc_2, test_eq_matrix_2);
  tcase_add_test(tc_3, test_eq_matrix_3);
  tcase_add_test(tc_4, test_eq_matrix_4);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);
  suite_add_tcase(s, tc_3);
  suite_add_tcase(s, tc_4);

  return s;
}

Suite *suite_sum_matrix() {
  Suite *s = suite_create("suite_sum_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");
  TCase *tc_3 = tcase_create("tc_3");

  tcase_add_test(tc_1, test_sum_matrix_1);
  tcase_add_test(tc_2, test_sum_matrix_2);
  tcase_add_test(tc_3, test_sum_matrix_3);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);
  suite_add_tcase(s, tc_3);

  return s;
}

Suite *suite_sub_matrix() {
  Suite *s = suite_create("suite_sub_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");
  TCase *tc_3 = tcase_create("tc_3");

  tcase_add_test(tc_1, test_sub_matrix_1);
  tcase_add_test(tc_2, test_sub_matrix_2);
  tcase_add_test(tc_3, test_sub_matrix_3);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);
  suite_add_tcase(s, tc_3);

  return s;
}

Suite *suite_mult_matrix() {
  Suite *s = suite_create("suite_mult_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");
  TCase *tc_3 = tcase_create("tc_3");

  tcase_add_test(tc_1, test_mult_matrix_1);
  tcase_add_test(tc_2, test_mult_matrix_2);
  tcase_add_test(tc_3, test_mult_matrix_3);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);
  suite_add_tcase(s, tc_3);

  return s;
}

Suite *suite_mult_number() {
  Suite *s = suite_create("suite_mult_number");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");

  tcase_add_test(tc_1, test_mult_number_1);
  tcase_add_test(tc_2, test_mult_number_2);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);

  return s;
}

Suite *suite_transpose() {
  Suite *s = suite_create("suite_transpose");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");

  tcase_add_test(tc_1, test_transpose_1);
  tcase_add_test(tc_2, test_transpose_2);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);

  return s;
}

Suite *suite_calc_complements() {
  Suite *s = suite_create("suite_calc_complements");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");

  tcase_add_test(tc_1, test_calc_complements_1);
  tcase_add_test(tc_2, test_calc_complements_2);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);

  return s;
}

Suite *suite_determinant() {
  Suite *s = suite_create("suite_determinant");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");
  TCase *tc_3 = tcase_create("tc_3");

  tcase_add_test(tc_1, test_determinant_1);
  tcase_add_test(tc_2, test_determinant_2);
  tcase_add_test(tc_3, test_determinant_3);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);
  suite_add_tcase(s, tc_3);

  return s;
}

Suite *suite_inverse_matrix() {
  Suite *s = suite_create("suite_inverse_matrix");
  TCase *tc_1 = tcase_create("tc_1");
  TCase *tc_2 = tcase_create("tc_2");

  tcase_add_test(tc_1, test_inverse_matrix_1);
  tcase_add_test(tc_2, test_inverse_matrix_2);

  suite_add_tcase(s, tc_1);
  suite_add_tcase(s, tc_2);

  return s;
}

// MAIN

void run_test(Suite *thesuit) {
  SRunner *therunner = srunner_create(thesuit);

  srunner_set_fork_status(therunner, CK_NOFORK);
  srunner_run_all(therunner, CK_NORMAL);
  srunner_ntests_failed(therunner);
  srunner_free(therunner);

  printf("\n");
}

int main() {
  printf("\n");
  srand(time(NULL));

  Suite *s_create_matrix = suite_create_matrix();
  Suite *s_remove_matrix = suite_remove_matrix();
  Suite *s_eq_matrix = suite_eq_matrix();
  Suite *s_sum_matrix = suite_sum_matrix();
  Suite *s_sub_matrix = suite_sub_matrix();
  Suite *s_mult_matrix = suite_mult_matrix();
  Suite *s_mult_number = suite_mult_number();
  Suite *s_transpose = suite_transpose();
  Suite *s_calc_complements = suite_calc_complements();
  Suite *s_determinant = suite_determinant();
  Suite *s_inverse_matrix = suite_inverse_matrix();

  run_test(s_create_matrix);
  run_test(s_remove_matrix);
  run_test(s_eq_matrix);
  run_test(s_sum_matrix);
  run_test(s_sub_matrix);
  run_test(s_mult_matrix);
  run_test(s_mult_number);
  run_test(s_transpose);
  run_test(s_calc_complements);
  run_test(s_determinant);
  run_test(s_inverse_matrix);

  return 0;
}
