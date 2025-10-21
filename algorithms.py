#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
常见算法示例
Common Algorithm Examples for Programming Competitions
"""

def fibonacci(n):
    """
    计算斐波那契数列的第n项
    Calculate the nth Fibonacci number
    
    Args:
        n: 第n项 (nth term)
    
    Returns:
        第n项斐波那契数 (nth Fibonacci number)
    """
    if n <= 1:
        return n
    
    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b

def binary_search(arr, target):
    """
    二分查找算法
    Binary search algorithm
    
    Args:
        arr: 已排序的数组 (sorted array)
        target: 目标值 (target value)
    
    Returns:
        目标值的索引，如果不存在则返回-1 (index of target, or -1 if not found)
    """
    left, right = 0, len(arr) - 1
    
    while left <= right:
        mid = (left + right) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    
    return -1

def bubble_sort(arr):
    """
    冒泡排序算法
    Bubble sort algorithm
    
    Args:
        arr: 待排序数组 (array to be sorted)
    
    Returns:
        排序后的数组 (sorted array)
    """
    n = len(arr)
    result = arr.copy()
    
    for i in range(n):
        swapped = False
        for j in range(0, n - i - 1):
            if result[j] > result[j + 1]:
                result[j], result[j + 1] = result[j + 1], result[j]
                swapped = True
        if not swapped:
            break
    
    return result

def gcd(a, b):
    """
    计算最大公约数（欧几里得算法）
    Calculate Greatest Common Divisor (Euclidean algorithm)
    
    Args:
        a: 第一个数 (first number)
        b: 第二个数 (second number)
    
    Returns:
        最大公约数 (greatest common divisor)
    """
    while b:
        a, b = b, a % b
    return a

def is_prime(n):
    """
    判断是否为质数
    Check if a number is prime
    
    Args:
        n: 待检查的数 (number to check)
    
    Returns:
        如果是质数返回True，否则返回False (True if prime, False otherwise)
    """
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    
    for i in range(3, int(n ** 0.5) + 1, 2):
        if n % i == 0:
            return False
    return True

if __name__ == "__main__":
    # 测试示例 / Test examples
    print("斐波那契数列 / Fibonacci:")
    print(f"F(10) = {fibonacci(10)}")
    
    print("\n二分查找 / Binary Search:")
    arr = [1, 3, 5, 7, 9, 11, 13, 15]
    print(f"在 {arr} 中查找 7: 索引 {binary_search(arr, 7)}")
    
    print("\n冒泡排序 / Bubble Sort:")
    unsorted = [64, 34, 25, 12, 22, 11, 90]
    print(f"排序前: {unsorted}")
    print(f"排序后: {bubble_sort(unsorted)}")
    
    print("\n最大公约数 / GCD:")
    print(f"gcd(48, 18) = {gcd(48, 18)}")
    
    print("\n质数判断 / Prime Check:")
    print(f"17 是质数吗？{is_prime(17)}")
    print(f"20 是质数吗？{is_prime(20)}")
