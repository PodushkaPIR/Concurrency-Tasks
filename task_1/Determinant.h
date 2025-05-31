#pragma once

#include "Matrix.h"

long double calculate_determinant(const Matrix& matrix, int depth);
Matrix create_minor(const Matrix& matrix, int row_start, int col_start);
