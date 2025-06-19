#include <string>
#include <functional>
#include <iostream>
#include <complex>
#include <cassert>

#include "RectangularMatrix.h"
#include "Student.h"
#include "Teacher.h"
#include "Person.h"
#include "utils.h"

#include <cmath>


using cd = std::complex<double>;
bool approxEqual(std::complex<double> a, std::complex<double> b, double eps = 1e-9) {
    return std::abs(a.real() - b.real()) < eps && std::abs(a.imag() - b.imag()) < eps;
}

template <typename T>
void testBasicMatrix(const T& a, const T& b, const std::string& label) {
    std::cout << "\n--- Testing RectangularMatrix<" << label << "> ---\n";

    RectangularMatrix<T> A(2, 2);
    A.at(0, 0) = a;
    A.at(0, 1) = b;
    A.at(1, 0) = a;
    A.at(1, 1) = b;

    // Копирование
    RectangularMatrix<T> B = A;
    assert(B == A);

    // Присваивание
    RectangularMatrix<T> C(2, 2);
    C = A;
    assert(C == A);

    // Перестановки
    A.swapRows(0, 1);
    A.swapCols(0, 1);
    std::cout << "After swaps:\n";
    A.print(std::cout);

    // zip + unzip
    auto zipped = zip(A, C);
    std::cout << "Zipped matrix:\n";
    zipped.print(std::cout);

    auto [AZ, CZ] = RectangularMatrix<T>::unzip(zipped);
    assert(AZ == A);
    assert(CZ == C);

    std::cout << "✅ Passed tests for type: " << label << "\n";
}
void testStringMatrix() {
    std::cout << "\n--- Testing RectangularMatrix<std::string> ---\n";

    RectangularMatrix<std::string> A(2, 2);
    A.at(0, 0) = "Alice";
    A.at(0, 1) = "Bob";
    A.at(1, 0) = "Carol";
    A.at(1, 1) = "Dave";

    RectangularMatrix<std::string> B = A;
    assert(A == B);

    A.swapRows(0, 1);
    A.swapCols(0, 1);
    std::cout << "Swapped string matrix:\n";
    A.print(std::cout);

    RectangularMatrix<std::string> C = A;
    auto zipped = zip(A, C);
    zipped.print(std::cout);

    auto [AZ, CZ] = RectangularMatrix<std::string>::unzip(zipped);
    assert(AZ == A);
    assert(CZ == C);

    std::cout << "✅ String matrix passed.\n";
}
void testFunctionMatrix() {
    std::cout << "\n--- Testing RectangularMatrix<std::function<int()>> ---\n";

    using F = std::function<int()>;
    RectangularMatrix<F> A(2, 2);

    A.at(0, 0) = []() { return 1; };
    A.at(0, 1) = []() { return 2; };
    A.at(1, 0) = []() { return 3; };
    A.at(1, 1) = []() { return 4; };

    // Проверка через вызов
    assert(A.at(0,0)() == 1);
    assert(A.at(1,1)() == 4);

    // Копия
    RectangularMatrix<F> B = A;
    assert(B.at(1, 0)() == 3);

    // zip / unzip
    auto zipped = zip(A, B);
    auto [AZ, BZ] = RectangularMatrix<F>::unzip(zipped);
    assert(AZ.at(0, 1)() == 2);
    assert(BZ.at(1, 0)() == 3);

    std::cout << "✅ Function matrix passed.\n";
}
void testArithmeticOps() {
    
    std::cout << "\n--- Arithmetic Operations Test (int) ---\n";
    RectangularMatrix<int> A(2, 2);
    A.at(0,0) = 1; A.at(0,1) = 2;
    A.at(1,0) = 3; A.at(1,1) = 4;
    RectangularMatrix<int> B(2, 2);
    B.at(0,0) = 4; B.at(0,1) = 6;
    B.at(1,0) = 5; B.at(1,1) = 7;
    // split
    auto [top, bottom] = A.splitRows(1);
    assert(top.getRows() == 1 && bottom.getRows() == 1);
    assert(top.at(0,0) == 1);

    auto [left, right] = A.splitCols(1);
    assert(left.at(1,0) == 3 && right.at(1,0) == 4);

    // scale row/col
    A.scaleRow(0, 2);  // row 0 *= 2
    assert(A.at(0,0) == 2 && A.at(0,1) == 4);

    A.scaleCol(1, 3);  // col 1 *= 3
    assert(A.at(0,1) == 12 && A.at(1,1) == 12);
    // add rows
    A.addRows(1,0,2);  // row1 += row0*2
    assert(A.at(1,0) == 3 + 4 && A.at(1,1) == 12 + 24);
    // add cols
    A.addCols(0, 1,2);  // col0 += col1*2
    assert(A.at(0,0) == 2 + 24&&A.at(1,0) == 7+72);
    // norm
    double norm = A.norm();
    assert(norm > 0); // Просто проверим, что считается
    // +=
    RectangularMatrix<int> C = A;
    C += A;
    assert(C.at(0,0) == A.at(0,0) * 2);
    // +
    auto D = A + B;
    assert(D.at(0,0) == A.at(0,0) + B.at(0,0));
    // *= скаляр
    A *= 2;
    assert(A.at(0,0)==52&& A.at(1,1)==72);
    // ==
    RectangularMatrix<int> E = A;
    assert(E == A);

    // !=
    E.at(1,1) += 1;
    assert(E != A);

    std::cout << "✅ All arithmetic matrix tests passed.\n";
}
void testArithmeticOpsDouble() {
    std::cout << "\n--- Arithmetic Operations Test (double) ---\n";
    RectangularMatrix<double> A(2, 2);
    A.at(0,0) = 1.0; A.at(0,1) = 2.0;
    A.at(1,0) = 3.0; A.at(1,1) = 4.0;

    RectangularMatrix<double> B(2, 2);
    B.at(0,0) = 4.0; B.at(0,1) = 6.0;
    B.at(1,0) = 5.0; B.at(1,1) = 7.0;

    // split
    auto [top, bottom] = A.splitRows(1);
    assert(top.getRows() == 1 && bottom.getRows() == 1);
    assert(top.at(0,0) == 1.0);

    auto [left, right] = A.splitCols(1);
    assert(left.at(1,0) == 3.0 && right.at(1,0) == 4.0);

    // scale row/col
    A.scaleRow(0, 2.0);  // row 0 *= 2
    assert(A.at(0,0) == 2.0 && A.at(0,1) == 4.0);

    A.scaleCol(1, 3.0);  // col 1 *= 3
    assert(A.at(0,1) == 12.0 && A.at(1,1) == 12.0);

    // add rows
    A.addRows(1, 0, 2.0);  // row1 += row0*2
    assert(A.at(1,0) == 3.0 + 4.0 && A.at(1,1) == 12.0 + 24.0);

    // add cols
    A.addCols(0, 1, 2.0);  // col0 += col1*2
    assert(A.at(0,0) == 2.0 + 24.0 && A.at(1,0) == 7.0 + 72.0);

    // norm
    double norm = A.norm();
    assert(norm > 0.0); // просто проверка

    // +=
    RectangularMatrix<double> C = A;
    C += A;
    assert(C.at(0,0) == A.at(0,0) * 2.0);

    // +
    auto D = A + B;
    assert(D.at(0,0) == A.at(0,0) + B.at(0,0));

    // *= скаляр
    A *= 2.0;
    assert(A.at(0,0) == 52.0 && A.at(1,1) == 72.0);

    // ==
    RectangularMatrix<double> E = A;
    assert(E == A);

    // !=
    E.at(1,1) += 0.01;
    assert(E != A);

    std::cout << "✅ All arithmetic matrix tests (double) passed.\n";
}

