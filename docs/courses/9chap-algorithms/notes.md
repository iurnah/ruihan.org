# 9 Chapter Algorithms

## Lecture 1 从strStr谈面试技巧与Coding Style

|  # |                Problem                |                         Comment                        |     Date    |
|:--:|:--------------------------------------|:-------------------------------------------------------|:-----------:|
| 1  | Implement strStr()                    |                                                        | 08/07/2017, |
| 2  | Subsets                               | No duplicates                                          | 08/07/2017, |
| 3  | Subsets II                            | With duplicates                                        | 08/07/2017, |
| 4  | Permutations                          | No duplicates                                          | 08/07/2017, |
| 5  | Permutations II                       | with duplicates                                        | 08/07/2017, |
| 6  | Combinations                          | from {1, ... n}, choose k of them                      | 08/07/2017, |
| 7  | Combination Sum                       | No duplicate, repeat selection allowed                 | 08/09/2017, |
| 8  | Combination Sum II                    | With duplicates, each can be select once               | 08/09/2017, |
| 9  | Combination Sum III                   | {1, ..., 9} select k numbers and add to n, (See K Sum) | 08/09/2017, |
| 10 | Combination Sum IV                    | (Same as Backpack VI)                                  | 08/09/2017, |
| 11 | Letter Combinations of a Phone Number |                                                        | 08/07/2017, |
| 12 | Palindrome Partitioning               |                                                        | 08/07/2017, |
| 13 | Palindrome Partitioning II            | C++版本如何通过大的Input                               | 08/07/2017, |
| 14 | Restore IP Address                    |                                                        | 08/07/2017, |

### Notes

1. strStr() problem
    - 不用那么的复杂的算法，但要做到bug free（检查corner case, base
      case, 程序缩进）。
2. 面试官
    - 代码工整 （形成自己的style）
    - coding习惯（corner test case，extreme case考虑到了没）
    - 沟通能力，交流障碍有吗
3. 面试考察基本功
    - 程序风格（缩进，括号，变量名）
    - Coding习惯（异常检查，边界处理）
    - 沟通（让面试官时刻明白你的意图）
    - 测试（主动写出合理的Testcase）
4. 如何准备面试
    - 如何一个月搞定算法面试？总结归类相似题目
    - 找出同类题型的模板程序
5. 拿offer四大法宝
    1. 摆正心态，尽可能多地刷题，不急于求成，做完一类再来下一类
    2. 愉快交流，
    3. 理解而非背诵，不记具体实现记这一类题通用思维方式和分析技巧。
    4. 时间要用在刀刃上，不要把时间浪费在那些基本不会考你又很心虚的内容（比如KMP，红黑树，AVL，ACM竞赛题）

## Lecture 2 二分搜索与旋转排序数组 Binary Search & Rotated Sorted Array

|  # |                 Problem                 |                  Comment                 |     Date    |
|:--:|:----------------------------------------|:-----------------------------------------|:-----------:|
| 1  | Classical Binary Search*                |                                          | 08/12/2017, |
| 2  | Search for a Range                      |                                          | 08/12/2017, |
| 3  | Search Insert Position                  |                                          | 08/12/2017, |
| 4  | Search in a Big Sorted Array            |                                          | 08/12/2017, |
| 5  | Search a 2D Matrix                      |                                          | 08/12/2017, |
| 6  | Search a 2D Matrix II                   |                                          | 08/15/2017, |
| 7  | First Bad Version                       |                                          | 08/15/2017, |
| 8  | Find Peak Element                       | Google, important, mit algorithm course. | 08/15/2017, |
| 9  | Recover Rotated Sorted Array*           |                                          | 08/15/2017, |
| 10 | Find Minimum in Rotated Sorted Array    |                                          | 08/15/2017, |
| 11 | Find Minimum in Rotated Sorted Array II |                                          | 08/15/2017, |
| 12 | Search in Rotated Sorted Array          |                                          | 08/15/2017, |
| 13 | Search in Rotated Sorted Array II       |                                          | 08/15/2017, |
| 14 | Median of Two Sorted Arrays             |                                          | 08/15/2017, |
| 15 | Rotate String*                          |                                          | 08/15/2017, |
| 16 | Reverse Words in a String               |                                          | 08/15/2017, |
| 17 | Wood Cut*                               |                                          | 08/15/2017, |

### Notes

1. Classical Binary Search
    - Given an sorted integer array - nums, and an integer - target.
      Find the <u>**any/first/last**</u> position of target in nums,
      return -1 if target doesn’t exist.
    - Prototype: `int binarySearch(int[] nums, int target)`
