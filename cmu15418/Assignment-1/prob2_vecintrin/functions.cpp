#include "CMU418intrin.h"
#include "logger.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
using namespace std;

void absSerial(float *values, float *output, int N) {
    for (int i = 0; i < N; i++) {
        float x = values[i];
        if (x < 0) {
            output[i] = -x;
        } else {
            output[i] = x;
        }
    }
}

// implementation of absolute value using 15418 instrinsics
void absVector(float *values, float *output, int N) {
    __cmu418_vec_float x;
    __cmu418_vec_float result;
    __cmu418_vec_float zero = _cmu418_vset_float(0.f);
    __cmu418_mask maskAll, maskIsNegative, maskIsNotNegative;

    // Note: Take a careful look at this loop indexing.  This example
    // code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
    // Why is that the case?
    for (int i = 0; i < N; i += VECTOR_WIDTH) {

        // All ones
        maskAll = _cmu418_init_ones();

        // All zeros
        maskIsNegative = _cmu418_init_ones(0);

        // Load vector of values from contiguous memory addresses
        _cmu418_vload_float(x, values + i, maskAll);                // x = values[i];

        // Set mask according to predicate
        _cmu418_vlt_float(maskIsNegative, x, zero, maskAll);        // if (x < 0) {

        // Execute instruction using mask ("if" clause)
        _cmu418_vsub_float(result, zero, x, maskIsNegative);        //   output[i] = -x;

        // Inverse maskIsNegative to generate "else" mask
        maskIsNotNegative = _cmu418_mask_not(maskIsNegative);       // } else {

        // Execute instruction ("else" clause)
        _cmu418_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

        // Write results back to memory
        _cmu418_vstore_float(output + i, result, maskAll);
    }
}

// Accepts an array of values and an array of exponents
// For each element, compute values[i]^exponents[i] and clamp value to
// 4.18.  Store result in outputs.
// Uses iterative squaring, so that total iterations is proportional
// to the log_2 of the exponent
void clampedExpSerial(float *values, int *exponents, float *output, int N) {
    for (int i = 0; i < N; i++) {
        float x = values[i];
        float result = 1.f;
        int y = exponents[i];
        float xpower = x;
        while (y > 0) {
            if (y & 0x1) {
                result *= xpower;
                if (result > 4.18f) {
                    result = 4.18f;
                    break;
                }
            }
            xpower = xpower * xpower;
            y >>= 1;
        }
        output[i] = result;
    }
}

void clampedExpVector(float *values, int *exponents, float *output, int N) {
    // TODO: Implement your vectorized version of clampedExpSerial here
    __cmu418_vec_float x, result, xpower;
    __cmu418_vec_int y, tmp_y; // tmp_y stores y & 0x1

    __cmu418_vec_float exp = _cmu418_vset_float(4.18f);
    __cmu418_vec_int zero = _cmu418_vset_int(0);
    __cmu418_vec_int one = _cmu418_vset_int(1);

    __cmu418_mask maskAll, maskIsNegative, maskIsNotNegative, maskResultIsClamped;

    int cnt = N % VECTOR_WIDTH;
    // if(cnt) clampedExpSerial(values, exponents, output, cnt); // Serial Solve

    // Vector Solve
    for (int i = 0; i < N; i += VECTOR_WIDTH) {
        maskAll = (i == 0 && cnt) ? _cmu418_init_ones(cnt) : _cmu418_init_ones();
        maskIsNegative = _cmu418_init_ones(0);

        _cmu418_vload_float(x, values + i, maskAll);                    // x = values[i];
        _cmu418_vset_float(result, 1.f, maskAll);                       // result = 1.f;
        _cmu418_vload_int(y, exponents + i, maskAll);                   // y = exponents[i];
        _cmu418_vmove_float(xpower, x, maskAll);                        // xpower = x;

        while(_cmu418_vgt_int(maskIsNegative, y, zero, maskAll), _cmu418_cntbits(maskIsNegative)) // while(y > 0)
        {
            _cmu418_vbitand_int(tmp_y, y, one, maskAll);                        // tmp_y = y & 0x1
            _cmu418_vgt_int(maskIsNegative, tmp_y, zero, maskAll);              // if(tmp_y) {
            _cmu418_vmult_float(result, result, xpower, maskIsNegative);        //  result *= xpower
                                                                                // }
            _cmu418_vmult_float(xpower, xpower, xpower, maskAll);               // xpower *= xpower
            _cmu418_vshiftright_int(y, y, one, maskAll);                        // y >>= 1
        }

        _cmu418_vgt_float(maskResultIsClamped, result, exp, maskAll);   //  if(result > 4.18f) {
        _cmu418_vmove_float(result, exp, maskResultIsClamped);          //      result = 4.18f
                                                                        //  }
        _cmu418_vstore_float(output + i, result, maskAll);              // output[i] = result

        if(i == 0 && cnt) i -= VECTOR_WIDTH;
    }
}

float arraySumSerial(float *values, int N) {
    float sum = 0;
    for (int i = 0; i < N; i++) {
        sum += values[i];
    }

    return sum;
}

// Assume N % VECTOR_WIDTH == 0
// Assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N) {
    // TODO: Implement your vectorized version here

    __cmu418_vec_float x, result = _cmu418_vset_float(0.f);
    __cmu418_mask maskAll = _cmu418_init_ones();

    int cnt = VECTOR_WIDTH;
    for(int i = 0; i < N; i += VECTOR_WIDTH)
    {
        _cmu418_vload_float(x, values + i, maskAll);                        // x = values[i];
        _cmu418_vadd_float(result, result, x, maskAll);                     // result += x;
    }

    while(cnt >>= 1)
    {
        _cmu418_hadd_float(result, result);
        _cmu418_interleave_float(result, result);
    }

    return result.value[0];
}
