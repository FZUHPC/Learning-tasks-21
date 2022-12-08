#include "../include/heap.h"
#include <stddef.h>

bool greater(const sum_t* a, const sum_t* b) {
    return a->sum > b->sum;
}

bool less(const sum_t* a, const sum_t* b) {
    return a->sum < b->sum;
}

void swap(sum_t* a, sum_t* b) {
    sum_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void sink(size_t size, sum_t* heap, bool (*cmp)(const sum_t*, const sum_t*)) {
    size_t ptr = 1;
    while (ptr <= size) {
        size_t child = ptr * 2;
        if (child > size)
            return;
        if (child + 1 <= size && cmp(heap + child, heap + child + 1))
            child = child + 1;
        if (cmp(heap + ptr, heap + child)) {
            swap(heap + ptr, heap + child);
            ptr = child;
        } else return;
    }
}