2. 复杂度分析
    1. T(n) = T(n/2) + O(1) = O(logn)
3. 算法面试中如果需要优化O(n)的时间复杂度那么只能是O(logn)的二分法
4. Recursion or While-loop?
    - bad for recursion: Linux have only 8MB stack, local variable
      takes much space for each level of recursion.
    - for simple problem, use iteration instead of recursion. for more
      complex problem use recursion. ask interviewer.
5. Binary Search模板四要素
    1. start + 1 &lt; end
    2. start + (end - start) / 2
    3. A\[mid\] == , &lt; ,&gt;
    4. A\[start\] A\[end\] ? target
6. Find First Position of XXX VS. Find Last Position of XXX
    1. character of binary search
    2. the split depends on you need the first position of XXX or last
       position of XXX. See the code.

## Lecture 3 二叉树问题与分治算法 Binary Tree & Divide Conquer Algorithm

|  # |                  Problem                 |                                Comment                                |     Date    |
|:--:|:-----------------------------------------|:----------------------------------------------------------------------|:-----------:|
| 1  | Binary Tree Preorder Traversal           | stack, right push first                                               | 08/26/2017, |
| 2  | Maximum Depth of Binary Tree             | Recursion, same as find height of the tree                            | 08/26/2017, |
| 3  | Balanced Binary Tree                     | find height first                                                     | 08/26/2017, |
| 4  | Lowest Common Ancestor of a Binary Tree  | Lowest Common Ancestor of a Binary Search Tree                        | 08/26/2017, |
| 5  | Binary Tree Maximum Path Sum             | "at least one node"                                                   | 08/26/2017, |
| 6  | Binary Tree Level Order Traversal        | BFS, DFS, queue                                                       | 08/26/2017, |
| 7  | Binary Tree Level Order Traversal II     | call reverse() when done by Binary Tree Level Order Traversal         | 08/26/2017, |
| 8  | Binary Tree Zigzag Level Order Traversal | use two stacks                                                        | 08/26/2017, |
| 9  | Validate Binary Search Tree              | inorder traversal, LONG_MAX, LONG_MIN                                 | 08/26/2017, |
| 10 | Search Range in a Binary Search Tree*    | inorder traversal                                                     | 08/26/2017, |
| 11 | Insert a Node in Binary Search Tree*     | recursion                                                             | 08/26/2017, |
| 12 | Binary Search Tree Iterator              | use stack                                                             | 08/26/2017, |
| 13 | Remove Node in Binary Search Tree*       | Problem Inorder Successor BST                                         | 08/26/2017, |
| 14 | Inorder Successor BST                    | Successor of a node, special case: [2,1],p=1, [3,1,null,null,2], p=2. | 08/26/2017, |

### Notes

1. 构造binary tree 唯一或者不唯一
   - preorder + inorder
   - postorder + inorder
   - preorder + postorder 不唯一
2. divide conquer solve preorder traversal O(n^2)
3. Max Path Sum
   - root -> leaf
   - root -> any node.
   - any node -> any node 存在负数节点，这条路径至少包含一个结点

## Lecture 4 动态规划 Dynamic Programming I

| # |      Problem     |           Comment           |     Date    |
|:-:|:-----------------|:----------------------------|:-----------:|
| 1 | Triangle         |                             | 08/29/2017, |
| 2 | Minimum Path Sum |                             | 08/16/2017, |
| 3 | Unique Paths     |                             | 08/16/2017, |
| 4 | Unique Paths II  |                             | 08/16/2017, |
| 5 | Climbing Stairs  |                             | 08/30/2017, |
| 6 | Jump Game        |                             | 08/16/2017, |
| 7 | Jump Game II     | Maximum Subarray, Frog Jump | 08/30/2017, |

### Notes

1. 从递归到动归 - Triangle
    1. DFS - Traverse
       1. 复杂度`O(2^n)` n is the height (每次有两个可选路径)
    2. DFS - Divide & Conquer
        1. 复杂度`O(2^n)` 每条路劲没有任何条件让它提前返回
        2. 通过画图来观察如何降低复杂度（有重复计算）
    3. Divide & Conquer + Memorization
        1. 记忆化搜索，装了眼睛，知道停哪
    4. Bottom-up 动归的四个步骤
        1. 状态的定义
        2. 初始化，终点先有值
        3. 循环递推（不是递归）求解
        4. 求结果: 起点
    5. Top-down
        1. 两种没有太多的优略之分，根据个人习惯灵活运用
