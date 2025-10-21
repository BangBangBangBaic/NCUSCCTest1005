#ifndef SORTS_H
#define SORTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

// 数据类型枚举
typedef enum {
    INT_TYPE,
    DOUBLE_TYPE
} DataType;

// 通用数据容器
typedef struct {
    void* data;        // 使用void*指向数据
    int size;          // 数据数量
    DataType type;     // 数据类型
    size_t elem_size;  // 元素大小
} GenericArray;

// 比较函数类型定义
typedef int (*CompareFunc)(const void*, const void*);

// 基础函数
void read_data_from_file(const char* filename, GenericArray* array, DataType type);
void free_generic_array(GenericArray* array);
int is_sorted_generic(const GenericArray* array);

// 比较函数
int compare_int(const void* a, const void* b);
int compare_double(const void* a, const void* b);

// 快速排序函数
void quick_sort_basic(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort_median(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort_random(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort_iterative(void* base, size_t num, size_t size, CompareFunc compare);

// 归并排序函数
void merge_sort_serial(void* base, size_t num, size_t size, CompareFunc compare);
void merge_sort_parallel(void* base, size_t num, size_t size, CompareFunc compare);

// 包装函数
void sort_generic_array(GenericArray* array, const char* algorithm);

#endif