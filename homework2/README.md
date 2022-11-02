```shell
gcc Matrix.c -o Matrix -mavx -mavx2 -fopenmp -lpthread

./Matrix omp
./Matrix simd
./Matrix pth
./Matrix normal
```
