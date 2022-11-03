#include <immintrin.h>
#include <pthread.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>

#define USE_SIMD

template <class T>
class matrix_t {
   private:
    // m row n column
    T* data;
    size_t m;
    size_t n;

    // return true if two matrix is cophenetic
    bool is_cophenetic(matrix_t* m) { return this->m == m->m && this->n == m->n; }

    // return the reference of the element in (m,n)
    T& get_element(size_t m, size_t n) { return data[this->n * m + n]; }

    // calculate the inner product of two vector (the w row of A and n column of B)
    // ! the number of A's row must equal to B's column
    static T dot_product(matrix_t* a, size_t m, matrix_t* b, size_t n) {
        T ret = 0;
        for (size_t i = 0; i < a->n; i++) {
            ret += a->get_element(m, i) * b->get_element(i, n);
        }
        return ret;
    }

    void row_operation(size_t row, float scale) {
        __m256 scale_reg, tmp;
        size_t begin = row * this->n, i = 0;
#ifdef USE_SIMD
        scale_reg = _mm256_set_ps(scale, scale, scale, scale, scale, scale, scale, scale);
        for (i = 0; i + 8 < n; i += 8) {
            tmp = _mm256_loadu_ps(this->data + begin + i);
            _mm256_storeu_ps(this->data + begin + i, _mm256_mul_ps(tmp, scale_reg));
        }
#endif
        while (i < n) {
            this->data[begin + i] *= scale;
            i++;
        }
    }

    void row_operation(size_t row_from, size_t row_to, float scale) {
        size_t from = row_from * n;
        size_t to = row_to * n;
        size_t i = 0;
#ifdef USE_SIMD
        __m256 scale_reg, from_reg, to_reg;
        scale_reg = _mm256_set_ps(scale, scale, scale, scale, scale, scale, scale, scale);
        for (i = 0; i + 8 <= n; i += 8) {
            from_reg = _mm256_loadu_ps(this->data + from + i);
            to_reg = _mm256_loadu_ps(this->data + to + i);
            from_reg = _mm256_mul_ps(from_reg, scale_reg);
            to_reg = _mm256_add_ps(from_reg, to_reg);
            _mm256_storeu_ps(this->data + to + i, to_reg);
        }
#endif
        while (i < n) {
            data[to + i] += (data[from + i] * scale);
            i++;
        }
    }

   public:
    matrix_t(size_t m, size_t n) {
        this->m = m;
        this->n = n;
        posix_memalign((void**)&this->data, 32, sizeof(float) * n * m);
    }

    ~matrix_t() {
        delete[] data;
    }

    matrix_t* plus(matrix_t* matrix) {
        if (is_cophenetic(matrix) == false)
            return nullptr;
        size_t len = matrix->m * matrix->n;
        matrix_t* ret = new matrix_t(this->m, this->n);
        size_t i = 0;
#ifdef USE_SIMD
        __m256 a, b;
        for (i = 0; i + 8 <= len; i += 8) {
            a = _mm256_load_ps(this->data + i);
            b = _mm256_load_ps(matrix->data + i);
            _mm256_store_ps(ret->data + i, _mm256_add_ps(a, b));
        }
#endif
        while (i < len) {
            ret->data[i] = this->data[i] + matrix->data[i];
            i++;
        }
        return ret;
    }

    matrix_t* sub(matrix_t* matrix) {
        if (is_cophenetic(matrix) == false)
            return nullptr;
        size_t len = matrix->m * matrix->n;
        matrix_t* ret = new matrix_t(this->m, this->n);
        size_t i = 0;
#ifdef USE_SIMD
        __m256 a, b;
        for (i = 0; i + 8 < len; i += 8) {
            a = _mm256_load_ps(this->data + i);
            b = _mm256_load_ps(matrix->data + i);
            _mm256_store_ps(ret->data + i, _mm256_sub_ps(a, b));
        }
#endif
        while (i < len) {
            ret->data[i] = this->data[i] - matrix->data[i];
            i++;
        }
        return ret;
    }

    matrix_t* mul(int n) {
        matrix_t* ret = new matrix_t(this->m, this->n);
        size_t len = this->m * this->n;
        for (size_t i = 0; i < len; i++) {
            ret->data[i] = this->data[i] * n;
        }
        return ret;
    }

    matrix_t* mul(matrix_t* matrix) {
        if (this->n != matrix->m)
            return nullptr;
        size_t len = this->m * matrix->n;
        matrix_t* ret = new matrix_t(this->m, matrix->n);
        std::thread threads[THREAD_NUM];
        for (size_t i = 0; i < THREAD_NUM; i++) {
            threads[i] = std::thread([this, i, len, &ret, &matrix] {
                for (size_t j = i; j < len; j += THREAD_NUM) {
                    size_t m = i / ret->n;
                    size_t n = i % ret->n;
                    ret->get_element(m, n) = dot_product(this, m, matrix, n);
                }
            });
        }
        for (size_t i = 0; i < THREAD_NUM; i++)
            threads[i].join();
        return ret;
    }

    static matrix_t* eye(size_t n) {
        size_t len = n * n;
        matrix_t* ret = new matrix_t(n, n);
        memset(ret->data, 0, sizeof(T) * len);
        for (size_t i = 0; i < n; i++) {
            ret->data[i * n + i] = 1;
        }
        return ret;
    }

    // return the inverse matrix of self
    //! m must equal to n
    matrix_t* inverse() {
        matrix_t* ret = eye(n);
        T* tmp = this->data;
        posix_memalign((void**)&this->data, 32, sizeof(float) * n * m);
        memcpy(this->data, tmp, n * n * sizeof(T));

        for (size_t i = 0; i < m; i++) {
            float s = 1.0 / get_element(i, i);
            row_operation(i, s);
            ret->row_operation(i, s);
            for (size_t j = i + 1; j < m; j++) {
                s = -get_element(j, i) / 1.0;
                row_operation(i, j, s);
                ret->row_operation(i, j, s);
            }
        }

        for (size_t i = 1; i < m; i++) {
            size_t j = i - 1;
            for (size_t j = i - 1; j + 1 != 0; j--) {
                float s = -get_element(j, i) / 1.0;
                row_operation(i, j, s);
                ret->row_operation(i, j, s);
            }
        }

        delete[] this->data;
        this->data = tmp;

        return ret;
    }

    matrix_t* div(matrix_t* m) {
        matrix_t* i = m->inverse();
        matrix_t* ret = this->mul(i);
        delete i;
        return ret;
    }

    void print() {
        size_t len = m * n;
        for (size_t i = 0; i < len; i++) {
            if (i % n == 0)
                std::cout << std::endl;
            std::cout.width(5);
            std::cout.precision(4);
            std::cout << data[i] << "  ";
        }
    }

    void fill(T* data) {
        for (size_t i = 0; i < this->m * this->n; i++) {
            this->data[i] = data[i];
        }
    }
};