2. 什么样的题适合使用动态规划？
    1. 满足三个条件之一则极有可能使用动归求解
        1. Maximum/Minimum
        2. Yes/No
        3. Count(\*) (i.e. 总共有多少个）
3. 不能用动归
    1. 要求出所有“具体”的方案而非方案“个数”
       http://www.lintcode.com/problem/palindromepartitioning/
    2. 输入数据是一个“集合”而不是“序列”
       http://www.lintcode.com/problem/longest-consecutivesequence/
4. 动态规划4点要素
    1. 状态 State
        - 灵感，创造力，存储小规模问题的结果
    2. 方程 Function
        - 状态之间的联系，怎么通过小的状态，来算大的状态
    3. 初始化 Intialization
        - 最极限的小状态是什么, 起点
    4. 答案 Answer
        - 最大的那个状态是什么，终点
5. 面试最常见的四种类型
    1. Matrix DP (15%)
        1. state: f\[x\]\[y\] 表示我从起点走到坐标x,y……
        2. function: 研究走到x,y这个点之前的一步
        3. intialize: 起点
        4. answer: 终点
    2. Sequence DP (40%)
        1. state: f\[i\]表示“前i”个位置/数字/字母,(以第i个为)...
        2. function: f\[i\] = f\[j\] … j 是i之前的一个位置
        3. intialize: f\[0\]..
        4. answer: f\[n-1\]..
    3. Two Sequences DP (40%)
    4. Others (5%)
6. 面试中常用动态规划
    1. 矩阵动态规划（坐标相关）
    2. 单序列动态规划（上）

## Lecture 5 动态规划 Dynamic Programming II

| # |           Problem           | Comment |     Date    |
|:-:|:----------------------------|:--------|:-----------:|
| 1 | Palindrome Partitioning II  |         | 08/16/2017, |
| 2 | Word Break                  |         | 08/16/2017, |
| 3 | Longest Common Subsequence* |         | 08/16/2017, |
| 4 | Longest Common Substring*   |         | 08/16/2017, |
| 5 | Edit Distance               |         | 08/16/2017, |
| 6 | Distinct Subsequences       |         | 08/16/2017, |
| 7 | Interleaving String         |         | 08/16/2017, |

## Lecture 6 链表 Linked List

|  # |                       Problem                      | Comment |     Date    |
|:--:|:---------------------------------------------------|:--------|:-----------:|
| 1  | Remove Duplicates from Sorted List II              |         | 08/30/2017, |
| 2  | Reverse Linked List II                             |         | 08/30/2017, |
| 3  | Merge Two Sorted Lists                             |         | 08/30/2017, |
| 4  | Partition List                                     |         | 08/30/2017, |
| 5  | Sort List                                          |         | 08/30/2017, |
| 6  | Reorder List                                       |         | 08/30/2017, |
| 7  | Merge k Sorted Lists                               |         | 08/30/2017, |
| 8  | Copy List with Random Pointer                      |         | 08/30/2017, |
| 9  | Linked List Cycle                                  |         | 08/30/2017, |
| 10 | Convert Sorted List to Balanced Binary Search Tree |         | 08/30/2017, |
| 11 | Convert Binary Tree to Doubly Linked List          |         | 08/30/2017, |
| 12 | Reverse List Nodes in k-Groups                     |         | 08/30/2017, |

### Notes

1. Basic skills
    1. Insert a Node in Sorted List
    2. Remove a Node from Linked List
    3. Reverse a Linked List
    4. Merge Two Linked Lists
    5. Find the Middle of a Linked List
2. Sorting
    1. quick sort, 原地排序，不稳定。相对顺序会改变，所以不稳定。
    2. 稳定排序，
    3. 睡眠排序
3. Fast Slow Pointers
    1. Find the Middle of Linked List
    2. Remove Nth Node From End of List
    3. Linked List Cycle I, II

## Lecture 7 数组与数 Array & Numbers

|  # |                   Problem                  |                Comment               |     Date    |
|:--:|:-------------------------------------------|:-------------------------------------|:-----------:|
| 1  | Recover Rotated Sorted Array*              |                                      | 08/30/2017, |
| 2  | Rotate String*                             |                                      | 08/30/2017, |
| 3  | Reverse Words in a String                  |                                      | 08/30/2017, |
| 4  | Merge Sorted Array                         |                                      | 08/30/2017, |
| 5  | Merge Sorted Array II*                     |                                      | 08/30/2017, |
| 6  | Median of Two Sorted Arrays                |                                      | 08/30/2017, |
| 7  | Maximum Subarray                           |                                      | 08/30/2017, |
| 8  | Maximum Subarray II*                       | Prefix sum, left, right              | 08/30/2017, |
| 9  | Maximum Subarray III*                      | DP, from O(n 2k)O(n2k) to O(nk)O(nk) | 08/30/2017, |
| 10 | Minimum Subarray*                          |                                      | 08/30/2017, |
| 11 | Subarray Sum*                              | Subarray Sum Equals K                | 08/30/2017, |
| 12 | Best Time to Buy and Sell Stock            |                                      | 08/30/2017, |
| 13 | Best Time to Buy and Sell Stock II         |                                      | 08/30/2017, |
| 14 | Best Time to Buy and Sell Stock III        |                                      | 08/30/2017, |
| 15 | Two Sum                                    |                                      | 08/30/2017, |
| 16 | 3Sum                                       |                                      | 08/30/2017, |
| 17 | 4Sum                                       |                                      | 08/30/2017, |
| 18 | K Sum*                                     |                                      | 08/30/2017, |
| 19 | Partition Array                            |                                      | 08/30/2017, |
| 20 | Sort Colors                                |                                      | 08/30/2017, |
| 21 | Sort Letters by Case                       |                                      | 08/30/2017, |
| 22 | Interleaving Positive and Negative Numbers |                                      | 08/30/2017, |

### Notes

1. 三步翻转法
    1. Recover Rotated Sorted Array
2. Median of Two Sorted Arrays (**Important)**
    1. find median ==&gt; find kth largest
    2. take O(1) to reduce the problem to find the k/2 th element.
3. Maximum Subarray == Best Time to Buy and Sell Stock (see the video
    at 53:00)
    1. **前缀和**

## Lecture 8 数据结构 Data Structure

|                #                | Comment |     Date    |
|:--------------------------------|:--------|:-----------:|
|               题目              |   备注  |     日期    |
| Min-Stack                       |         |             |
| Implement a queue by two stacks |         |             |
| Largest Rectangle in histogram  | 递增栈  |             |
| Max Tree                        | 递减栈  |             |
| Rehashing                       |         |             |
| LRU Cache                       |         |             |
| Longest Consecutive Sequence    |         |             |
| Subarray Sum                    |         |             |
| Copy List with Random Pointer   |         |             |
| Anagrams                        |         |             |
| Median Number                   |         |             |
| Heapify                         |         |             |
| Merge K sorted List             |         |             |
| Word Search II                  |         | 08/30/2017, |

### Notes

1. Running time complexity for stack operation or other data structure
    1. talk average instead of worst case.
    2. think in the larger
2. For the largest Rectangle in histogram problem, considering the concept of 递增栈
3. 强化班内容：递增栈其他应用，表达式值，表达式树
4. Largest Rectangle in Histogram:
   思路模板：“这种题目的相似性是找数组里面一个元素左边或者右边第一个比他小或者比他大的元素"
5. 并查集，两个指针，hashheap，动态规划优化题目
6. Hash implementation:
    ```c
    int hashfunc(string key){
        int sum = 0;
        for (int i = 0; i < key.length(); i++) {
            sum = sum * 33 + (int)(key.charAt(i));
            sum = sum % HASH_TABLE_SIZE;
        }
        return sum
    }
    ```
7. Collision
    1. open hashing
    2. closed hashing

## Lecture 9 图论与搜索 Graph & Search

|              题目             | 备注 | 日期 |
|:------------------------------|:-----|:----:|
| Clone graph                   |      |      |
| Copy List with Random Pointer |      |      |
| Find the connected            |      |      |

### Notes

1. time for prepare interview
    1. easy: 5 min
    2. medium: 15 min
    3. hard: 30-40 min
2. 最好不用DFS， node数量多的情况下，recursion spaec消耗太大，
3. Topological sorting
    1. inDegree
    2. outDegree
4. 何适用BFS，
    1. 从一个点出发，找连通块
    2. level by level, 寻找两node最短距离
5. subset
    1. 隐式图搜索 VS 显式图搜索
6. 何时用DFS，
    1. 找出所有的结果而不是找出总结果数。
7. Running time permutation: O(n! \* n)
8. Running time subset: O(2^n \* n)
9. 搜索类型：
    1. 显式图搜索
        1. Matrix
        2. Graph
    2. 隐式图搜索
        1. permutation
        2. subset
        3. combination
            1. 对分割类，可以把问题看成分割线的combination problem.
