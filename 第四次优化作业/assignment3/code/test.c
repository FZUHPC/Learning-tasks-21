#include <stdio.h>

int main(int argc, char** argv) {
    const char* file = argv[1];
    FILE* f = fopen(file, "r");
    int n, sum = 0;
    while (fscanf(f, "%d", &n) != EOF) {
        sum += n;
    }
    printf("%d\n", sum);
}
