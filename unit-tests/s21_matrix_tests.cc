#include <gtest/gtest.h>

#include "../src/s21_matrix_oop.h"

TEST(CONSTRUCTORS, NOERR) {
  S21Matrix test = S21Matrix();
  EXPECT_EQ(test.GetRowsCount(), 0);
  EXPECT_EQ(test.GetColsCount(), 0);
  EXPECT_TRUE(test.GetMatrix() == nullptr);

  test = S21Matrix(4, 4);
  EXPECT_EQ(test.GetRowsCount(), 4);
  EXPECT_EQ(test.GetColsCount(), 4);
  EXPECT_TRUE(test.GetMatrix() != nullptr);

  S21Matrix test2 = S21Matrix(test);
  EXPECT_EQ(test.GetRowsCount(), test2.GetRowsCount());
  EXPECT_EQ(test.GetColsCount(), test2.GetColsCount());
  EXPECT_TRUE(test2.GetMatrix() != nullptr);

  test.SetMatrix(4.4f);
  test2.SetMatrix(4.4f);
  for (int i = 0; i < test.GetRowsCount(); i++) {
    for (int j = 0; j < test.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(test(i, j), test2(i, j));
    }
  }

  S21Matrix test3(std::move(test));
  EXPECT_EQ(test.GetRowsCount(), 0);
  EXPECT_EQ(test.GetColsCount(), 0);
  EXPECT_TRUE(test.GetMatrix() == nullptr);
  EXPECT_EQ(test3.GetRowsCount(), test2.GetRowsCount());
  EXPECT_EQ(test3.GetColsCount(), test2.GetColsCount());
  EXPECT_TRUE(test3.GetMatrix() != nullptr);
  for (int i = 0; i < test3.GetRowsCount(); i++) {
    for (int j = 0; j < test3.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(test3(i, j), test2(i, j));
      EXPECT_ANY_THROW(EXPECT_DOUBLE_EQ(test(i, j), test3(i, j)));
    }
  }
}

TEST(GETSET, NOERR) {
  S21Matrix test1 = S21Matrix(4, 4);
  test1.SetMatrix(5);
  test1.SetColsCount(40);

  EXPECT_EQ(test1.GetColsCount(), 40);
  EXPECT_EQ(test1.GetRowsCount(), 4);
  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      if (j < 4) {
        EXPECT_DOUBLE_EQ(test1(i, j), 5);

      } else {
        EXPECT_DOUBLE_EQ(test1(i, j), 0);
      }
    }
  }

  S21Matrix test2 = S21Matrix(4, 40);
  test1.SetMatrix(test2);

  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(test1(i, j), 0);
    }
  }

  test1.SetRowsCount(40);
  test2.SetRowsCount(40);

  test1.SetRowsCount(test1.GetRowsCount());
  test2.SetColsCount(test2.GetColsCount());

  test1.SetMatrix(0, 1);
  test2.SetMatrix(0, 1);

  EXPECT_EQ(test1.GetRowsCount(), 40);
  EXPECT_EQ(test2.GetRowsCount(), 40);
  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(test1(i, j), test2(i, j));
    }
  }

  test1.SetElementAtIndex(5, 5, 555.1f);
  test2(5, 5) = 555.1f;

  EXPECT_DOUBLE_EQ(test1.GetElementAtIndex(5, 5), 555.1f);
  EXPECT_DOUBLE_EQ(test2.GetElementAtIndex(5, 5), 555.1f);
  EXPECT_DOUBLE_EQ(test1(5, 5), test2(5, 5));
}

TEST(GETSET, ERR) {
  S21Matrix test1 = S21Matrix(4, 4);
  test1.SetMatrix(4);

  for (int i = -10; i < 0; i++) {
    for (int j = -10; j < 0; j++) {
      EXPECT_ANY_THROW(EXPECT_DOUBLE_EQ(test1(i, j), 0));
      EXPECT_ANY_THROW(EXPECT_DOUBLE_EQ(test1.GetElementAtIndex(i, j), 0));
      EXPECT_ANY_THROW(test1.SetElementAtIndex(i, j, 1));
      EXPECT_ANY_THROW(test1(i, j) == 0);
    }
  }

  for (int i = 4; i <= 10; i++) {
    for (int j = 4; j <= 10; j++) {
      EXPECT_ANY_THROW(EXPECT_DOUBLE_EQ(test1(i, j), 0));
      EXPECT_ANY_THROW(EXPECT_DOUBLE_EQ(test1.GetElementAtIndex(i, j), 0));
      EXPECT_ANY_THROW(test1.SetElementAtIndex(i, j, 1));
      EXPECT_ANY_THROW(test1(i, j) = 0);
    }
  }
}

