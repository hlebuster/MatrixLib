#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <initializer_list>

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows, cols;

public:
    // Конструкторы
    Matrix(size_t r = 1, size_t c = 1);
    Matrix(std::initializer_list<std::initializer_list<T>> list);
    
    // Доступ к элементам
    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    
    // Основные операции
    Matrix transpose() const;
    T determinant() const;
    Matrix inverse() const;
    std::vector<T> solveLinear(const std::vector<T>& b) const;
    
    // Перегрузка операторов
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    bool operator==(const Matrix& other) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (size_t i = 0; i < m.rows; ++i) {
            os << "[";
            for (size_t j = 0; j < m.cols; ++j) {
                os << m.data[i][j];
                if (j < m.cols - 1) os << " ";
            }
            os << "]\n";
        }
        return os;
    }
    
private:
    struct LUResult {
        Matrix L, U;
        std::vector<size_t> P;
    };
    LUResult luDecompose() const;
};

// ========== РЕАЛИЗАЦИЯ МЕТОДОВ ==========

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c) : rows(r), cols(c) {
    if (r == 0 || c == 0) throw std::invalid_argument("Zero dimension");
    data.resize(rows, std::vector<T>(cols, T(0)));
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list) {
    rows = list.size();
    if (rows == 0) throw std::invalid_argument("Empty matrix");
    
    cols = list.begin()->size();
    data.resize(rows, std::vector<T>(cols));
    
    size_t i = 0;
    for (const auto& row : list) {
        if (row.size() != cols) throw std::invalid_argument("Inconsistent row size");
        size_t j = 0;
        for (const T& val : row) {
            data[i][j++] = val;
        }
        i++;
    }
}

template<typename T>
T& Matrix<T>::at(size_t i, size_t j) {
    if (i >= rows || j >= cols) throw std::out_of_range("Index out of bounds");
    return data[i][j];
}

template<typename T>
const T& Matrix<T>::at(size_t i, size_t j) const {
    if (i >= rows || j >= cols) throw std::out_of_range("Index out of bounds");
    return data[i][j];
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix result(cols, rows);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.data[j][i] = data[i][j];
    return result;
}

template<typename T>
T Matrix<T>::determinant() const {
    if (rows != cols) throw std::logic_error("Determinant requires square matrix");
    
    if (rows == 2) {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
    else if (rows == 3) {
        return data[0][0] * data[1][1] * data[2][2]
             + data[0][1] * data[1][2] * data[2][0]
             + data[0][2] * data[1][0] * data[2][1]
             - data[0][2] * data[1][1] * data[2][0]
             - data[0][1] * data[1][0] * data[2][2]
             - data[0][0] * data[1][2] * data[2][1];
    }
    else {
        throw std::logic_error("Determinant implemented only for 2x2 and 3x3");
    }
}

template<typename T>
typename Matrix<T>::LUResult Matrix<T>::luDecompose() const {
    if (rows != cols) throw std::logic_error("LU decomposition requires square matrix");
    
    size_t n = rows;
    Matrix L(n, n);
    Matrix U(n, n);
    std::vector<size_t> P(n);
    
    std::vector<std::vector<T>> A = data;
    
    for (size_t i = 0; i < n; ++i) P[i] = i;
    
    for (size_t k = 0; k < n; ++k) {
        T maxVal = std::abs(A[k][k]);
        size_t maxRow = k;
        for (size_t i = k + 1; i < n; ++i) {
            if (std::abs(A[i][k]) > maxVal) {
                maxVal = std::abs(A[i][k]);
                maxRow = i;
            }
        }
        
        if (maxVal < T(1e-10)) {
            throw std::runtime_error("Matrix is singular, cannot decompose");
        }
        
        if (maxRow != k) {
            std::swap(A[k], A[maxRow]);
            std::swap(P[k], P[maxRow]);
        }
        
        for (size_t i = k + 1; i < n; ++i) {
            T factor = A[i][k] / A[k][k];
            A[i][k] = factor;
            for (size_t j = k + 1; j < n; ++j) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j) {
                L.data[i][j] = T(1);
                U.data[i][j] = A[i][j];
            } else if (i > j) {
                L.data[i][j] = A[i][j];
                U.data[i][j] = T(0);
            } else {
                L.data[i][j] = T(0);
                U.data[i][j] = A[i][j];
            }
        }
    }
    
    return {L, U, P};
}

template<typename T>
std::vector<T> Matrix<T>::solveLinear(const std::vector<T>& b) const {
    if (rows != cols) throw std::logic_error("Can only solve square systems");
    if (b.size() != rows) throw std::invalid_argument("Vector size mismatch");
    
    LUResult lu = luDecompose();
    size_t n = rows;
    
    std::vector<T> pb(n);
    for (size_t i = 0; i < n; ++i) {
        pb[i] = b[lu.P[i]];
    }
    
    std::vector<T> y(n);
    for (size_t i = 0; i < n; ++i) {
        T sum = T(0);
        for (size_t j = 0; j < i; ++j) {
            sum += lu.L.at(i, j) * y[j];
        }
        y[i] = pb[i] - sum;
    }
    
    std::vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        T sum = T(0);
        for (size_t j = i + 1; j < n; ++j) {
            sum += lu.U.at(i, j) * x[j];
        }
        x[i] = (y[i] - sum) / lu.U.at(i, i);
    }
    
    return x;
}

template<typename T>
Matrix<T> Matrix<T>::inverse() const {
    if (rows != cols) throw std::logic_error("Inverse requires square matrix");
    
    size_t n = rows;
    Matrix result(n, n);
    
    for (size_t j = 0; j < n; ++j) {
        std::vector<T> e(n, T(0));
        e[j] = T(1);
        std::vector<T> x = solveLinear(e);
        for (size_t i = 0; i < n; ++i) {
            result.data[i][j] = x[i];
        }
    }
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Dimension mismatch");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.data[i][j] = data[i][j] + other.data[i][j];
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Dimension mismatch");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.data[i][j] = data[i][j] - other.data[i][j];
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix multiplication dimension mismatch");
    }
    Matrix result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < other.cols; ++j)
            for (size_t k = 0; k < cols; ++k)
                result.data[i][j] += data[i][k] * other.data[k][j];
    return result;
}

template<typename T>
bool Matrix<T>::operator==(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) return false;
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            if (data[i][j] != other.data[i][j]) return false;
    return true;
}

#endif