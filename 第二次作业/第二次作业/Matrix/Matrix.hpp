#pragma once

#include <iostream>

using namespace std;

template<typename T>
class Matrix {
private:
    int row, col;
    T** p;
    void init();
public:
    Matrix(){}
    Matrix(int row, int col);//默认初值为0 
    Matrix(int row, int col, T value);//全部赋予初值 
    Matrix(const Matrix& r);//拷贝构造函数 
    int GetRow() const { return row; }
    int GetCol() const { return col; }
    T* operator[](int i) { return p[i]; }

   /* Matrix& operator=(const Matrix& r) {//矩阵复制 
        if (this == &r)
            return *this;
        if (row != r.row || col != r.col) {
            for (int i = 0; i < row; i++) {
                delete[] p[i];
            }
            delete[] p;
            row = r.row;
            col = r.col;
            init();
        }
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                p[i][j] = r.p[i][j];
            }
        }
        return *this;
    }*/

    Matrix Inverse();
    Matrix operator=(const Matrix& r);
    Matrix operator+(const Matrix& r);//矩阵加法 
    Matrix operator-(const Matrix& r);//矩阵减法 
    Matrix operator*(const Matrix& r);//矩阵乘法
    Matrix operator*=(const Matrix& r);//矩阵优化版乘法 
    Matrix operator/(const Matrix& r);//矩阵除法 
    void show();
};

template<typename T>
void Matrix<T>::init() {
    p = new T * [row];//分配指针空间 
    for (int i = 0; i < row; i++)
        p[i] = new T[col];//分配double型空间 
}

template<typename T>
Matrix<T>::Matrix(int rows, int cols) {
    row = rows;
    col = cols;
    init();
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            p[i][j] = 0;
}

template<typename T>
Matrix<T>::Matrix(int rows, int cols, T value) {
    row = rows;
    col = cols;
    init();
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            p[i][j] = value;
}

template<typename T>
Matrix<T>::Matrix(const Matrix& r) {
    row = r.row;
    col = r.col;
    init();
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            p[i][j] = r.p[i][j];
}

template<typename T>
Matrix<T> Matrix<T>::operator=(const Matrix& r) {
            if (this == &r)
            return *this;
        if (row != r.row || col != r.col) {
            for (int i = 0; i < row; i++) {
                delete[] p[i];
            }
            delete[] p;
            row = r.row;
            col = r.col;
            init();
        }
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                p[i][j] = r.p[i][j];
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& r) {
    Matrix c(row, col);
    if (row == r.row && col == r.col)
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                c[i][j] = p[i][j] + r.p[i][j];
    return c;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& r) {
    Matrix c(row, col);
    if (row == r.row && col == r.col)
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                c[i][j] = p[i][j] - r.p[i][j];
    return c;
}

template<typename T>
Matrix<T> Matrix<T>::operator * (const Matrix& r) {
    if (col != r.GetRow()) return Matrix<T>(0, 0);
    T sum = 0;
    Matrix c(row, r.GetCol());
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < r.col; j++) {
            for (int k = 0; k < col; k++)
                sum += p[i][k] * (r.p[k][j]);
            c[i][j] = sum;
            sum = 0;
        }
    }
    return c;
}

template<typename T>
Matrix<T> Matrix<T>::operator *= (const Matrix& r) {
    if (col != r.row) return Matrix<T>(0, 0);
     __m256d sum = _mm256_set1_pd(0);    
    int i, j, k, ii, jj, kk, BLK_SZ = 50, BLK_SZ_K = 10000;
    Matrix c(row, r.col);
    for ( i = 0; i < row / BLK_SZ; ++i) {
        for ( j = 0; j < r.col / BLK_SZ; ++j) {
            for ( k = 0; k < col / BLK_SZ_K; ++k) {
                for ( ii = 0; ii < BLK_SZ; ++ii) {
                    for ( jj = 0; jj < BLK_SZ; ++jj) {
                        for ( kk = 0; kk < BLK_SZ_K; ++kk) {
                           sum = _mm256_add_pd(sum, _mm256_mul_pd(_mm256_set1_pd (p[i * BLK_SZ + ii][k * BLK_SZ_K + kk]), _mm256_set1_pd(r.p[k * BLK_SZ_K + kk][j * BLK_SZ + jj])));
                              _mm256_store_pd(&p[i][j] , sum);
                                sum = _mm256_set1_pd(0);
                        }
                    }
                }
            }
        }
    }
    return c;
}

template<typename T>
Matrix<T> Matrix<T>::Inverse() {
    int i, j, k, M = this->col, N = 2 * this->col;
    T** b = new T * [M];
    for (int i = 0; i < N; i++)
        b[i] = new T[N];
    Matrix c(M, N);
    for (i = 0; i < M; i++)
        for (j = 0; j < M; j++)
            b[i][j] = p[i][j];
    for (i = 0; i < M; i++)
        for (j = M; j < N; j++) {
            if (i == (j - M))
                b[i][j] = 1;
            else
                b[i][j] = 0;
        }
    for (i = 0; i < M; i++) {
        if (b[i][i] == 0) {
            for (k = i; k < M; k++) {
                if (b[k][i]) {
                    for (int j = 0; j < N; j++) {
                        T temp;
                        temp = b[i][j];
                        b[i][j] = b[k][j];
                        b[k][j] = temp;
                    }
                    break;
                }
            }
            if (k == M) {
                printf("该矩阵不可逆！\n");
                exit(0);
            }
        }
        for (j = N - 1; j >= i; j--)
            b[i][j] /= b[i][i];
        for (k = 0; k < M; k++) {
            if (k != i) {
                T temp = b[k][i];
                for (j = 0; j < N; j++)
                    b[k][j] -= temp * b[i][j];
            }
        }
    }
    for (i = 0; i < M; i++)
        for (j = 3; j < N; j++)
            c[i][j - 3] = b[i][j];
    return c;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const Matrix<T>& r) {
    Matrix<T> c = this->Inverse();
    c = c * r;
    return c;
}

template<typename T>
void Matrix<T>::show() {
    for (int i = 0; i < row; i++) {
        cout << endl;
        for (int j = 0; j < col; j++)
            cout << p[i][j] << " ";
    }
    cout << endl;
}
