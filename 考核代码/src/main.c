#include "sorts.h"
#include <sys/stat.h>

// 性能测试函数
void test_sort_performance(const char* filename, DataType data_type, const char* algorithm) {
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
    printf("算法: %-15s 数据: %-25s 大小: %-6d 类型: %-6s 时间: %.6f秒 %s\n", 
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
        "test_data/xlarge_int_data.txt",
        "test_data/small_double_data.txt",
        "test_data/medium_double_data.txt",
        "test_data/large_double_data.txt",
        "test_data/xlarge_double_data.txt",
        NULL
    };
    
    // 测试所有算法
    const char* algorithms[] = {
        "quick_basic",
        "quick_median", 
        "quick_random",
        "quick_iterative",
        "merge_serial",
        "merge_parallel",
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
            test_sort_performance(test_files[i], data_type, algorithms[j]);
            total_tests++;
        }
    }
    
    printf("\n=== 测试完成 ===\n");
    printf("完成时间: %s", ctime(&(time_t){time(NULL)}));
    printf("总测试次数: %d\n", total_tests);
    printf("结果已保存到: sorting_results.csv\n");
    
    return 0;
}