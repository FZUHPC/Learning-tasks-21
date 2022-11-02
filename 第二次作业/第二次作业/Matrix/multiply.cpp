#include <iostream>
#include <iomanip>
#include <fstream>
#include <omp.h>
#include <x86intrin.h>
#include <immintrin.h>
#include "Matrix.hpp"
using namespace std;

const int N = 499;

int main() {
	Matrix<double> a(N, N);
	Matrix<double> b(N, N);
	ifstream fp1("data_a.txt");
	ifstream fp2("data_b.txt");
	if (!fp1.is_open() || !fp2.is_open()) {
		exit(-1);
	}
	for (int i = 0; i < N; i++) {
		if (fp1.eof() || fp2.eof()) break;
		for (int j = 0; j < N; j++) {
			fp1 >> a[i][j];
			fp2 >> b[i][j];
		}
	}
	fp1.close();
	fp2.close();
	double t0, t1, T0, T1;
	Matrix<double> temp = a;
	Matrix<double> c = a * b;
	t0 = omp_get_wtime();
	for (int i = 0; i < 100; i++)
		c = a * b;
	t1 = omp_get_wtime();

	T0 = (t1 - t0) * 1000;
	printf("T0: %f ms\n", T0);

#pragma omp parallel num_threads(8)
	t0 = omp_get_wtime();
	for (int i = 0; i < 100; i++) 
		c = ( a *= b );
	t1 = omp_get_wtime();

	T1 = (t1 - t0) * 1000;
	printf("T1: %f ms\n", T1);
	printf("T0/T1: %f \n", T0 / T1);
	ofstream op("data_c.txt");
	if (!op.is_open()) {
		exit(-1);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			op << setiosflags(ios::fixed) << setprecision(6) << c[i][j] << '\n';
		}
	}
	return 0;
}
