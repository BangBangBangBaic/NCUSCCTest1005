#!/usr/bin/env python3
import csv
from collections import defaultdict
import math

infile = 'sorting_results_example.csv'
outfile = 'time_chart.svg'

# 读取数据
data = defaultdict(lambda: {})  # data[algorithm][size] = time
sizes = set()
algos = []
with open(infile, newline='', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for row in reader:
        algo = row['algorithm']
        size = int(row['size'])
        t = float(row['time'])
        if algo not in algos:
            algos.append(algo)
        data[algo][size] = t
        sizes.add(size)

sizes = sorted(sizes)

# SVG setup
W = 1000
H = 600
margin = 80
chart_w = W - margin*2
chart_h = H - margin*2

# scale functions
min_size = min(sizes)
max_size = max(sizes)
min_time = min(min(d.get(s, float('inf')) for s in sizes) for d in data.values())
max_time = max(max(d.get(s, 0.0) for s in sizes) for d in data.values())
if min_time == float('inf'):
    min_time = 0.0

# Use log scale for x (size) and linear for y (time)
import math

def sx(x):
    # log scale for x
    lx = math.log10(x)
    lx_min = math.log10(min_size)
    lx_max = math.log10(max_size)
    return margin + (lx - lx_min) / (lx_max - lx_min) * chart_w

def sy(t):
    # linear scale for y, inverted
    if max_time == min_time:
        return margin + chart_h
    return margin + chart_h - (t - min_time) / (max_time - min_time) * chart_h

# colors
colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b']

svg = []
svg.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">\n')
svg.append('<rect width="100%" height="100%" fill="#fff" />\n')
svg.append('<style>text{font-family:Arial, Helvetica, sans-serif;}</style>\n')

# axes
svg.append(f'<line x1="{margin}" y1="{margin+chart_h}" x2="{margin+chart_w}" y2="{margin+chart_h}" stroke="#000"/>\n')
svg.append(f'<line x1="{margin}" y1="{margin}" x2="{margin}" y2="{margin+chart_h}" stroke="#000"/>\n')

# x ticks (log scale)
xticks = sizes
for s in xticks:
    x = sx(s)
    svg.append(f'<line x1="{x}" y1="{margin+chart_h}" x2="{x}" y2="{margin+chart_h+6}" stroke="#000"/>\n')
    svg.append(f'<text x="{x}" y="{margin+chart_h+22}" font-size="12" text-anchor="middle">{s}</text>\n')

# y ticks (more granular: 10 intervals -> 11 ticks)
ticks = 10
for i in range(ticks + 1):
    v = min_time + (max_time - min_time) * i / ticks
    y = sy(v)
    # faint grid line for readability
    svg.append(f'<line x1="{margin}" y1="{y}" x2="{margin+chart_w}" y2="{y}" stroke="#888" stroke-opacity="0.15"/>\n')
    svg.append(f'<line x1="{margin-6}" y1="{y}" x2="{margin}" y2="{y}" stroke="#000"/>\n')
    svg.append(f'<text x="{margin-10}" y="{y+4}" font-size="12" text-anchor="end">{v:.6f}</text>\n')

# plot lines
for idx, algo in enumerate(algos):
    color = colors[idx % len(colors)]
    points = []
    for s in sizes:
        t = data[algo].get(s, None)
        if t is None:
            continue
        x = sx(s)
        y = sy(t)
        points.append((x, y))
    if not points:
        continue
    path = 'M ' + ' L '.join(f'{x:.2f} {y:.2f}' for x, y in points)
    svg.append(f'<path d="{path}" stroke="{color}" stroke-width="2" fill="none" />\n')
    # markers
    for (x, y) in points:
        svg.append(f'<circle cx="{x:.2f}" cy="{y:.2f}" r="3" fill="{color}" />\n')
    # legend entry
    lx = margin + chart_w + 20
    ly = margin + 20 + idx*20
    svg.append(f'<rect x="{lx}" y="{ly-12}" width="12" height="12" fill="{color}" />\n')
    svg.append(f'<text x="{lx+18}" y="{ly}" font-size="12">{algo}</text>\n')

# labels
svg.append(f'<text x="{W/2}" y="{H-16}" font-size="14" text-anchor="middle">数据规模 (n)</text>\n')
svg.append(f'<text x="{20}" y="{H/2}" font-size="14" transform="rotate(-90 20,{H/2})" text-anchor="middle">运行时间 (秒)</text>\n')

svg.append('</svg>\n')

with open(outfile, 'w', encoding='utf-8') as f:
    f.write(''.join(svg))

print('Wrote', outfile)
