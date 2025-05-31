#include <cassert>
#include <sstream>
#include <complex>
#include <iostream>
#include "RectangularMatrix.h"

using cd = std::complex<double>;

int main() {
    // 1. copy / assign
    RectangularMatrix<int> A(2,2);
    A.at(0,0)=1; A.at(0,1)=2; A.at(1,0)=3; A.at(1,1)=4;
    RectangularMatrix<int> B = A;
    assert(B == A);
    RectangularMatrix<int> C(2,2);
    C = A;
    assert(C == A);

    // 2. map / reduce
    A.map([](int x){ return x+1; }); // {2,3;4,5}
    int sum = A.reduce(0, [](int acc,int v){ return acc+v; });
    assert(sum == 2+3+4+5);


    // 3. zip / unzip
    RectangularMatrix<double> D(2,2);
    for (size_t i=0;i<2;++i) for (size_t j=0;j<2;++j)
        D.at(i,j) = A.at(i,j) + 0.5;
    auto Z = zip(A, D);
    assert(Z.at(1,1).first == A.at(1,1));
    assert(Z.at(1,1).second == D.at(1,1));
    auto [topPart, bottomPart] = RectangularMatrix<int>::unzip(Z);
    assert(topPart == A);
    assert(bottomPart == D);

    // 4. slice / split
    auto S = A.slice(0,1,1,2);
    assert(S.getRows()==1 && S.getCols()==1 && S.at(0,0)==3);
    auto [Tpart, Bpart] = A.splitRows(1);
    assert(Tpart.getRows()==1 && Bpart.getRows()==1);
    auto [Lpart, Rpart] = A.splitCols(1);
    assert(Lpart.getCols()==1 && Rpart.getCols()==1);

    // 5. iterator
    int cnt = 0;
    for (auto &v : A) ++cnt;
    assert(cnt == 4);

    // 6. +, +=, *, *=
    RectangularMatrix<int> E = A + A;
    E *= 3;  // 3*(A+A)
    RectangularMatrix<int> F = 3 * (A + A);
    assert(E == F);

    // 7. norm
    RectangularMatrix<cd> M(1,2);
    M.at(0,0) = cd(3,4); M.at(0,1) = cd(1,-1);
    double n = M.norm();
    assert(std::abs(n - std::sqrt(25+2)) < 1e-9);

    // 8. row/col transforms
    RectangularMatrix<int> G(3,3);
    int v = 1;
    for (size_t i=0;i<3;++i)
        for (size_t j=0;j<3;++j)
            G.at(i,j) = v++;
    G.swapRows(0,2);
    assert(G.at(0,0) == 7 && G.at(2,0) == 1);
    G.swapCols(0,2);
    assert(G.at(0,0) == 9 && G.at(0,2) == 7);
    G.scaleRow(0, -1);
    assert(G.at(0,1) == -8);
    G.scaleCol(2, 2);
    // После scaleCol: G.at(1,2) == 8
    assert(G.at(1,2) == 8);
    G.addRows(1, 0, 2);
    // После addRows: G.at(1,0) == -12
    assert(G.at(1,0) == -12);
    G.addCols(1, 2, -1);
    // После addCols: G.at(2,1) == 0
    assert(G.at(2,1) == 0);

    // 9. print
    std::ostringstream oss;
    oss << G;
    std::string out = oss.str();
    assert(out.find("9 ") != std::string::npos);

    std::cout << "All tests passed!\n";
    return 0;
}
