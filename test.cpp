#include <gtest/gtest.h>
#include "Matrix.h"

TEST(MatrixTest, Addition) {
    Matrix<int> A = {{1, 2}, {3, 4}};
    Matrix<int> B = {{5, 6}, {7, 8}};
    Matrix<int> expected = {{6, 8}, {10, 12}};
    EXPECT_TRUE((A + B) == expected);
}

TEST(MatrixTest, Multiplication) {
    Matrix<int> A = {{1, 2}, {3, 4}};
    Matrix<int> B = {{5, 6}, {7, 8}};
    Matrix<int> expected = {{19, 22}, {43, 50}};
    EXPECT_TRUE((A * B) == expected);
}

TEST(MatrixTest, Determinant2x2) {
    Matrix<int> A = {{4, 3}, {6, 3}};
    EXPECT_EQ(A.determinant(), -6);
}

TEST(MatrixTest, Transpose) {
    Matrix<int> A = {{1, 2, 3}, {4, 5, 6}};
    Matrix<int> expected = {{1, 4}, {2, 5}, {3, 6}};
    EXPECT_TRUE(A.transpose() == expected);
}

TEST(MatrixTest, Inverse) {
    Matrix<double> A = {{4, 3}, {6, 3}};
    Matrix<double> inv = A.inverse();
    Matrix<double> I = {{1, 0}, {0, 1}};
    Matrix<double> product = A * inv;
    EXPECT_NEAR(product.at(0,0), 1.0, 1e-9);
    EXPECT_NEAR(product.at(0,1), 0.0, 1e-9);
    EXPECT_NEAR(product.at(1,0), 0.0, 1e-9);
    EXPECT_NEAR(product.at(1,1), 1.0, 1e-9);
}

TEST(MatrixTest, SolveLinear) {
    Matrix<double> A = {{4, 3}, {6, 3}};
    std::vector<double> b = {7, 9};
    std::vector<double> x = A.solveLinear(b);
    EXPECT_NEAR(x[0], 1.0, 1e-9);
    EXPECT_NEAR(x[1], 1.0, 1e-9);
}

TEST(MatrixTest, SingularMatrixThrows) {
    Matrix<double> A = {{1, 2}, {2, 4}};  
    EXPECT_THROW(A.inverse(), std::runtime_error);
}

TEST(MatrixTest, Inverse3x3) {
    Matrix<double> A = {{2, 1, 1},
                        {1, 3, 2},
                        {1, 2, 4}};
    
    Matrix<double> inv = A.inverse();
    
    Matrix<double> product = A * inv;
    
    EXPECT_NEAR(product.at(0,0), 1.0, 1e-9);
    EXPECT_NEAR(product.at(0,1), 0.0, 1e-9);
    EXPECT_NEAR(product.at(0,2), 0.0, 1e-9);
    
    EXPECT_NEAR(product.at(1,0), 0.0, 1e-9);
    EXPECT_NEAR(product.at(1,1), 1.0, 1e-9);
    EXPECT_NEAR(product.at(1,2), 0.0, 1e-9);
    
    EXPECT_NEAR(product.at(2,0), 0.0, 1e-9);
    EXPECT_NEAR(product.at(2,1), 0.0, 1e-9);
    EXPECT_NEAR(product.at(2,2), 1.0, 1e-9);
}