#include "Matrix.h"
#include <vector>

Matrix::Matrix(std::vector<std::vector<double>>& data) 
    : m_data(data), m_size(data.size()) {
}

Matrix::Matrix(std::vector<std::vector<double>>&& data) 
    : m_data(std::move(data)), m_size(data.size()) {
}
