#include "sorts.h"

// 交换函数
static void swap(void* a, void* b, size_t size) {
    char* temp = (char*)malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

// 读取数据文件
void read_data_from_file(const char* filename, GenericArray* array, DataType type) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件: %s\n", filename);
        exit(1);
    }
    
    // 计算数据行数
    int count = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }
    
    // 设置数组属性
    array->size = count;
    array->type = type;
    
    if (type == INT_TYPE) {
        array->elem_size = sizeof(int);
        array->data = malloc(count * sizeof(int));
    } else {
        array->elem_size = sizeof(double);
        array->data = malloc(count * sizeof(double));
    }
    
    // 读取数据
    rewind(file);
    for (int i = 0; i < count; i++) {
        fgets(buffer, sizeof(buffer), file);
        if (type == INT_TYPE) {
            ((int*)array->data)[i] = atoi(buffer);
        } else {
            ((double*)array->data)[i] = atof(buffer);
        }
    }
    
    fclose(file);
}

// 释放数组
void free_generic_array(GenericArray* array) {
    if (array->data != NULL) {
        free(array->data);
        array->data = NULL;
    }
    array->size = 0;
}

// 检查是否已排序
int is_sorted_generic(const GenericArray* array) {
    CompareFunc compare;
    
    if (array->type == INT_TYPE) {
        compare = compare_int;
    } else {
        compare = compare_double;
    }
    
    for (int i = 1; i < array->size; i++) {
        void* elem1 = (char*)array->data + (i-1) * array->elem_size;
        void* elem2 = (char*)array->data + i * array->elem_size;
        
        if (compare(elem1, elem2) > 0) {
            return 0;
        }
    }
    return 1;
}

// 比较函数
int compare_int(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return (int_a > int_b) - (int_a < int_b);
}

int compare_double(const void* a, const void* b) {
    double double_a = *(const double*)a;
    double double_b = *(const double*)b;
    return (double_a > double_b) - (double_a < double_b);
}

// ==================== 快速排序实现 ====================

// 基础分区函数
static int partition(void* base, int low, int high, size_t size, CompareFunc compare) {
    void* pivot = (char*)base + high * size;
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        void* current = (char*)base + j * size;
        if (compare(current, pivot) <= 0) {
            i++;
            void* elem_i = (char*)base + i * size;
            swap(elem_i, current, size);
        }
    }
    
    void* elem_i1 = (char*)base + (i + 1) * size;
    void* elem_high = (char*)base + high * size;
    swap(elem_i1, elem_high, size);
    
    return i + 1;
}

// 基础快速排序
static void quick_sort_helper(void* base, int low, int high, size_t size, CompareFunc compare) {
    if (low < high) {
        int pi = partition(base, low, high, size, compare);
        quick_sort_helper(base, low, pi - 1, size, compare);
        quick_sort_helper(base, pi + 1, high, size, compare);
    }
}

void quick_sort_basic(void* base, size_t num, size_t size, CompareFunc compare) {
    quick_sort_helper(base, 0, num - 1, size, compare);
}

// 三数取中法
static int median_of_three(void* base, int low, int high, size_t size, CompareFunc compare) {
    int mid = low + (high - low) / 2;
    
    void* elem_low = (char*)base + low * size;
    void* elem_mid = (char*)base + mid * size;
    void* elem_high = (char*)base + high * size;
    
    // 对三个元素排序
    if (compare(elem_low, elem_mid) > 0)
        swap(elem_low, elem_mid, size);
    if (compare(elem_low, elem_high) > 0)
        swap(elem_low, elem_high, size);
    if (compare(elem_mid, elem_high) > 0)
        swap(elem_mid, elem_high, size);
    
    // 将中位数放到high-1位置
    swap(elem_mid, (char*)base + (high - 1) * size, size);
    return high - 1;
}

static void quick_sort_median_helper(void* base, int low, int high, size_t size, CompareFunc compare) {
    if (high - low > 1) {
        int pivot_index = median_of_three(base, low, high, size, compare);
        void* pivot = (char*)base + pivot_index * size;
        
        int i = low, j = high - 1;
        while (1) {
            do { i++; } while (compare((char*)base + i * size, pivot) < 0);
            do { j--; } while (compare((char*)base + j * size, pivot) > 0);
            
            if (i < j) {
                swap((char*)base + i * size, (char*)base + j * size, size);
            } else {
                break;
            }
        }
        
        swap((char*)base + i * size, (char*)base + (high - 1) * size, size);
        
        quick_sort_median_helper(base, low, i - 1, size, compare);
        quick_sort_median_helper(base, i + 1, high, size, compare);
    } else if (high - low == 1) {
        void* elem_low = (char*)base + low * size;
        void* elem_high = (char*)base + high * size;
        if (compare(elem_low, elem_high) > 0) {
            swap(elem_low, elem_high, size);
        }
    }
}

