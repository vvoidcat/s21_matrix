# s21_matrix - version-cpp

The library supports the following operations:
  1) **compare** (equality)
  2) **add**
  3) **substract**
  4) **multiply** (numbers)
  5) **multiply** (matrices)
  6) **transpose**
  7) **calculate complements**
  8) **calculate the determinant**
  9) **inverse**

Aside from those, the library has implementations of multiple **constructors** (basic, parametrized, copy, move) and a **destructor**, various **getter** and **setter methods**, **operator overloads**.

## Note:
 - the matrix is implemented as a **S21Matrix** class containing a pointer to a ***2-dimensional array*** of doubles, int ***rows_*** (size Y), int ***cols_*** (size X) as private class fields;
 - the library contains a number of additional helper functions written primarily for testing and ease-of-use purposes;
 - some the library functions use exception throwing;
 - the library supports matrix resizing after the **S21Matrix** object instantialization;
 - the library has been tested with valgrind (**no leaks**);
 - unit tests cover **100%** of the library source file.
