#ifndef SORTS_H
#define SORTS_H

#include <stddef.h>

typedef int (*CompareFunc)(const void*, const void*);

void quick_sort_generic(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort_median_generic(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort_iterative_generic(void* base, size_t num, size_t size, CompareFunc compare);
void merge_sort_generic(void* base, size_t num, size_t size, CompareFunc compare);
void merge_sort_parallel_generic(void* base, size_t num, size_t size, CompareFunc compare);
void your_third_sort_generic(void* base, size_t num, size_t size, CompareFunc compare);

#endif
