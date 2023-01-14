#include "s21_matrix_oop.h"

// constructors, destructor

S21Matrix::S21Matrix() noexcept { InitMatrix(); }

S21Matrix::S21Matrix(int rows, int cols) {
  CreateMatrix(rows, cols);
  SetMatrix(0.0f);
}

S21Matrix::S21Matrix(const S21Matrix& other) {
  CreateMatrix(other.cols_, other.rows_);
  SetMatrix(other);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept {
  cols_ = other.cols_;
  rows_ = other.rows_;
  matrix_ = other.matrix_;
  other.InitMatrix();
}

S21Matrix::~S21Matrix() { DeleteMatrix(); }

// operators

double& S21Matrix::operator()(int i, int j) {
  if (!Contains(i, j))
    throw std::out_of_range("S21Matrix: index out of range exception");

  return matrix_[i][j];
}

const double& S21Matrix::operator()(int i, int j) const {
  if (!Contains(i, j))
    throw std::out_of_range("S21Matrix: index out of range exception");

  return matrix_[i][j];
}

bool S21Matrix::operator==(const S21Matrix& other) const noexcept {
  return EqMatrix(other);
}

bool S21Matrix::operator!=(const S21Matrix& other) const noexcept {
  return !EqMatrix(other);
}

S21Matrix S21Matrix::operator=(const S21Matrix& other) {
  CopyMatrix(other, other.rows_, other.cols_);
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix result = S21Matrix(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix result = S21Matrix(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix result = S21Matrix(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(double num) const {
  S21Matrix result = S21Matrix(*this);
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(const double num, const S21Matrix& other) {
  S21Matrix result(other);
  return result * num;
}

S21Matrix S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(double num) {
  MulNumber(num);
  return *this;
}

// public functions (lib)

bool S21Matrix::EqMatrix(const S21Matrix& other) const noexcept {
  bool result = true;

  if (IsEqualSize(other)) {
    if (!IsNullOrEmpty() && !other.IsNullOrEmpty()) {
      for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
          if (fabs(matrix_[i][j] - other.matrix_[i][j]) > EPS) {
            result = false;
            break;
          }
        }
      }
    }
  } else {
    result = false;
  }
  return result;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (IsNullOrEmpty() || other.IsNullOrEmpty())
    throw std::invalid_argument("S21Matrix::SumMatrix: null matrix exception");

  if (!IsEqualSize(other))
    throw std::invalid_argument(
        "S21Matrix::SumMatrix: different matrix dimensions exception");

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (IsNullOrEmpty() || other.IsNullOrEmpty())
    throw std::invalid_argument("S21Matrix::SubMatrix: null matrix exception");

  if (!IsEqualSize(other))
    throw std::invalid_argument(
        "S21Matrix::SubMatrix: different matrix dimensions exception");

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  if (IsNullOrEmpty())
    throw std::invalid_argument("S21Matrix::MulNumber: null matrix exception");

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (IsNullOrEmpty() || other.IsNullOrEmpty())
    throw std::invalid_argument("S21Matrix::MulMatrix: null matrix exception");

  if (!IsEqualSize(other))
    throw std::invalid_argument(
        "S21Matrix::MulMatrix: different matrix dimensions exception");

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= other.matrix_[i][j];
    }
  }
}

S21Matrix S21Matrix::Transpose() const {
  if (IsNullOrEmpty())
    throw std::invalid_argument("S21Matrix::Transpose: null matrix exception");

  S21Matrix result = S21Matrix(cols_, rows_);

  for (int i = 0; i < result.rows_; i++) {
    for (int j = 0; j < result.cols_; j++) {
      result.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (IsNullOrEmpty())
    throw std::invalid_argument(
        "S21Matrix::CalcComplements: null matrix exception");

  if (!IsSquare())
    throw std::invalid_argument(
        "S21Matrix::CalcComplements: matrix is not square exception");

  S21Matrix result = S21Matrix(*this);

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      S21Matrix buffer = CalcMinorElements(*this, i, j);
      double determinant = buffer.Determinant();
      result.matrix_[i][j] = pow(-1, i + j) * determinant;
    }
  }
  return result;
}

double S21Matrix::Determinant() const {
  if (IsNullOrEmpty())
    throw std::invalid_argument(
        "S21Matrix::Determinant: null matrix exception");

  if (!IsSquare())
    throw std::invalid_argument(
        "S21Matrix::Determinant: matrix is not square exception");

  double result = 0.0f;

  if (rows_ == 1) {
    result = matrix_[0][0];
  } else if (rows_ == 2) {
    result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    int sign = 1;

    for (int i = 0; i < cols_; i++) {
      S21Matrix buffer = CalcMinorElements(*this, 0, i);
      double minor_result = buffer.Determinant();
      result += matrix_[0][i] * minor_result * sign;
      sign = -1 * sign;
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() const {
  if (IsNullOrEmpty())
    throw std::invalid_argument(
        "S21Matrix::InverseMatrix: null matrix exception");

  if (!IsSquare())
    throw std::invalid_argument(
        "S21Matrix::InverseMatrix: matrix is not square exception");

  double determinant = Determinant();

  if (fabs(determinant) <= EPS)
    throw std::invalid_argument(
        "S21Matrix::InverseMatrix: zero determinant exception");

  S21Matrix mComplements = CalcComplements();
  S21Matrix mTransposed = mComplements.Transpose();
  mTransposed.MulNumber(1.0f / determinant);
  return mTransposed;
}

// getters, setters

int S21Matrix::GetRowsCount() const noexcept { return rows_; }

int S21Matrix::GetColsCount() const noexcept { return cols_; }

double** S21Matrix::GetMatrix() const noexcept { return matrix_; }

double S21Matrix::GetElementAtIndex(int indexRows, int indexCols) const {
  if (!Contains(indexRows, indexCols))
    throw std::out_of_range(
        "S21Matrix::GetElementAtIndex: index out of range exception");

  return matrix_[indexRows][indexCols];
}

void S21Matrix::SetRowsCount(int newValue) {
  if (rows_ != newValue) {
    S21Matrix copy = S21Matrix(*this);
    CopyMatrix(copy, newValue, cols_);
  }
}

void S21Matrix::SetColsCount(int newValue) {
  if (cols_ != newValue) {
    S21Matrix copy = S21Matrix(*this);
    CopyMatrix(copy, rows_, newValue);
  }
}

void S21Matrix::SetMatrix(double value) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = value;
    }
  }
}

void S21Matrix::SetMatrix(double valueMin, double valueIncrement) {
  double value = valueMin;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (matrix_[i] != nullptr) {
        matrix_[i][j] = value;
        value += valueIncrement;
      }
    }
  }
}