TEST(EQ, NOERR) {
  S21Matrix test1 = S21Matrix(4, 4);
  S21Matrix test2 = S21Matrix(test1);
  S21Matrix test3 = S21Matrix(1, 1);

  EXPECT_TRUE(test1.EqMatrix(test2));
  EXPECT_FALSE(test2.EqMatrix(test3));

  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      test1.SetMatrix(i, j);
      test2.SetMatrix(-10, -10);
      EXPECT_FALSE(test1 == test2);
      EXPECT_FALSE(test2 == test3);
    }
  }

  S21Matrix test4 = test1;
  test3 = test1;

  EXPECT_TRUE(test4 == test1);
  EXPECT_TRUE(test3 == test4);

  test4.SetMatrix(0, 0);
  test2.SetColsCount(15);

  EXPECT_FALSE(test3 == test4);
  EXPECT_FALSE(test3.EqMatrix(test4));
  EXPECT_FALSE(test2 == test3);
}

TEST(OPERATORS, NOERR) {
  S21Matrix test1 = S21Matrix(4, 4);
  S21Matrix test2 = S21Matrix(test1);
  test1.SetMatrix(4);

  S21Matrix test3 = test1 + test2;
  test2 += test1;

  EXPECT_TRUE(test3 == test1);
  EXPECT_TRUE(test2 == test1);

  S21Matrix test4 = test3 - test2;
  test2 -= test3;

  EXPECT_TRUE(test2 == test4);
  EXPECT_FALSE(test4 == test3);

  test1 = test3 * 1;
  test2 *= 1;

  EXPECT_FALSE(test1 == test2);
  EXPECT_TRUE(test1 == test3);

  test1 = test3 * test3;
  test2 *= test3;

  EXPECT_FALSE(test1 == test2);
  EXPECT_FALSE(test1 == test3);
}

TEST(OPERATORS, ERR) {
  S21Matrix test1 = S21Matrix(4, 4);
  S21Matrix test2 = S21Matrix();
  test1.SetMatrix(4);

  EXPECT_ANY_THROW(test1 += test2);
  EXPECT_ANY_THROW(test1 -= test2);
  EXPECT_ANY_THROW(test1 *= test2);
  EXPECT_ANY_THROW(test2 *= 1);

  EXPECT_ANY_THROW(test1 + test2);
  EXPECT_ANY_THROW(test1 - test2);
  EXPECT_ANY_THROW(test1 * test2);
  EXPECT_ANY_THROW(test2 * 1);
  EXPECT_ANY_THROW(1 * test2);

  S21Matrix test3 = S21Matrix(3, 4);
  S21Matrix test4 = S21Matrix(4, 50);

  EXPECT_ANY_THROW(test1 += test3);
  EXPECT_ANY_THROW(test1 -= test4);
  EXPECT_ANY_THROW(test3 *= test4);
  EXPECT_ANY_THROW(test1 + test3);
  EXPECT_ANY_THROW(test1 - test4);
  EXPECT_ANY_THROW(test3 * test4);
}

TEST(SUMSUB, NOERR) {
  S21Matrix test1 = S21Matrix(30, 30);
  S21Matrix test2 = S21Matrix(test1);
  test2.SetMatrix(1);

  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(test1(i, j), i);
    }
    test1.SumMatrix(test2);
  }

  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(test1(i, j), test1.GetRowsCount() - i);
    }
    test1.SubMatrix(test2);
  }
}

TEST(MULT, NOERR) {
  S21Matrix test1 = S21Matrix(30, 30);
  S21Matrix test2 = S21Matrix(test1);
  test1.SetMatrix(1);
  test2.SetMatrix(2);

  EXPECT_TRUE(test1 * test2 == test2);
  EXPECT_TRUE(test2 * test2 == test2 * 2);
  EXPECT_TRUE(test1 * 44 == test2 * 22);
}

