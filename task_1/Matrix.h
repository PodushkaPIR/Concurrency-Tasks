#pragma once

#include <vector>

class Matrix {
public:
    explicit Matrix(std::vector<std::vector<double>>& data);
    explicit Matrix(std::vector<std::vector<double>>&& data);
    const std::vector<double>& operator[](size_t index) const { return m_data[index]; }
    std::vector<double>& operator[](size_t index) { return m_data[index]; }
    ~Matrix() = default;

    size_t size() const { return m_data.size(); }

private:
    std::vector<std::vector<double>> m_data;
    size_t m_size;
};
