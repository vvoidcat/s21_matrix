# s21_matrix

A School 21 educational project - an implementation of a custom library for working with matrices in C.

The library supports the following operations:
  1) **create**
  2) **delete**
  3) **compare** (equality)
  4) **add**
  5) **substract**
  6) **multiply** (numbers)
  7) **multiply** (matrices)
  8) **transpose**
  9) **calculate complements**
  10) **calculate the determinant**
  11) **inverse**
  
 ## Note:
 - the matrix is a ***matrix_t*** structure containing a pointer to a ***2-dimensional array*** of doubles, int ***size_Y***, int ***size_X***;
 - the library contains a number of additional helper functions written primarily for testing purposes;
 - the library has been tested with valgrind (**no leaks**);
 - unit tests cover **more than 90%** of each function;
 - (!) memory allocation happens INSIDE the creation method, so allocationg memory for a matrix before calling the creation method will cause a memory leak;
 - (!) resizing matrices is unsupported, and manually changing the size of a matrix that's already been allocated is unadvised as it will cause a memory leak as well.
