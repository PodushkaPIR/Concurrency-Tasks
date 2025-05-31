#include "Matrix.h"
#include "Determinant.h"
#include <iostream>
#include <chrono>

void benchmark_test(Matrix& matrix, int depth){
    int ex_time = 0;

    auto start = std::chrono::system_clock::now();
    long double result = calculate_determinant(matrix, depth);
    ex_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now() - start).count();

    std::cout << "-----------------------------------------\n";
    std::cout << "Matrix size: " << matrix.size() << "\n";
    std::cout << "Determinant = " << result << "\n";
    std::cout << "Deep of concurency: " << depth << "\n";
    std::cout << "Execution time: " << ex_time << " mks\n";
    std::cout << "-----------------------------------------\n";
}

void benchmark_test(Matrix& matrix, int depth);

void test1() {
    Matrix m({{5}});
    benchmark_test(m, 0);
}

void test2() {
    Matrix m({
        {1, 2},
        {3, 4}
    });
    benchmark_test(m, 0);
    benchmark_test(m, 1);
}

void test3() {
    Matrix m({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 10}
    });
    benchmark_test(m, 0);
    benchmark_test(m, 1);
}

void test4() {
    Matrix m({
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    });
    benchmark_test(m, 0);
    benchmark_test(m, 1);
}

void test5() {
    Matrix m({
        {2, 1, 3, 4, 5},
        {1, 2, 0, 1, 2},
        {3, 0, 1, 2, 1},
        {4, 1, 2, 3, 4},
        {5, 2, 1, 4, 3}
    });
    benchmark_test(m, 0);
    benchmark_test(m, 1);
}

void test7() {
    Matrix m({{21 ,-9 ,34 ,35 ,31 ,20 ,-4},
              {18 ,-4 ,38 ,20 ,16 ,43 ,40},
              {18 ,14 , 7 ,37 ,38 ,30 ,-6},
              {27 , 5 ,-5 ,-3 ,47 ,49 ,-1},
              {5 ,21 ,43 ,-6 ,25 ,11 ,35},
              {42 ,50 ,13 ,-1 ,37 ,49 ,11},
              {32 ,-5 ,-5 ,20 ,29 ,-6 ,29}
    });
    benchmark_test(m, 0);
    benchmark_test(m, 1);
    benchmark_test(m, 2);
}

void test10() {
    Matrix m({{21, -9 ,34 ,35 ,31 ,20 ,-4, 0, 6, 1},
              {18 ,-4 ,38 ,20, 10, 4, -5, 16 ,43 ,40},
              {18 ,14 , 7 ,37 ,38 ,30 ,-6, 8, 2, -4},
              {27 , 5 ,-5 ,-3 ,47, 3, 1, 10 ,49 ,-1},
              {5 ,21 ,43 ,-6 ,25 ,11 ,35, 10, 2, 3},
              {42 ,50 ,13 ,-1, 9, -4, 4, 37 ,49 ,11},
              {32 ,-5, 2, 1, 5, -5 ,20 ,29 ,-6 ,29},
              {3, 10, 12, 4, -5, -20, 3, 10, 9, -3},
              {4, 1, 5, 10, 2, -4, 3, 32, 11, -53},
              {5, 3, 2, 0, -42, 23, 12, 33, 22, 10}
    });
    benchmark_test(m, 0);
    benchmark_test(m, 1);
    benchmark_test(m, 2);
    benchmark_test(m, 3);
}

void run_all_tests() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test7();
    test10();
}
