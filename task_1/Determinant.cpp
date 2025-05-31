#include "Determinant.h"

#include "Matrix.h"

#include <vector>
#include <stdexcept>
#include <thread>

long double calculate_determinant(const Matrix& matrix, int depth) {
    const size_t size = matrix.size();
    
    if (size == 0) throw std::invalid_argument("Empty matrix");
    if (size == 1) return matrix[0][0];

    if (depth > 0) {
        std::vector<std::thread> threads;
        std::vector<long double> result_part(size);
        
        for (size_t i = 0; i < size; ++i) {
            threads.emplace_back(
                [i, &matrix, &result_part, depth]() {
                    Matrix minor = create_minor(matrix, 0, i);
                    result_part[i] = calculate_determinant(minor, depth - 1);
                }
            );
        }

        for (auto& t : threads) {
            t.join();
        }

        double result = 0;
        for (size_t i = 0; i < size; ++i) {
            result += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * result_part[i];
        }
        return result;
    }

    else {
        long double result = 0;
        for (size_t i = 0; i < size; ++i) {
            Matrix minor = create_minor(matrix, 0, i);
            long double minor_det = calculate_determinant(minor, depth - 1);
            result += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * minor_det;
        }
        return result;
    }
}

Matrix create_minor(const Matrix& matrix, int start_row, int start_col) {
    const size_t size = matrix.size();
    if (size <= 1) {
        throw std::invalid_argument("Bro, you can't create matrix 1x1");
    }

    std::vector<std::vector<double>> minor_data;
    minor_data.reserve(size - 1);

    for (int i = 0; i < size; ++i) {
        if (i == start_row) continue;

        const auto& source_row = matrix[i];
        std::vector<double> row;
        row.reserve(size - 1);

        std::copy(source_row.begin(), source_row.begin() + start_col, std::back_inserter(row));
        std::copy(source_row.begin() + start_col + 1, source_row.end(), std::back_inserter(row));
        minor_data.emplace_back(std::move(row));
    }

    return Matrix(std::move(minor_data));
}
