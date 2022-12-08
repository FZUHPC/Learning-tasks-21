typedef int bool;
#include <stddef.h>

// 且比雪夫和类型
typedef struct {
    double sum;            // 和的值
    size_t pivot_index; // 该和对应的pivots在pivots数组中的下标
} sum_t;

bool greater(const sum_t* a, const sum_t* b);

bool less(const sum_t* a, const sum_t* b);

// 堆的下沉操作
void sink(size_t size, sum_t* heap, bool (*cmp)(const sum_t*, const sum_t*));