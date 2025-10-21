#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from scipy import stats
import os

# 设置中文字体和样式
plt.rcParams['font.sans-serif'] = ['DejaVu Sans', 'Arial']
plt.rcParams['axes.unicode_minus'] = False
sns.set_style("whitegrid")

def comprehensive_analysis():
    """全面的性能分析"""
    
    if not os.path.exists('all_results.csv'):
        print("错误: 未找到 all_results.csv 文件")
        print("请先运行 build_and_test.sh 生成测试数据")
        return
    
    # 读取数据
    df = pd.read_csv('all_results.csv')
    
    print("=== 排序算法性能分析报告 ===")
    print(f"数据记录数: {len(df)}")
    print(f"测试的优化级别: {sorted(df['optimization'].unique())}")
    print(f"测试的算法: {sorted(df['algorithm'].unique())}")
    print(f"数据类型: {sorted(df['data_type'].unique())}")
    print(f"数据规模: {sorted(df['size'].unique())}")
    
    # 创建可视化图表
    create_comprehensive_charts(df)
    
    # 时间复杂度分析
    analyze_time_complexity(df)
    
    # 生成详细报告
    generate_detailed_report(df)

def create_comprehensive_charts(df):
    """创建综合图表"""
    
    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    fig.suptitle('Sorting Algorithm Performance Analysis', fontsize=16, fontweight='bold')
    
    # 图表1: 整型数据性能对比
    ax1 = axes[0, 0]
    int_data = df[df['data_type'] == 'int']
    if not int_data.empty:
        pivot_int = int_data.pivot_table(values='time', index='size', columns='algorithm', aggfunc='mean')
        pivot_int.plot(ax=ax1, marker='o', linewidth=2)
        ax1.set_xlabel('Data Size')
        ax1.set_ylabel('Sorting Time (seconds)')
        ax1.set_title('Integer Data Sorting Performance')
        ax1.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
        ax1.grid(True, alpha=0.3)
        ax1.set_xscale('log')
        ax1.set_yscale('log')
    
    # 图表2: 浮点数数据性能对比
    ax2 = axes[0, 1]
    double_data = df[df['data_type'] == 'double']
    if not double_data.empty:
        pivot_double = double_data.pivot_table(values='time', index='size', columns='algorithm', aggfunc='mean')
        pivot_double.plot(ax=ax2, marker='s', linewidth=2)
        ax2.set_xlabel('Data Size')
        ax2.set_ylabel('Sorting Time (seconds)')
        ax2.set_title('Double Data Sorting Performance')
        ax2.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
        ax2.grid(True, alpha=0.3)
        ax2.set_xscale('log')
        ax2.set_yscale('log')
    
    # 图表3: 优化级别效果
    ax3 = axes[0, 2]
    optimization_effect = df.groupby(['optimization', 'size'])['time'].mean().unstack('optimization')
    if not optimization_effect.empty:
        optimization_effect.plot(ax=ax3, marker='^', linewidth=2)
        ax3.set_xlabel('Data Size')
        ax3.set_ylabel('Average Sorting Time (seconds)')
        ax3.set_title('Compiler Optimization Effects')
        ax3.legend(title='Optimization')
        ax3.grid(True, alpha=0.3)
        ax3.set_xscale('log')
        ax3.set_yscale('log')
    
    # 图表4: 大数据集性能排名
    ax4 = axes[1, 0]
    large_data = df[df['size'] >= 100000]
    if not large_data.empty:
        best_performance = large_data.groupby(['algorithm', 'data_type'])['time'].mean().unstack('data_type')
        if not best_performance.empty:
            x = np.arange(len(best_performance))
            width = 0.35
            
            bars1 = ax4.bar(x - width/2, best_performance['int'], width, label='Integer', alpha=0.7)
            bars2 = ax4.bar(x + width/2, best_performance['double'], width, label='Double', alpha=0.7)
            
            ax4.set_xlabel('Sorting Algorithm')
            ax4.set_ylabel('Average Time (seconds)')
            ax4.set_title('Large Dataset Performance (≥100,000)')
            ax4.set_xticks(x)
            ax4.set_xticklabels(best_performance.index, rotation=45)
            ax4.legend()
            ax4.grid(True, alpha=0.3)
    
    # 图表5: 并行化效果
    ax5 = axes[1, 1]
    parallel_data = df[df['algorithm'] == 'merge_parallel']
    serial_data = df[df['algorithm'] == 'merge_serial']
    
    if not parallel_data.empty and not serial_data.empty:
        speedup_data = []
        for size in df['size'].unique():
            for data_type in df['data_type'].unique():
                parallel_time = parallel_data[
                    (parallel_data['size'] == size) & 
                    (parallel_data['data_type'] == data_type)
                ]['time'].mean()
                
                serial_time = serial_data[
                    (serial_data['size'] == size) & 
                    (serial_data['data_type'] == data_type)
                ]['time'].mean()
                
                if not np.isnan(parallel_time) and not np.isnan(serial_time):
                    speedup = serial_time / parallel_time
                    speedup_data.append({
                        'size': size,
                        'data_type': data_type,
                        'speedup': speedup
                    })
        
        if speedup_data:
            speedup_df = pd.DataFrame(speedup_data)
            sns.boxplot(data=speedup_df, x='data_type', y='speedup', ax=ax5)
            ax5.set_xlabel('Data Type')
            ax5.set_ylabel('Speedup Ratio')
            ax5.set_title('Parallel Merge Sort Speedup')
            ax5.axhline(y=1, color='red', linestyle='--', alpha=0.5)
    
    # 图表6: pivot选择策略比较
    ax6 = axes[1, 2]
    quick_algorithms = ['quick_basic', 'quick_median', 'quick_random']
    quick_data = df[df['algorithm'].isin(quick_algorithms)]
    
    if not quick_data.empty:
        pivot_comparison = quick_data.groupby(['algorithm', 'size'])['time'].mean().unstack('algorithm')
        if not pivot_comparison.empty:
            pivot_comparison.plot(ax=ax6, marker='D', linewidth=2)
            ax6.set_xlabel('Data Size')
            ax6.set_ylabel('Sorting Time (seconds)')
            ax6.set_title('Quick Sort Pivot Strategy Comparison')
            ax6.legend(title='Pivot Strategy')
            ax6.grid(True, alpha=0.3)
            ax6.set_xscale('log')
            ax6.set_yscale('log')
    
    plt.tight_layout()
    plt.savefig('comprehensive_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()

def analyze_time_complexity(df):
    """分析时间复杂度"""
    
    print("\n=== Time Complexity Analysis ===")
    
    algorithms = df['algorithm'].unique()
    optimizations = df['optimization'].unique()
    data_types = df['data_type'].unique()
    
    complexity_results = []
    
    for algo in algorithms:
        for opt in optimizations:
            for data_type in data_types:
                algo_data = df[
                    (df['algorithm'] == algo) & 
                    (df['optimization'] == opt) & 
                    (df['data_type'] == data_type)
                ]
                
                if len(algo_data) >= 3:
                    sizes = algo_data['size'].unique()
                    if len(sizes) >= 3:
                        # 计算平均时间
                        avg_times = []
                        for size in sorted(sizes):
                            time_avg = algo_data[algo_data['size'] == size]['time'].mean()
                            avg_times.append(time_avg)
                        
                        # 对数线性回归估计复杂度
                        log_sizes = np.log(sorted(sizes))
                        log_times = np.log(avg_times)
                        
                        try:
                            slope, intercept, r_value, p_value, std_err = stats.linregress(log_sizes, log_times)
                            
                            complexity_results.append({
                                'algorithm': algo,
                                'optimization': opt,
                                'data_type': data_type,
                                'empirical_complexity': slope,
                                'r_squared': r_value**2,
                                'data_points': len(sizes)
                            })
                            
                            # 理论复杂度对比
                            theoretical = 1.0  # O(n log n) 在对数坐标中斜率约为1
                            difference = abs(slope - theoretical)
                            
                            complexity_type = "O(n log n)" if difference < 0.3 else f"O(n^{slope:.2f})"
                            
                            print(f"{algo:15} ({opt:5}, {data_type:6}): "
                                  f"Empirical O(n^{slope:.3f}), Inferred {complexity_type}, R²={r_value**2:.3f}")
                            
                        except:
                            continue
    
    # 保存复杂度分析结果
    if complexity_results:
        complexity_df = pd.DataFrame(complexity_results)
        complexity_df.to_csv('complexity_analysis.csv', index=False)
        print("Complexity analysis results saved to: complexity_analysis.csv")

def generate_detailed_report(df):
    """生成详细报告"""
    
    print("\n=== Detailed Performance Report ===")
    
    # 最佳性能排名
    print("\n1. Overall Performance Ranking (Average Time):")
    print("-" * 50)
    overall_ranking = df.groupby('algorithm')['time'].mean().sort_values()
    for i, (algo, avg_time) in enumerate(overall_ranking.items(), 1):
        print(f"{i:2}. {algo:20} {avg_time:.4f}s")
    
    # 数据类型性能差异
    print("\n2. Data Type Performance Differences:")
    print("-" * 50)
    type_comparison = df.groupby(['algorithm', 'data_type'])['time'].mean().unstack('data_type')
    if 'int' in type_comparison.columns and 'double' in type_comparison.columns:
        type_comparison['difference'] = type_comparison['double'] - type_comparison['int']
        type_comparison['difference_pct'] = (type_comparison['difference'] / type_comparison['int']) * 100
        
        for algo, row in type_comparison.iterrows():
            print(f"{algo:20}: Integer {row['int']:.4f}s, "
                  f"Double {row['double']:.4f}s, Difference {row['difference_pct']:+.1f}%")
    
    # 优化效果分析
    print("\n3. Compiler Optimization Effects (Speedup vs -O0):")
    print("-" * 50)
    base_times = df[df['optimization'] == '-O0'].groupby('algorithm')['time'].mean()
    
    for opt in ['-O1', '-O2', '-O3', '-Ofast']:
        opt_data = df[df['optimization'] == opt]
        if not opt_data.empty:
            opt_times = opt_data.groupby('algorithm')['time'].mean()
            
            speedups = []
            for algo in base_times.index:
                if algo in opt_times.index:
                    speedup = base_times[algo] / opt_times[algo]
                    speedups.append(speedup)
            
            if speedups:
                avg_speedup = np.mean(speedups)
                print(f"{opt:6}: Average speedup {avg_speedup:.2f}x")
    
    # 大数据集推荐
    print("\n4. Recommended Algorithms for Large Datasets (≥100,000):")
    print("-" * 50)
    large_data = df[df['size'] >= 100000]
    if not large_data.empty:
        best_large = large_data.loc[large_data.groupby(['algorithm', 'data_type'])['time'].idxmin()]
        best_large = best_large.nsmallest(10, 'time')
        
        for i, (idx, row) in enumerate(best_large.iterrows(), 1):
            print(f"{i:2}. {row['algorithm']:20} ({row['data_type']:6}, {row['optimization']:5}): "
                  f"{row['time']:.4f}s")
    
    # 保存详细统计
    detailed_stats = df.groupby(['algorithm', 'data_type', 'optimization', 'size'])['time'].agg([
        'mean', 'std', 'count'
    ]).round(6)
    detailed_stats.to_csv('detailed_statistics.csv')
    
    print(f"\nDetailed statistics saved to: detailed_statistics.csv")
    print(f"Comprehensive analysis chart saved to: comprehensive_analysis.png")

if __name__ == "__main__":
    comprehensive_analysis()