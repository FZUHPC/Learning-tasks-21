#include <stdio.h>
#include <algorithm>
#include <math.h>
#include "CMU418intrin.h"
#include "logger.h"
using namespace std;


void absSerial(float* values, float* output, int N) {
    for (int i=0; i<N; i++) {
	float x = values[i];
	if (x < 0) {
	    output[i] = -x;
	} else {
	    output[i] = x;
	}
    }
}

// implementation of absolute value using 15418 instrinsics
void absVector(float* values, float* output, int N) {
    __cmu418_vec_float x;
    __cmu418_vec_float result;
    __cmu418_vec_float zero = _cmu418_vset_float(0.f);
    __cmu418_mask maskAll, maskIsNegative, maskIsNotNegative;

    //  Note: Take a careful look at this loop indexing.  This example
    //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
    //  Why is that the case?
    for (int i=0; i<N; i+=VECTOR_WIDTH) {

	// All ones
	maskAll = _cmu418_init_ones();

	// All zeros
	maskIsNegative = _cmu418_init_ones(0);

	// Load vector of values from contiguous memory addresses
	_cmu418_vload_float(x, values+i, maskAll);               // x = values[i];

	// Set mask according to predicate
	_cmu418_vlt_float(maskIsNegative, x, zero, maskAll);     // if (x < 0) {

	// Execute instruction using mask ("if" clause)
	_cmu418_vsub_float(result, zero, x, maskIsNegative);      //   output[i] = -x;

	// Inverse maskIsNegative to generate "else" mask
	maskIsNotNegative = _cmu418_mask_not(maskIsNegative);     // } else {

	// Execute instruction ("else" clause)
	_cmu418_vload_float(result, values+i, maskIsNotNegative); //   output[i] = x; }

	// Write results back to memory
	_cmu418_vstore_float(output+i, result, maskAll);
    }
}

// Accepts an array of values and an array of exponents
// For each element, compute values[i]^exponents[i] and clamp value to
// 4.18.  Store result in outputs.
// Uses iterative squaring, so that total iterations is proportional
// to the log_2 of the exponent
void clampedExpSerial(float* values, int* exponents, float* output, int N) {
	
    for (int i=0; i<N; i++) {
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

void clampedExpVector(float* values, int* exponents, float* output, int N) {
    __cmu418_vec_float x,xpower;
	__cmu418_vec_int y;
    __cmu418_vec_float result =_cmu418_vset_float(1.f);
    __cmu418_vec_int zero = _cmu418_vset_int(0);
	__cmu418_vec_int one = _cmu418_vset_int(1);
	__cmu418_vec_float ex = _cmu418_vset_float(4.18f);
	__cmu418_vec_int  temp=_cmu418_vset_int(0);
    __cmu418_mask maskIsPositive,maskAll,maskAll1,masktemp;
	maskAll = _cmu418_init_ones();
	maskAll1 = _cmu418_init_ones();
	for (int i=0; i<N/VECTOR_WIDTH*VECTOR_WIDTH; i+=VECTOR_WIDTH){
		result =_cmu418_vset_float(1.f);
		maskAll = _cmu418_init_ones();
		maskAll1 = _cmu418_init_ones();
		_cmu418_vload_float(x, &values[i], maskAll);//load x
		_cmu418_vload_float(xpower, &values[i], maskAll);//load xpower
		_cmu418_vload_int(y,&exponents[i], maskAll);//load y
		while(_cmu418_cntbits(maskAll)){
			_cmu418_vgt_int(masktemp,y,zero,maskAll);//判断是否关闭改列
			maskAll=_cmu418_mask_and(maskAll,masktemp);
			_cmu418_vbitand_int(temp,y,one,maskAll);//y&1
			_cmu418_vgt_int(maskIsPositive,temp,zero,maskAll);//判断y时候是奇数
			maskIsPositive=_cmu418_mask_and(maskAll,maskIsPositive);
			_cmu418_vmult_float(result,result,xpower,maskIsPositive);
			_cmu418_vgt_float(masktemp,result,ex,maskIsPositive);
			masktemp=_cmu418_mask_and(masktemp,maskIsPositive);
			_cmu418_vset_float(result,4.18,masktemp);
			masktemp=_cmu418_mask_not(masktemp);
			maskAll=_cmu418_mask_and(maskAll,masktemp);
			_cmu418_vmult_float(xpower,xpower,xpower,maskAll);
			_cmu418_vshiftright_int(y,y,one,maskAll);
		}
		_cmu418_vstore_float(&output[i], result, maskAll1);
	}
    result =_cmu418_vset_float(1.f);
	maskAll = _cmu418_init_ones(N-N/VECTOR_WIDTH*VECTOR_WIDTH);
	maskAll1 = _cmu418_init_ones(N-N/VECTOR_WIDTH*VECTOR_WIDTH);
	for (int i=N/VECTOR_WIDTH*VECTOR_WIDTH; i<N; i+=VECTOR_WIDTH){
		_cmu418_vload_float(x, values+i, maskAll);//load x
		_cmu418_vload_float(xpower, values+i, maskAll);//load xpower
		_cmu418_vload_int(y,exponents+i, maskAll);//load y
		while(_cmu418_cntbits(maskAll)){
			_cmu418_vgt_int(masktemp,y,zero,maskAll);//判断是否关闭改列
			maskAll=_cmu418_mask_and(maskAll,masktemp);
			_cmu418_vbitand_int(temp,y,one,maskAll);//y&1
			_cmu418_vgt_int(maskIsPositive,temp,zero,maskAll);//判断y时候是奇数
			maskIsPositive=_cmu418_mask_and(maskAll,maskIsPositive);
			_cmu418_vmult_float(result,result,xpower,maskIsPositive);
			_cmu418_vgt_float(masktemp,result,ex,maskIsPositive);
			masktemp=_cmu418_mask_and(masktemp,maskIsPositive);
			_cmu418_vset_float(result,4.18,masktemp);
			masktemp=_cmu418_mask_not(masktemp);
			maskAll=_cmu418_mask_and(maskAll,masktemp);
			_cmu418_vmult_float(xpower,xpower,xpower,maskAll);
			_cmu418_vshiftright_int(y,y,one,maskAll);
		}
		_cmu418_vstore_float(&output[i], result, maskAll1);
	}
}


float arraySumSerial(float* values, int N) {
    float sum = 0;
    for (int i=0; i<N; i++) {
		sum += values[i];
    }

    return sum;
}

// Assume N % VECTOR_WIDTH == 0
// Assume VECTOR_WIDTH is a power of 2
float arraySumVector(float* values, int N) {
	float sum=0,sum1;
	__cmu418_vec_float x;
    __cmu418_vec_float zero = _cmu418_vset_float(0.f);
	__cmu418_vec_float one = _cmu418_vset_float(0.f);
	__cmu418_vec_float ex = _cmu418_vset_float(4.18);
	__cmu418_vec_float  temp=_cmu418_vset_float(1.f);
	int h=1;
	__cmu418_mask maskAll=_cmu418_init_ones();
    __cmu418_mask mask=_cmu418_init_ones(1);
	for (int i=0; i<N/VECTOR_WIDTH*VECTOR_WIDTH; i+=VECTOR_WIDTH){
		_cmu418_vload_float(x, values+i,maskAll);
		int flag=VECTOR_WIDTH;
		while(flag>1){
			flag>>=1;
			_cmu418_hadd_float(x,x);
			_cmu418_interleave_float(x,x);
		}
		
		_cmu418_vstore_float(&sum1,x,mask);
		sum+=sum1;
	}
	return sum;
}