TEST(TRANSPOSE, NOERR) {
  S21Matrix test1 = S21Matrix(30, 30);
  S21Matrix test2 = S21Matrix(1, 5);
  S21Matrix test3 = S21Matrix(3, 2);
  test1.SetMatrix(2);
  test2.SetMatrix(1, 1);
  test3.SetMatrix(1, 1);

  std::cout << "origin matrix: transpose\n";
  test3.PrintMatrix();

  S21Matrix trans1 = test1.Transpose();
  S21Matrix trans2 = test2.Transpose();
  S21Matrix trans3 = test3.Transpose();

  std::cout << "result matrix: transpose\n";
  trans3.PrintMatrix();

  EXPECT_TRUE(trans1.GetColsCount() == test1.GetRowsCount());
  EXPECT_TRUE(trans1.IsSquare());
  EXPECT_TRUE(trans1 == test1);
  for (int i = 0; i < test1.GetRowsCount(); i++) {
    for (int j = 0; j < test1.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(trans1(i, j), test1(i, j));
    }
  }

  EXPECT_TRUE(trans2.GetColsCount() == test2.GetRowsCount());
  EXPECT_TRUE(trans2.GetRowsCount() == test2.GetColsCount());
  EXPECT_TRUE(trans2 != test2);
  for (int i = 0; i < test2.GetRowsCount(); i++) {
    for (int j = 0; j < test2.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(trans2(j, i), test2(i, j));
    }
  }

  EXPECT_TRUE(trans3.GetColsCount() == test3.GetRowsCount());
  EXPECT_TRUE(trans3.GetRowsCount() == test3.GetColsCount());
  EXPECT_TRUE(trans3 != test3);
  for (int i = 0; i < test3.GetRowsCount(); i++) {
    for (int j = 0; j < test3.GetColsCount(); j++) {
      EXPECT_DOUBLE_EQ(trans3(j, i), test3(i, j));
    }
  }
}

TEST(TRANSPOSE, ERR) {
  S21Matrix test1 = S21Matrix(-1, -1);
  S21Matrix test2 = S21Matrix(0, 0);

  EXPECT_ANY_THROW(test1.Transpose());
  EXPECT_ANY_THROW(test2.Transpose());
}

TEST(CALC_COMPLEMENTS, NOERR) {
  S21Matrix test = S21Matrix(3, 3);

  test(0, 0) = 1;
  test(0, 1) = 2;
  test(0, 2) = 3;
  test(1, 0) = 0;
  test(1, 1) = 4;
  test(1, 2) = 2;
  test(2, 0) = 5;
  test(2, 1) = 2;
  test(2, 2) = 1;

  std::cout << "origin matrix: calc_complements\n";
  test.PrintMatrix();

  S21Matrix result = test.CalcComplements();

  std::cout << "result matrix: calc_complements\n";
  result.PrintMatrix();

  EXPECT_DOUBLE_EQ(result(0, 0), 0);
  EXPECT_DOUBLE_EQ(result(0, 1), 10);
  EXPECT_DOUBLE_EQ(result(0, 2), -20);
  EXPECT_DOUBLE_EQ(result(1, 0), 4);
  EXPECT_DOUBLE_EQ(result(1, 1), -14);
  EXPECT_DOUBLE_EQ(result(1, 2), 8);
  EXPECT_DOUBLE_EQ(result(2, 0), -8);
  EXPECT_DOUBLE_EQ(result(2, 1), -2);
  EXPECT_DOUBLE_EQ(result(2, 2), 4);
}

TEST(CALC_COMPLEMENTS, ERR) {
  S21Matrix test1 = S21Matrix(-1, -1);
  S21Matrix test2 = S21Matrix(0, 0);

  EXPECT_ANY_THROW(test1.CalcComplements());
  EXPECT_ANY_THROW(test2.CalcComplements());

  S21Matrix test3 = S21Matrix(2, 3);
  S21Matrix test4 = S21Matrix(50, 20);

  EXPECT_ANY_THROW(test3.CalcComplements());
  EXPECT_ANY_THROW(test4.CalcComplements());
}

TEST(DETERMINANT, NOERR) {
  S21Matrix test1 = S21Matrix(4, 4);

  test1(0, 0) = 9;
  test1(0, 1) = 2;
  test1(0, 2) = 2;
  test1(0, 3) = 4;

  test1(1, 0) = 3;
  test1(1, 1) = 4;
  test1(1, 2) = 4;
  test1(1, 3) = 4;

  test1(2, 0) = 4;
  test1(2, 1) = 4;
  test1(2, 2) = 9;
  test1(2, 3) = 9;

  test1(3, 0) = 1;
  test1(3, 1) = 1;
  test1(3, 2) = 5;
  test1(3, 3) = 1;

  std::cout << "origin matrix: determinant\n";
  test1.PrintMatrix();

  double determ1 = test1.Determinant();
  EXPECT_DOUBLE_EQ(determ1, -578);
  std::cout << "result: " << determ1 << "\n";

  S21Matrix test2 = S21Matrix(1, 1);
  test2(0, 0) = 10;
  double determ2 = test2.Determinant();
  EXPECT_DOUBLE_EQ(determ2, 10);

  S21Matrix test3 = S21Matrix(9, 9);
  test3.SetMatrix(1);
  double determ3 = test3.Determinant();
  EXPECT_DOUBLE_EQ(determ3, 0);
}