void S21Matrix::SetMatrix(const S21Matrix& matrix) {
  SetMatrix(0.0f);

  for (int i = 0; i < matrix.rows_ && i < rows_; i++) {
    for (int j = 0; j < matrix.cols_ && j < cols_; j++) {
      matrix_[i][j] = matrix.matrix_[i][j];
    }
  }
}

void S21Matrix::SetElementAtIndex(int indexRows, int indexCols, double value) {
  if (!Contains(indexRows, indexCols))
    throw std::out_of_range(
        "S21Matrix::SetElementAtIndex: index out of range exception");

  matrix_[indexRows][indexCols] = value;
}

// public functions (helpers)

void S21Matrix::PrintMatrix() const noexcept {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      std::cout << matrix_[i][j] << " ";
    }
    std::cout << "\n";
  }
}

bool S21Matrix::Contains(int indexRows, int indexCols) const noexcept {
  return (!IsNullOrEmpty() && (indexRows >= 0 && indexRows < rows_) &&
          (indexCols >= 0 && indexCols < cols_))
             ? true
             : false;
}

bool S21Matrix::IsSquare() const noexcept {
  return (rows_ == cols_ && rows_ > 0) ? true : false;
}

bool S21Matrix::IsNullOrEmpty() const noexcept {
  return (matrix_ == nullptr || rows_ < 1 || cols_ < 1) ? true : false;
}

bool S21Matrix::IsEqualSize(const S21Matrix& other) const noexcept {
  return (rows_ == other.rows_ && cols_ == other.cols_) ? true : false;
}

// private functions (helpers)

void S21Matrix::InitMatrix() noexcept {
  cols_ = 0;
  rows_ = 0;
  matrix_ = nullptr;
}

void S21Matrix::CreateMatrix(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;
  AllocateMatrix();
}

void S21Matrix::AllocateMatrix() {
  if (rows_ > 0 && cols_ > 0) {
    matrix_ = new double*[rows_];

    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new double[cols_];
    }
  } else {
    matrix_ = nullptr;
  }
}

void S21Matrix::DeleteMatrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      if (matrix_[i] != nullptr) {
        delete[] matrix_[i];
      }
    }
    delete[] matrix_;
    InitMatrix();
  }
}

void S21Matrix::CopyMatrix(const S21Matrix& copy, int newRows, int newCols) {
  DeleteMatrix();
  CreateMatrix(newRows, newCols);
  SetMatrix(copy);
}

S21Matrix S21Matrix::CalcMinorElements(const S21Matrix& origin, int indexRows,
                                       int indexCols) const {
  S21Matrix result;

  if (!origin.IsNullOrEmpty() && origin.IsSquare()) {
    int elemRows = 0;
    int elemCols = 0;
    result = S21Matrix(origin.rows_ - 1, origin.cols_ - 1);

    if (!result.IsNullOrEmpty()) {
      for (int i = 0; i < origin.rows_; i++) {
        for (int j = 0; j < origin.cols_; j++) {
          if (i != indexRows && j != indexCols) {
            result.matrix_[elemRows][elemCols] = origin.matrix_[i][j];
            elemCols++;
          }
        }
        if (i != indexRows) {
          elemRows++;
        }
        elemCols = 0;
      }
    }
  }
  return result;
}