void quick_sort_median(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    quick_sort_median_helper(base, 0, num - 1, size, compare);
}

// 随机pivot快速排序
void quick_sort_random(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    
    // 随机选择pivot
    int random_index = rand() % num;
    void* random_elem = (char*)base + random_index * size;
    void* last_elem = (char*)base + (num - 1) * size;
    swap(random_elem, last_elem, size);
    
    quick_sort_helper(base, 0, num - 1, size, compare);
}

// 非递归快速排序
typedef struct {
    int low;
    int high;
} StackItem;

void quick_sort_iterative(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    
    StackItem* stack = (StackItem*)malloc(num * sizeof(StackItem));
    int top = -1;
    
    top++;
    stack[top].low = 0;
    stack[top].high = num - 1;
    
    while (top >= 0) {
        int low = stack[top].low;
        int high = stack[top].high;
        top--;
        
        if (low < high) {
            int pi = partition(base, low, high, size, compare);
            
            if (pi - 1 > low) {
                top++;
                stack[top].low = low;
                stack[top].high = pi - 1;
            }
            
            if (pi + 1 < high) {
                top++;
                stack[top].low = pi + 1;
                stack[top].high = high;
            }
        }
    }
    
    free(stack);
}

// ==================== 归并排序实现 ====================

// 合并函数
static void merge(void* base, int left, int mid, int right, size_t size, CompareFunc compare) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    void* left_arr = malloc(n1 * size);
    void* right_arr = malloc(n2 * size);
    
    memcpy(left_arr, (char*)base + left * size, n1 * size);
    memcpy(right_arr, (char*)base + (mid + 1) * size, n2 * size);
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        void* left_elem = (char*)left_arr + i * size;
        void* right_elem = (char*)right_arr + j * size;
        
        if (compare(left_elem, right_elem) <= 0) {
            memcpy((char*)base + k * size, left_elem, size);
            i++;
        } else {
            memcpy((char*)base + k * size, right_elem, size);
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        memcpy((char*)base + k * size, (char*)left_arr + i * size, size);
        i++;
        k++;
    }
    
    while (j < n2) {
        memcpy((char*)base + k * size, (char*)right_arr + j * size, size);
        j++;
        k++;
    }
    
    free(left_arr);
    free(right_arr);
}

// 串行归并排序辅助函数
static void merge_sort_helper(void* base, int left, int right, size_t size, CompareFunc compare) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_helper(base, left, mid, size, compare);
        merge_sort_helper(base, mid + 1, right, size, compare);
        merge(base, left, mid, right, size, compare);
    }
}

void merge_sort_serial(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    merge_sort_helper(base, 0, num - 1, size, compare);
}

// 并行归并排序辅助函数
static void merge_sort_parallel_helper(void* base, int left, int right, size_t size, CompareFunc compare, int threshold) {
    if (left < right) {
        if (right - left <= threshold) {
            merge_sort_helper(base, left, right, size, compare);
        } else {
            int mid = left + (right - left) / 2;
            
            #pragma omp parallel sections
            {
                #pragma omp section
                merge_sort_parallel_helper(base, left, mid, size, compare, threshold);
                
                #pragma omp section
                merge_sort_parallel_helper(base, mid + 1, right, size, compare, threshold);
            }
            
            merge(base, left, mid, right, size, compare);
        }
    }
}

void merge_sort_parallel(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort_parallel_helper(base, 0, num - 1, size, compare, 1000);
    }
}

// 包装函数
void sort_generic_array(GenericArray* array, const char* algorithm) {
    CompareFunc compare;
    
    if (array->type == INT_TYPE) {
        compare = compare_int;
    } else {
        compare = compare_double;
    }
    
    if (strcmp(algorithm, "quick_basic") == 0) {
        quick_sort_basic(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "quick_median") == 0) {
        quick_sort_median(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "quick_random") == 0) {
        quick_sort_random(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "quick_iterative") == 0) {
        quick_sort_iterative(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "merge_serial") == 0) {
        merge_sort_serial(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "merge_parallel") == 0) {
        merge_sort_parallel(array->data, array->size, array->elem_size, compare);
    } else {
        printf("未知的排序算法: %s\n", algorithm);
    }
}