TEST(DETERMINANT, ERR) {
  S21Matrix test1 = S21Matrix(-1, -1);
  S21Matrix test2 = S21Matrix(0, 0);

  EXPECT_ANY_THROW(test1.Determinant());
  EXPECT_ANY_THROW(test2.Determinant());

  S21Matrix test3 = S21Matrix(2, 3);
  S21Matrix test4 = S21Matrix(50, 20);

  EXPECT_ANY_THROW(test3.Determinant());
  EXPECT_ANY_THROW(test4.Determinant());
}

TEST(INVERSE, NOERR) {
  S21Matrix test = S21Matrix(3, 3);

  test(0, 0) = 4;
  test(0, 1) = -2;
  test(0, 2) = 1;
  test(1, 0) = 1;
  test(1, 1) = 6;
  test(1, 2) = -2;
  test(2, 0) = 1;
  test(2, 1) = 0;
  test(2, 2) = 0;

  std::cout << "origin matrix: inverse\n";
  test.PrintMatrix();

  S21Matrix result = test.InverseMatrix();

  std::cout << "result matrix: inverse\n";
  result.PrintMatrix();

  EXPECT_DOUBLE_EQ(result(0, 1), 0);
  EXPECT_DOUBLE_EQ(result(0, 2), 1);
  EXPECT_DOUBLE_EQ(result(1, 0), 1);
  EXPECT_DOUBLE_EQ(result(2, 0), 3);
  EXPECT_DOUBLE_EQ(result(2, 1), 1);
  EXPECT_DOUBLE_EQ(result(2, 2), -13);
}

TEST(INVERSE, ERR) {
  S21Matrix test1 = S21Matrix(-1, -1);
  S21Matrix test2 = S21Matrix(0, 0);

  EXPECT_ANY_THROW(test1.InverseMatrix());
  EXPECT_ANY_THROW(test2.InverseMatrix());

  S21Matrix test3 = S21Matrix(2, 3);
  S21Matrix test4 = S21Matrix(50, 20);

  EXPECT_ANY_THROW(test3.InverseMatrix());
  EXPECT_ANY_THROW(test4.InverseMatrix());

  S21Matrix test5 = S21Matrix(1, 1);
  EXPECT_ANY_THROW(test5.InverseMatrix());
}

TEST(CONSTANTS, NOERR) {
  S21Matrix origin1 = S21Matrix(3, 3);
  S21Matrix origin2 = S21Matrix(3, 3);
  origin1.SetMatrix(1);
  origin2.SetMatrix(2);

  const S21Matrix test1 = S21Matrix(origin1);
  const S21Matrix test2 = S21Matrix(origin2);
  const S21Matrix test3 = S21Matrix(3, 3);

  EXPECT_TRUE(test2(0, 0) == 2);
  EXPECT_ANY_THROW(test2(10, 10) == 0);

  S21Matrix result1 = test1 + test3;
  S21Matrix result2 = test2 - test3;
  S21Matrix result3 = test1 * test2;
  S21Matrix result4 = test2 * 1.0f;
  S21Matrix result5 = 0.0f * test1;

  EXPECT_TRUE(result1 == test1);
  EXPECT_TRUE(result2 == test2);
  EXPECT_TRUE(result3 == test2);
  EXPECT_TRUE(result4 == test2);
  EXPECT_TRUE(result5 == test3);
  EXPECT_FALSE(result4 != test2);

  S21Matrix result6 = S21Matrix(0, 0);
  result6 = test2;
  EXPECT_TRUE(result6 == test2);

  EXPECT_NO_THROW(test1.Transpose());
  EXPECT_NO_THROW(test1.CalcComplements());
  EXPECT_NO_THROW(test1.Determinant());
  EXPECT_ANY_THROW(test1.InverseMatrix());
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}