#!/usr/bin/env python3
import random
import os

def generate_test_data():
    """生成整型和浮点数的测试数据"""
    
    datasets = {
        'small': 1000,      # 1,000条数据
        'medium': 10000,    # 10,000条数据  
        'large': 100000,    # 100,000条数据
        'xlarge': 500000    # 500,000条数据
    }
    
    # 创建测试数据目录
    if not os.path.exists('test_data'):
        os.makedirs('test_data')
        print("创建 test_data 目录")
    
    # 生成整型数据
    for name, size in datasets.items():
        filename = f'test_data/{name}_int_data.txt'
        
        print(f"生成 {name} 整型数据集，大小: {size}")
        
        with open(filename, 'w') as f:
            for i in range(size):
                num = random.randint(-100000, 100000)
                f.write(f"{num}\n")
        
        print(f"  已保存: {filename}")
    
    # 生成浮点数数据
    for name, size in datasets.items():
        filename = f'test_data/{name}_double_data.txt'
        
        print(f"生成 {name} 浮点数数据集，大小: {size}")
        
        with open(filename, 'w') as f:
            for i in range(size):
                num = random.uniform(-100000.0, 100000.0)
                num = round(num, 4)  # 保留4位小数
                f.write(f"{num}\n")
        
        print(f"  已保存: {filename}")
    
    print("\n✅ 所有测试数据生成完成！")

if __name__ == "__main__":
    generate_test_data()