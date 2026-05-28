#include "Matrix.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== MATRIX LIBRARY DEMO ===\n\n";
        
        // Создаём матрицы
        Matrix<double> A = {{4, 3}, {6, 3}};
        Matrix<double> B = {{1, 2}, {3, 4}};
        
        std::cout << "Matrix A:\n" << A;
        std::cout << "Matrix B:\n" << B;
        
        // Сложение
        std::cout << "A + B:\n" << (A + B);
        
        // Умножение
        std::cout << "A * B:\n" << (A * B);
        
        // Определитель
        std::cout << "det(A) = " << A.determinant() << "\n";
        
        // Транспонирование
        std::cout << "A transposed:\n" << A.transpose();
        
        // Обратная матрица
        Matrix<double> invA = A.inverse();
        std::cout << "Inverse of A:\n" << invA;
        
        // Проверка: A * invA должна быть единичной
        std::cout << "A * invA (should be identity):\n" << (A * invA);
        
        // Решение системы Ax = b
        std::vector<double> b = {7, 9};
        std::vector<double> x = A.solveLinear(b);
        std::cout << "Solution of Ax = b: x = [" << x[0] << ", " << x[1] << "]\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}