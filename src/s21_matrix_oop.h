#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#include <math.h>

#include <exception>
#include <iostream>

class S21Matrix {
#define EPS 1e-7

 public:
  S21Matrix() noexcept;
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  double& operator()(int i, int j);
  const double& operator()(int i, int j) const;
  bool operator==(const S21Matrix& other) const noexcept;
  bool operator!=(const S21Matrix& other) const noexcept;
  S21Matrix operator=(const S21Matrix& other);

  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(double num) const;

  S21Matrix operator+=(const S21Matrix& other);
  S21Matrix operator-=(const S21Matrix& other);
  S21Matrix operator*=(const S21Matrix& other);
  S21Matrix operator*=(double num);

  bool EqMatrix(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  int GetRowsCount() const noexcept;
  int GetColsCount() const noexcept;
  double** GetMatrix() const noexcept;
  double GetElementAtIndex(int indexRows, int indexCols) const;

  void SetRowsCount(int newValue);
  void SetColsCount(int newValue);
  void SetMatrix(double value);
  void SetMatrix(double valueMin, double valueIncrement);
  void SetMatrix(const S21Matrix& matrix);
  void SetElementAtIndex(int indexRows, int indexCols, double value);

  void PrintMatrix() const noexcept;

  bool Contains(int indexRows, int indexCols) const noexcept;
  bool IsSquare() const noexcept;
  bool IsEqualSize(const S21Matrix& other) const noexcept;
  bool IsNullOrEmpty() const noexcept;

 private:
  void InitMatrix() noexcept;
  void CreateMatrix(int rows, int cols);
  void AllocateMatrix();
  void DeleteMatrix();
  void CopyMatrix(const S21Matrix& copy, int newRows, int newCols);
  S21Matrix CalcMinorElements(const S21Matrix& origin, int indexRows,
                              int indexCols) const;

  int rows_;
  int cols_;
  double** matrix_;
};

S21Matrix operator*(const double num, const S21Matrix& other);

#endif  // SRC_S21_MATRIX_OOP_H_