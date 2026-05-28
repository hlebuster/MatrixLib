#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <initializer_list>

/**
 * @brief Класс для работы с матрицами произвольного размера
 * 
 * @tparam T Тип элементов матрицы (int, double, float и т.д.)
 * 
 * Реализует основные операции линейной алгебры:
 * - Сложение, вычитание, умножение матриц
 * - Транспонирование, определитель (2x2, 3x3)
 * - LU-разложение, обратная матрица, решение систем Ax = b
 * 
 * @note В проекте не используется прямое управление памятью (new/delete),
 *       все данные хранятся в std::vector
 */
template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;  ///< Двумерный вектор для хранения элементов матрицы
    size_t rows;                        ///< Количество строк
    size_t cols;                        ///< Количество столбцов

public:
    /**
     * @brief Конструктор матрицы заданного размера
     * @param r Количество строк
     * @param c Количество столбцов
     * @throws std::invalid_argument Если r == 0 или c == 0
     * 
     * Пример:
     * @code
     * Matrix<int> M(3, 4);  // создаёт матрицу 3×4, заполненную нулями
     * @endcode
     */
    Matrix(size_t r = 1, size_t c = 1);
    
    /**
     * @brief Конструктор из initializer_list (удобное создание маленьких матриц)
     * @param list Список списков, например {{1,2},{3,4}}
     * @throws std::invalid_argument Если матрица пустая или строки разной длины
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{1, 2, 3}, {4, 5, 6}};
     * @endcode
     */
    Matrix(std::initializer_list<std::initializer_list<T>> list);
    
    /**
     * @brief Доступ к элементу матрицы с проверкой границ
     * @param i Номер строки (от 0 до rows-1)
     * @param j Номер столбца (от 0 до cols-1)
     * @return Ссылка на элемент (позволяет изменять значение)
     * @throws std::out_of_range При выходе за границы матрицы
     * 
     * Пример:
     * @code
     * Matrix<int> A(2, 2);
     * A.at(0, 0) = 5;  // установить значение
     * int x = A.at(0, 0);  // прочитать значение
     * @endcode
     */
    T& at(size_t i, size_t j);
    
    /**
     * @brief Константный доступ к элементу матрицы (только чтение)
     * @param i Номер строки (от 0 до rows-1)
     * @param j Номер столбца (от 0 до cols-1)
     * @return Константная ссылка на элемент
     * @throws std::out_of_range При выходе за границы матрицы
     * 
     * Используется для константных объектов матрицы.
     */
    const T& at(size_t i, size_t j) const;
    
    /**
     * @brief Получить количество строк
     * @return rows
     */
    size_t getRows() const { return rows; }
    
    /**
     * @brief Получить количество столбцов
     * @return cols
     */
    size_t getCols() const { return cols; }
    
    /**
     * @brief Транспонирование матрицы
     * @return Новая матрица, где строки исходной стали столбцами
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{1, 2, 3}, {4, 5, 6}};
     * Matrix<int> B = A.transpose();
     * // B = {{1, 4}, {2, 5}, {3, 6}}
     * @endcode
     */
    Matrix transpose() const;
    
    /**
     * @brief Вычисление определителя матрицы
     * @return Значение определителя
     * @throws std::logic_error Если матрица не квадратная или размер > 3
     * 
     * @note Реализовано только для матриц 2×2 и 3×3
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{4, 3}, {6, 3}};
     * int det = A.determinant();  // det = -6
     * @endcode
     */
    T determinant() const;
    
    /**
     * @brief Нахождение обратной матрицы
     * @return Обратная матрица A⁻¹
     * @throws std::logic_error Если матрица не квадратная
     * @throws std::runtime_error Если матрица вырожденная (det = 0)
     * 
     * Алгоритм: решаются n систем уравнений Ax = e_j,
     * где e_j - столбцы единичной матрицы.
     * 
     * @note Результат проверяется: A * A⁻¹ = E (единичная матрица)
     * 
     * Пример:
     * @code
     * Matrix<double> A = {{4, 3}, {6, 3}};
     * Matrix<double> inv = A.inverse();
     * // inv = {{-0.5, 0.5}, {1, -0.666667}}
     * @endcode
     */
    Matrix inverse() const;
    
    /**
     * @brief Решение системы линейных уравнений Ax = b
     * @param b Вектор правой части (размер должен быть равен rows)
     * @return Вектор x (решение системы)
     * @throws std::logic_error Если матрица не квадратная
     * @throws std::invalid_argument Если размер b не совпадает с rows
     * @throws std::runtime_error Если матрица вырожденная
     * 
     * Алгоритм: LU-разложение + прямая/обратная подстановка
     * 
     * Пример:
     * @code
     * Matrix<double> A = {{4, 3}, {6, 3}};
     * std::vector<double> b = {7, 9};
     * std::vector<double> x = A.solveLinear(b);  // x = [1, 1]
     * @endcode
     */
    std::vector<T> solveLinear(const std::vector<T>& b) const;
    
    /**
     * @brief Оператор сложения матриц
     * @param other Матрица для сложения
     * @return Новая матрица (сумма)
     * @throws std::invalid_argument Если размеры матриц не совпадают
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{1, 2}, {3, 4}};
     * Matrix<int> B = {{5, 6}, {7, 8}};
     * Matrix<int> C = A + B;  // C = {{6, 8}, {10, 12}}
     * @endcode
     */
    Matrix operator+(const Matrix& other) const;
    
    /**
     * @brief Оператор вычитания матриц
     * @param other Матрица для вычитания
     * @return Новая матрица (разность)
     * @throws std::invalid_argument Если размеры матриц не совпадают
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{5, 6}, {7, 8}};
     * Matrix<int> B = {{1, 2}, {3, 4}};
     * Matrix<int> C = A - B;  // C = {{4, 4}, {4, 4}}
     * @endcode
     */
    Matrix operator-(const Matrix& other) const;
    
    /**
     * @brief Оператор умножения матриц
     * @param other Матрица для умножения
     * @return Новая матрица (произведение)
     * @throws std::invalid_argument Если число столбцов A != числу строк B
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{1, 2}, {3, 4}};
     * Matrix<int> B = {{5, 6}, {7, 8}};
     * Matrix<int> C = A * B;  // C = {{19, 22}, {43, 50}}
     * @endcode
     */
    Matrix operator*(const Matrix& other) const;
    
    /**
     * @brief Оператор сравнения матриц на равенство
     * @param other Матрица для сравнения
     * @return true если все элементы равны, false в противном случае
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{1, 2}, {3, 4}};
     * Matrix<int> B = {{1, 2}, {3, 4}};
     * if (A == B) { ... }  // true
     * @endcode
     */
    bool operator==(const Matrix& other) const;
    
    /**
     * @brief Оператор вывода матрицы в поток
     * @param os Поток вывода (например, std::cout)
     * @param m Матрица для вывода
     * @return Ссылка на поток для цепочечного вызова
     * 
     * Формат вывода каждой строки: [элемент1 элемент2 ...]
     * 
     * Пример:
     * @code
     * Matrix<int> A = {{1, 2}, {3, 4}};
     * std::cout << A;
     * // Вывод:
     * // [1 2]
     * // [3 4]
     * @endcode
     */
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
    /**
     * @brief Структура для хранения результатов LU-разложения
     * 
     * LU-разложение: P * A = L * U
     * - L: нижняя треугольная матрица (на диагонали единицы)
     * - U: верхняя треугольная матрица
     * - P: вектор перестановок (для численной устойчивости)
     */
    struct LUResult {
        Matrix L;               ///< Нижняя треугольная матрица (единицы на диагонали)
        Matrix U;               ///< Верхняя треугольная матрица
        std::vector<size_t> P;  ///< Матрица перестановок (PA = LU)
    };
    
    /**
     * @brief LU-разложение матрицы с частичным выбором главного элемента
     * @return Структура LUResult, содержащая L, U и P
     * @throws std::logic_error Если матрица не квадратная
     * @throws std::runtime_error Если матрица вырожденная (главный элемент близок к нулю)
     * 
     * Алгоритм:
     * 1. Копируем исходную матрицу A
     * 2. Для каждого столбца k:
     *    - Находим максимальный по модулю элемент в столбце k (начиная со строки k)
     *    - Меняем строки местами (запоминаем перестановку в P)
     *    - Вычисляем множители для строк ниже k
     *    - Обновляем оставшуюся часть матрицы
     * 3. Извлекаем L и U из полученной матрицы
     * 
     * @note L имеет единицы на диагонали, множители хранятся под диагональю
     * @note U имеет нули под диагональю
     */
    LUResult luDecompose() const;
};

// ====================================================================
//                    РЕАЛИЗАЦИЯ МЕТОДОВ
// ====================================================================

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

#endif  // MATRIX_H