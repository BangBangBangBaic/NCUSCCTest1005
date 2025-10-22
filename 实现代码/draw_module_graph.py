#!/usr/bin/env python3
"""
生成一个简单的模块/文件依赖关系图并输出为 SVG（纯文本 SVG，无外部依赖）
输出文件: module_graph.svg
"""

nodes = [
    ("main.c", 400, 60),
    ("sorts.h", 400, 170),
    ("quicksort1.c", 120, 300),
    ("quicksort2.c", 400, 300),
    ("mencysort.c", 680, 300),
    ("random.c", 260, 420),
    ("run_sorts.c", 540, 60)
]

# edges: (from, to)
edges = [
    ("main.c", "sorts.h"),
    ("run_sorts.c", "quicksort1.c"),
    ("quicksort1.c", "sorts.h"),
    ("quicksort2.c", "sorts.h"),
    ("mencysort.c", "sorts.h"),
    ("quicksort1.c", "random.c"),
    ("quicksort2.c", "random.c"),
]

node_map = {name: (x, y) for (name, x, y) in nodes}

W = 800
H = 520

def svg_header(w, h):
    return f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}" viewBox="0 0 {w} {h}">\n'

def defs():
    return ("<defs>\n"
            "  <marker id=\"arrow\" viewBox=\"0 0 10 10\" refX=\"10\" refY=\"5\" markerWidth=\"6\" markerHeight=\"6\" orient=\"auto-start-reverse\">\n"
            "    <path d=\"M 0 0 L 10 5 L 0 10 z\" fill=\"#333\" />\n"
            "  </marker>\n"
            "</defs>\n")

def draw_edge(sx, sy, tx, ty):
    # draw a curved path
    mx = (sx + tx) / 2
    my = (sy + ty) / 2
    dx = tx - sx
    dy = ty - sy
    # small control offset
    cx1 = sx + dx * 0.25 - dy * 0.08
    cy1 = sy + dy * 0.25 + dx * 0.06
    cx2 = sx + dx * 0.75 - dy * 0.06
    cy2 = sy + dy * 0.75 + dx * 0.08
    path = f'<path d="M {sx} {sy} C {cx1} {cy1}, {cx2} {cy2}, {tx} {ty}" stroke="#333" stroke-width="2" fill="none" marker-end="url(#arrow)" />\n'
    return path

def draw_node(name, x, y):
    w = 160
    h = 44
    rx = 8
    left = x - w/2
    top = y - h/2
    rect = f'<rect x="{left}" y="{top}" width="{w}" height="{h}" rx="{rx}" ry="{rx}" fill="#f7f7f9" stroke="#333" stroke-width="1.5" />\n'
    text_x = x
    text_y = y + 6
    text = f'<text x="{text_x}" y="{text_y}" font-family="Segoe UI, Arial, sans-serif" font-size="14" text-anchor="middle" fill="#111">{name}</text>\n'
    return rect + text

svg = []
svg.append(svg_header(W, H))
svg.append(defs())
svg.append('<rect width="100%" height="100%" fill="#ffffff" />\n')

# draw edges first (so nodes are on top)
for a, b in edges:
    if a in node_map and b in node_map:
        ax, ay = node_map[a]
        bx, by = node_map[b]
        # adjust edge endpoints to node border
        # simple adjustment: point to top of target rect and bottom of source rect
        sx = ax
        sy = ay + 22
        tx = bx
        ty = by - 22
        svg.append(draw_edge(sx, sy, tx, ty))

# draw nodes
for name, x, y in nodes:
    svg.append(draw_node(name, x, y))

svg.append('</svg>\n')

out = ''.join(svg)
with open('module_graph.svg', 'w', encoding='utf-8') as f:
    f.write(out)

print('Wrote module_graph.svg')