#include <complex>
using cd = std::complex<double>;

void testArithmeticOpsComplex() {
    std::cout << "\n--- Arithmetic Operations Test (std::complex<double>) ---\n";

    RectangularMatrix<cd> A(2, 2);
    A.at(0,0) = cd(1, 1); A.at(0,1) = cd(2, 0);
    A.at(1,0) = cd(3, 0); A.at(1,1) = cd(4, 1);
    auto A_origin = A;
    RectangularMatrix<cd> B(2, 2);
    B.at(0,0) = cd(4, -1); B.at(0,1) = cd(6, 2);
    B.at(1,0) = cd(5, 0);  B.at(1,1) = cd(7, 1);

    // split
    auto [top, bottom] = A.splitRows(1);
    assert(top.getRows() == 1 && bottom.getRows() == 1);
    assert(top.at(0,0) == cd(1, 1));

    auto [left, right] = A.splitCols(1);
    assert(left.at(1,0) == cd(3, 0) && right.at(1,0) == cd(4, 1));

    // scale row/col
    A.scaleRow(0, cd(2, 0));
    assert(A.at(0,0) == cd(2, 2) && A.at(0,1) == cd(4, 0));

    A.scaleCol(1, cd(0, 1));  // умножим на i
    assert(A.at(0,1) == cd(0, 4));  // (4 + 0i) * i = 0 + 4i

    // add rows
    A.addRows(1, 0, cd(2, 0));
    assert(A.at(1,0) == cd(3,0) + cd(2,2)*cd(2,0));

    // add cols
    A.addCols(0, 1, cd(1, 0));  // col0 += col1
    assert(A.at(0,0) == cd(2,2) + cd(0,4));

    // norm
    double norm = A.norm();
    assert(norm > 0);

    // +=
    RectangularMatrix<cd> C = A;
    C += A;
    assert(C.at(0,0) == A.at(0,0) * cd(2,0));

    // +
    auto D = A + B;
    assert(D.at(0,0) == A.at(0,0) + B.at(0,0));

    // ==
    RectangularMatrix<cd> E = A;
    assert(E == A);

    // !=
    E.at(1,1) += cd(0.01, 0);
    assert(E != A);

    std::cout << "✅ All arithmetic matrix tests (complex) passed.\n";
}


int main() {
    std::cout << "=== RECTANGULAR MATRIX TESTS ===\n";
    // int
    testBasicMatrix<int>(1, 2, "int");

    // double
    testBasicMatrix<double>(1.5, 2.5, "double");

    // complex
    testBasicMatrix<cd>(cd(1.0, 1.0), cd(2.0, -1.0), "complex<double>");

    //string
    testStringMatrix();

    //function
    testFunctionMatrix();

    // student
    Student s1("Alice", 90, PersonID("AA123", 2002));
    Student s2("Bob", 85, PersonID("BB456", 2001));
    testBasicMatrix<Student>(s1, s2, "Student");

    // teacher
    Teacher t1("Prof. Ivanov", 15, PersonID("CC789", 1980));
    Teacher t2("Prof. Petrov", 20, PersonID("DD987", 1975));
    testBasicMatrix<Teacher>(t1, t2, "Teacher");

    // person
    Person p1("Generic", PersonID("ID001", 1999));
    Person p2("Other",   PersonID("ID002", 1990));
    testBasicMatrix<Person>(p1, p2, "Person");

    testArithmeticOps();

    testArithmeticOpsDouble();

    testArithmeticOpsComplex();
    
    std::cout << "\n✅ All tests passed.\n";
    return 0;
}
