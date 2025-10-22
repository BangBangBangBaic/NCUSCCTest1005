#!/usr/bin/env python3
# 绘制散点并拟合直线，风格类似用户提供的示例图
import csv
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict

infile = 'sorting_results_example.csv'
outfile = 'time_regression.svg'

# 读取数据
data = defaultdict(lambda: {'sizes': [], 'times': []})
algos_order = []
with open(infile, newline='', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for row in reader:
        algo = row['algorithm']
        size = float(row['size'])
        t = float(row['time'])
        if algo not in algos_order:
            algos_order.append(algo)
        data[algo]['sizes'].append(size)
        data[algo]['times'].append(t)

# plotting
try:
    plt.style.use('seaborn')
except Exception:
    try:
        plt.style.use('ggplot')
    except Exception:
        pass
# configure fonts to support Chinese characters on Windows
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial']
plt.rcParams['axes.unicode_minus'] = False
fig, ax = plt.subplots(figsize=(9,6))
colors = ['red', 'blue', 'black', 'gold', 'green', 'purple']

for i, algo in enumerate(algos_order):
    sizes = np.array(data[algo]['sizes'])
    times = np.array(data[algo]['times'])
    if sizes.size == 0:
        continue
    # scatter
    ax.scatter(sizes, times, s=12, color=colors[i % len(colors)], alpha=0.35)
    # linear fit in original scale
    # Use robust fit: polyfit on log(size) may approximate slope; but sample shows roughly linear
    # Fit degree=1
    coef = np.polyfit(sizes, times, 1)
    poly = np.poly1d(coef)
    xs = np.linspace(min(sizes), max(sizes), 200)
    ys = poly(xs)
    ax.plot(xs, ys, color=colors[i % len(colors)], linewidth=2.2, label=algo)
    # Slightly thicker black edge for one curve to mimic sample (optional)

# aesthetics
ax.set_xlabel('n (数据规模)', fontsize=12)
ax.set_ylabel('运行时间 (秒)', fontsize=12)
ax.set_xlim(left=0)
# set y limit a bit above max
all_times = np.concatenate([np.array(data[a]['times']) for a in algos_order if data[a]['times']])
ax.set_ylim(0, max(all_times) * 1.15)
# legend with box
leg = ax.legend(loc='upper left', frameon=True, fontsize=10)
leg.get_frame().set_facecolor('white')
leg.get_frame().set_edgecolor('#d0d0d0')

ax.ticklabel_format(style='plain', axis='x')
plt.tight_layout()
plt.savefig(outfile, format='svg')
print('Wrote', outfile)
