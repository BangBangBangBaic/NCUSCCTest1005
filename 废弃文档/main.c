#include "sorts.h"
#include <sys/stat.h>

// 比较函数实现
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

// 包装排序函数
void sort_generic_array(GenericArray* array, const char* algorithm) {
    CompareFunc compare;
    
    if (array->type == INT_TYPE) {
        compare = compare_int;
    } else {
        compare = compare_double;
    }
    
    // 根据算法名称调用相应的排序函数
    if (strcmp(algorithm, "quick_basic") == 0) {
        quick_sort_generic(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "quick_median") == 0) {
        quick_sort_median_generic(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "quick_iterative") == 0) {
        quick_sort_iterative_generic(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "merge_serial") == 0) {
        merge_sort_generic(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "merge_parallel") == 0) {
        merge_sort_parallel_generic(array->data, array->size, array->elem_size, compare);
    } else if (strcmp(algorithm, "third_algorithm") == 0) {
        your_third_sort_generic(array->data, array->size, array->elem_size, compare);
    } else {
        printf("未知的排序算法: %s\n", algorithm);
    }
}

// 性能测试函数 - 只关注时间测量
double test_sort_performance(const char* filename, DataType data_type, const char* algorithm) {
    GenericArray array;
    
    // 读取数据
    read_data_from_file(filename, &array, data_type);
    
    // 创建数据副本用于排序
    GenericArray test_array;
    test_array.size = array.size;
    test_array.type = array.type;
    test_array.elem_size = array.elem_size;
    test_array.data = malloc(array.size * array.elem_size);
    memcpy(test_array.data, array.data, array.size * array.elem_size);
    
    // 记录开始时间
    double start_time = omp_get_wtime();
    
    // 执行排序
    sort_generic_array(&test_array, algorithm);
    
    // 记录结束时间
    double end_time = omp_get_wtime();
    double time_spent = end_time - start_time;
    
    // 验证排序结果
    int sorted = is_sorted_generic(&test_array);
    
    // 输出结果
    printf("算法: %-15s 数据: %-20s 大小: %-6d 类型: %-6s 时间: %.6f秒 %s\n", 
           algorithm, filename, array.size, 
           data_type == INT_TYPE ? "int" : "double", 
           time_spent, sorted ? "✓" : "✗");
    
    // 将结果保存到CSV
    FILE* csv_file = fopen("sorting_results.csv", "a");
    if (csv_file) {
        fprintf(csv_file, "%s,%s,%s,%d,%.6f,%s\n", 
                algorithm, 
                data_type == INT_TYPE ? "int" : "double",
                filename, 
                array.size, 
                time_spent,
                sorted ? "success" : "fail");
        fclose(csv_file);
    }
    
    free_generic_array(&test_array);
    free_generic_array(&array);
    
    return time_spent;
}

int main(int argc, char *argv[]) {
    printf("=== 泛型排序算法性能测试 ===\n");
    printf("开始时间: %s", ctime(&(time_t){time(NULL)}));
    
    // 设置随机种子
    srand(time(NULL));
    
    // 创建CSV文件头
    FILE* csv_file = fopen("sorting_results.csv", "w");
    if (csv_file) {
        fprintf(csv_file, "algorithm,data_type,dataset,size,time,status\n");
        fclose(csv_file);
    }
    
    // 测试数据集
    const char* test_files[] = {
        "test_data/small_int_data.txt",
        "test_data/medium_int_data.txt", 
        "test_data/large_int_data.txt",
        "test_data/small_double_data.txt",
        "test_data/medium_double_data.txt",
        "test_data/large_double_data.txt",
        NULL
    };
    
    // 测试所有算法
    const char* algorithms[] = {
        "quick_basic",
        "quick_median", 
        "quick_iterative",
        "merge_serial",
        "merge_parallel",
        "third_algorithm",  // 您的第三个排序算法
        NULL
    };
    
    int total_tests = 0;
    int successful_tests = 0;
    
    // 测试所有文件
    for (int i = 0; test_files[i] != NULL; i++) {
        // 检查文件是否存在
        if (access(test_files[i], F_OK) != 0) {
            printf("文件不存在: %s\n", test_files[i]);
            continue;
        }
        
        // 确定数据类型
        DataType data_type;
        if (strstr(test_files[i], "int") != NULL) {
            data_type = INT_TYPE;
        } else {
            data_type = DOUBLE_TYPE;
        }
        
        // 测试所有排序算法
        for (int j = 0; algorithms[j] != NULL; j++) {
            double time = test_sort_performance(test_files[i], data_type, algorithms[j]);
            total_tests++;
            if (time >= 0) { // 假设时间>=0表示成功
                successful_tests++;
            }
        }
    }
    
    printf("\n=== 测试完成 ===\n");
    printf("完成时间: %s", ctime(&(time_t){time(NULL)}));
    printf("总测试次数: %d, 成功: %d\n", total_tests, successful_tests);
    printf("结果已保存到: sorting_results.csv\n");
    
    return 0;
}