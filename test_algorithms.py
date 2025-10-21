#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
算法测试文件
Test file for algorithms
"""

import unittest
from algorithms import fibonacci, binary_search, bubble_sort, gcd, is_prime

class TestAlgorithms(unittest.TestCase):
    """算法测试类 / Algorithm test class"""
    
    def test_fibonacci(self):
        """测试斐波那契数列 / Test Fibonacci sequence"""
        self.assertEqual(fibonacci(0), 0)
        self.assertEqual(fibonacci(1), 1)
        self.assertEqual(fibonacci(5), 5)
        self.assertEqual(fibonacci(10), 55)
        self.assertEqual(fibonacci(15), 610)
    
    def test_binary_search(self):
        """测试二分查找 / Test binary search"""
        arr = [1, 3, 5, 7, 9, 11, 13, 15]
        self.assertEqual(binary_search(arr, 7), 3)
        self.assertEqual(binary_search(arr, 1), 0)
        self.assertEqual(binary_search(arr, 15), 7)
        self.assertEqual(binary_search(arr, 4), -1)
        self.assertEqual(binary_search([], 5), -1)
    
    def test_bubble_sort(self):
        """测试冒泡排序 / Test bubble sort"""
        self.assertEqual(bubble_sort([64, 34, 25, 12, 22, 11, 90]), 
                        [11, 12, 22, 25, 34, 64, 90])
        self.assertEqual(bubble_sort([5, 4, 3, 2, 1]), [1, 2, 3, 4, 5])
        self.assertEqual(bubble_sort([1]), [1])
        self.assertEqual(bubble_sort([]), [])
        self.assertEqual(bubble_sort([1, 2, 3]), [1, 2, 3])
    
    def test_gcd(self):
        """测试最大公约数 / Test GCD"""
        self.assertEqual(gcd(48, 18), 6)
        self.assertEqual(gcd(100, 50), 50)
        self.assertEqual(gcd(17, 13), 1)
        self.assertEqual(gcd(0, 5), 5)
        self.assertEqual(gcd(12, 8), 4)
    
    def test_is_prime(self):
        """测试质数判断 / Test prime check"""
        self.assertTrue(is_prime(2))
        self.assertTrue(is_prime(3))
        self.assertTrue(is_prime(5))
        self.assertTrue(is_prime(17))
        self.assertTrue(is_prime(97))
        self.assertFalse(is_prime(0))
        self.assertFalse(is_prime(1))
        self.assertFalse(is_prime(4))
        self.assertFalse(is_prime(20))
        self.assertFalse(is_prime(100))

if __name__ == "__main__":
    print("运行算法测试... / Running algorithm tests...")
    unittest.main(verbosity=2)
