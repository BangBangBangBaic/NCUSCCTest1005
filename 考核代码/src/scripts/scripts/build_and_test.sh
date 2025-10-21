#!/bin/bash

echo "=== 排序算法性能测试 ==="

# 检查是否在项目根目录
if [ ! -d "src" ] || [ ! -d "scripts" ]; then
    echo "错误: 请在项目根目录运行此脚本"
    echo "项目结构:"
    echo "  sorting_project/"
    echo "  ├── src/          # 源代码"
    echo "  ├── scripts/      # 脚本文件" 
    echo "  └── test_data/    # 测试数据"
    exit 1
fi

# 生成测试数据
echo "1. 生成测试数据..."
python3 scripts/generate_data.py

echo "2. 编译排序程序..."
# 清理之前的结果文件
rm -f sorting_results.csv
rm -f results_*.csv
rm -f all_results.csv

# 定义优化级别
OPT_LEVELS=("-O0" "-O1" "-O2" "-O3" "-Ofast")

for opt in "${OPT_LEVELS[@]}"; do
    echo ""
    echo "========================================"
    echo "使用优化级别: $opt 进行编译"
    echo "========================================"
    
    # 编译程序 (启用OpenMP)
    gcc $opt -fopenmp -o "sort_test_${opt}" \
        src/main.c \
        src/sorts.c \
        -lm
    
    if [ $? -eq 0 ]; then
        echo "✅ 编译成功: sort_test_${opt}"
        
        # 运行测试
        echo "运行测试..."
        ./"sort_test_${opt}"
        
        # 重命名结果文件以包含优化级别
        if [ -f "sorting_results.csv" ]; then
            # 添加优化级别列
            sed -i "1s/^/optimization,/" sorting_results.csv
            sed -i "2,\$s/^/$opt,/" sorting_results.csv
            mv sorting_results.csv "results_${opt}.csv"
            echo "结果保存到: results_${opt}.csv"
        fi
    else
        echo "❌ 编译失败: $opt"
    fi
done

echo ""
echo "3. 合并所有结果..."
# 合并所有CSV文件
echo "optimization,algorithm,data_type,dataset,size,time,status" > all_results.csv
for file in results_*.csv; do
    if [ "$file" != "all_results.csv" ]; then
        tail -n +2 "$file" >> all_results.csv
    fi
done

echo "所有测试完成！最终结果保存在 all_results.csv"