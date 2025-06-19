#pragma once
#include "ArraySequence.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <complex>

template <typename T>
class RectangularMatrix {
private:
    ArraySequence<ArraySequence<T>> data;
    size_t rows, cols;

    static double absval(const T& v) {
        return static_cast<double>(std::abs(v));
    }

public:
    RectangularMatrix(size_t r, size_t c): rows(r), cols(c)
    {
        for (size_t i = 0; i < rows; ++i) {
            ArraySequence<T> row;
            for (size_t j = 0; j < cols; ++j) {
                row.append(T{});
            }
            data.append(row);
        }
    }

    RectangularMatrix(const RectangularMatrix& o): rows(o.rows), cols(o.cols)
    {
        for (size_t i = 0; i < rows; ++i) {
            ArraySequence<T> row;
            for (size_t j = 0; j < cols; ++j) {
                row.append(o.at(i,j));
            }
            data.append(row);
        }
    }

    RectangularMatrix& operator=(const RectangularMatrix& o) {
        if (this != &o) {
            rows = o.rows;
            cols = o.cols;
            data = ArraySequence<ArraySequence<T>>();
            for (size_t i = 0; i < rows; ++i) {
                ArraySequence<T> row;
                for (size_t j = 0; j < cols; ++j) {
                    row.append(o.at(i,j));
                }
                data.append(row);
            }
        }
        return *this;
    }

    T& at(size_t i, size_t j) {
        if (i >= rows || j >= cols) throw std::out_of_range("bad index");
        return data.get(i).get(j);
    }
    const T& at(size_t i, size_t j) const {
        if (i >= rows || j >= cols) throw std::out_of_range("bad index");
        return data.get(i).get(j);
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    class iterator {
        RectangularMatrix* m; size_t r,c;
    public:
        iterator(RectangularMatrix* mm, size_t rr, size_t cc)
            : m(mm), r(rr), c(cc) {}
        T& operator*() { return m->at(r,c); }
        iterator& operator++() {
            if (++c == m->cols) { c = 0; ++r; }
            return *this;
        }
        bool operator==(const iterator& o) const { return r==o.r && c==o.c; }
        bool operator!=(const iterator& o) const { return !(*this==o); }
    };
    iterator begin() { return {this,0,0}; }
    iterator end()   { return {this,rows,0}; }

    void print(std::ostream& os = std::cout) const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                os << at(i,j) << ' ';
            }
            os << '\n';
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const RectangularMatrix& M) {
        M.print(os); return os;
    }

    bool operator==(const RectangularMatrix& o) const {
        if (rows!=o.rows || cols!=o.cols) return false;
        for (size_t i=0;i<rows;++i)
            for (size_t j=0;j<cols;++j)
                if (at(i,j)!=o.at(i,j)) return false;
        return true;
    }
    bool operator!=(const RectangularMatrix& o) const { return !(*this==o); }

    RectangularMatrix& operator+=(const RectangularMatrix& o) {
        if (rows!=o.rows || cols!=o.cols) throw std::invalid_argument("size mismatch");
        for (size_t i=0;i<rows;++i)
            for (size_t j=0;j<cols;++j)
                at(i,j) += o.at(i,j);
        return *this;
    }
    friend RectangularMatrix operator+(RectangularMatrix a, const RectangularMatrix& b) {
        return a += b;
    }

    RectangularMatrix& operator*=(const T& k) {
        for (auto& v : *this) { v *= k; }
        return *this;
    }
    friend RectangularMatrix operator*(RectangularMatrix a, const T& k) {
        return a *= k;
    }

    double norm() const {
        double s=0;
        for (size_t i=0;i<rows;++i)
            for (size_t j=0;j<cols;++j){
                double v=absval(at(i,j));
                s+=v*v;
            }
        return std::sqrt(s);
    }

    void swapRows(size_t i,size_t j) {
        if (i>=rows||j>=rows) throw std::out_of_range("row");
        auto tmp = data.get(i);
        data.get(i) = data.get(j);
        data.get(j) = tmp;
    }
    void swapCols(size_t i,size_t j) {
        if (i>=cols||j>=cols) throw std::out_of_range("col");
        for (size_t r=0;r<rows;++r) std::swap(at(r,i), at(r,j));
    }
    void scaleRow(size_t i,const T& k){
        if (i>=rows) throw std::out_of_range("row");
        for (size_t j=0;j<cols;++j) at(i,j)*=k;
    }
    void scaleCol(size_t j,const T& k){
        if (j>=cols) throw std::out_of_range("col");
        for (size_t i=0;i<rows;++i) at(i,j)*=k;
    }
    void addRows(size_t dst,size_t src,const T& k){
        if (dst>=rows||src>=rows) throw std::out_of_range("row");
        for (size_t j=0;j<cols;++j) at(dst,j)+=at(src,j)*k;
    }
    void addCols(size_t dst,size_t src,const T& k){
        if (dst>=cols||src>=cols) throw std::out_of_range("col");
        for (size_t i=0;i<rows;++i) at(i,dst)+=at(i,src)*k;
    }

template <typename Func>
RectangularMatrix<T> Map(Func f) const {
    RectangularMatrix<T> result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.at(i,j) = f(at(i,j));
    return result;
}


    template<typename U>
    friend RectangularMatrix<std::pair<T,U>>
    zip(const RectangularMatrix& A,const RectangularMatrix<U>& B){
        if (A.getRows()!=B.getRows()||A.getCols()!=B.getCols())
            throw std::invalid_argument("zip size");
        RectangularMatrix<std::pair<T,U>> R(A.getRows(),A.getCols());
        for (size_t i=0;i<A.getRows();++i)
            for (size_t j=0;j<A.getCols();++j)
                R.at(i,j)={A.at(i,j),B.at(i,j)};
        return R;
    }

    template<typename U>
    static std::pair<RectangularMatrix<T>,RectangularMatrix<U>>
    unzip(const RectangularMatrix<std::pair<T,U>>& Z) {
        size_t r=Z.getRows(), c=Z.getCols();
        RectangularMatrix<T> A(r,c);
        RectangularMatrix<U> B(r,c);
        for (size_t i=0;i<r;++i)
            for (size_t j=0;j<c;++j){
                auto p = Z.at(i,j);
                A.at(i,j)=p.first;
                B.at(i,j)=p.second;
            }
        return std::make_pair(A,B);
    }

    RectangularMatrix slice(size_t r0,size_t r1,size_t c0,size_t c1) const {
        if (r1>rows||c1>cols||r0>r1||c0>c1) throw std::out_of_range("slice");
        RectangularMatrix S(r1-r0,c1-c0);
        for (size_t i=r0;i<r1;++i)
            for (size_t j=c0;j<c1;++j)
                S.at(i-r0,j-c0)=at(i,j);
        return S;
    }
    std::pair<RectangularMatrix,RectangularMatrix> splitRows(size_t idx) const {
        return {slice(0,idx,0,cols), slice(idx,rows,0,cols)};
    }
    std::pair<RectangularMatrix,RectangularMatrix> splitCols(size_t idx) const {
        return {slice(0,rows,0,idx), slice(0,rows,idx,cols)};
    }
};