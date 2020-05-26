# Nine Chapter Dynamic Programming

* [Nine Chapter Dynamic Programming Course](https://wiki.rui-han.com/index.php/Private:Courses/Languages_and_Algorithms/Nine_Chapter_Dynamic_Programming_Course)

## Lecture 1 Introduction to Dynamic Programming

| Problem | Category |
|-------------------------------|--------------------|
| [Coin Change](#coin-change)   | |
| [Unique Paths](#unique-paths) | coordinate |
| [Jump Game](#jump-game)       | |

1. 动态规划题目特点:
    1. 计数
        * 有多少种方式走到右下角
        * 有多少种方法选出k个数使得和是Sum
    2. 求最大最小值
        * 从左上角走到右下角路径的最大数字和
        * 最长上升子序列长度
    3. 求存在性
        * 取石子游戏，先手是否必胜
        * 能不能选出k个数使得和是Sum
2. 状态是动态规划定海神针，确定状态需要两个基本意识：
    * 最后一步
    * 子问题

!!! Note "Four Ingredients for DP"
    1. What's the state?
        - start with the last step for the optimal solution
        - decompose into subproblems
    2. Write the state transition?
        - find the transition from subproblem relations
    3. Initial value and boundary conditions
        - need to think careful in this step
    4. How can you compute the states?
        - iteration directio
        - forward computing

### Coin Change

* Imagine the last coin you can use and the minimum solution is found can be
  represented as `f[amount]`. It can be solved by solving the smaller problem
  first. we have `f[amount] = min(f[amount], f[amount - last\_coin] + 1)`.
* The problem is we don't know which coin will be selected for the last one to
  reach the solution, so we have to iterate throught the coins to check every
  one of them. We expecting to see two `for` loops in our code.
* DP 4 ingredient:
    1. size of the dp array `f[amount + 1]`
    2. initial state: `f[0] = 0`, amount 0 can use 0 coin.
    3. subproblem: `f[amount] = min(f[amount], f[amount - last\_coin] + 1)`.
    4. results: `f[amount]`

```C++ tab=
class Solution {
public:
    int coinChange(vector<int>& coins , int amount) {
        int n = coins.size();
        int f[amount + 1];
        f[0] = 0;

        /* calculate the f[1], f[2], ... f[amount] */
        for (int i = 1; i <= amount; i++) {
            f[i] = INT_MAX;  /* small trick, set to invalid first */
            for (int j = 0; j < n; j++) { /* update states */
                /* f[i] can select coins[j] && f[i - coins[j]] is possible && coins[j] is last coin */
                if (i >= coins[j] && f[i - coins[j]] != INT_MAX && f[i - coins[j]] + 1 < f[i]) {
                    f[i] = f[i - coins[j]] + 1;
                }
            }
        }

        return f[amount] == INT_MAX ? -1 : f[amount];
    }
};
```

```C++ tab="Alternative Solution"
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        int n = coins.size();
        int f[amount+1];
        // f[i] represent the minimum counts to make up i amount
        // use INT_MAX to represent impossible case.
        f[0] = 0;
        for (int i = 1; i <= amount; i++) {
            f[i] = INT_MAX;
            for (int j = 0; j < n; j++) {
                if (i >= coins[j] && f[i - coins[j]] != INT_MAX) {
                    f[i] = min(f[i], f[i - coins[j]] + 1);
                }
            }
        }

        return f[amount] == INT_MAX ? -1 : f[amount];
    }
};
```

### Unique Paths

* Solving smaller problem first than by accumulating the results from the smaller
  problems, we can solve the the overall problem.
* Use a 2-d array to record the result the smaller problem, we know for the
  position `f[i][j] = f[i - 1][j] + f[i][j - 1]`, which which mean the summation
  of number of path from above and from left.
* The initial state is the first row and the first column are all equal to `1`.  

```C++ tab=
class Solution {
public:
    /**
     * @param n, m: positive integer (1 <= n ,m <= 100)
     * @return an integer
     */
    int uniquePaths(int m, int n) {
        int f[m][n] = {0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 || j == 0) {
                    f[i][j] = 1;
                }

                if (i > 0 && j > 0) {
                    f[i][j] = f[i - 1][j] + f[i][j - 1];
                }
            }
        }

        return f[m - 1][n - 1];

    }
};
```

### Jump Game

* Notice the problem statement "Each element in the array represents your maximum jump length at that position."

Solution 1 DP

* The problem have characteristics of the dynamic problem, that is it can be
  decomposed into smaller problem, the large problem can be solved using the
  result from solving smaller problems.
* We use a dp array `f[i]` to store whether it can jump from previous position to
  position `i`. if `nums[i] + i >= j`, it can also jump to position `j`.
* Looks like we are going to have a nested loop. the outer loop iterate to check
  whether can jump to each step `j`. inner loop check each step before `j`, namely
  the smaller size problem.

```C++ tab=
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return true;
        int f[n];
        f[0] = true; /* initialize */

        for (int j = 1; j < n; j++) {
            f[j] = false;
            for (int i = 0; i < j; i++) {
                if (f[i] && nums[i] + i >= j) {
                    f[j] = true;
                    break;
                }
            }
        }

        return f[n - 1];
    }
};
```

Solution 2 Greedy

* Use a variable `cur_max` to maintain the possible maximum jump position, if the
  current position is less than the maximum possible jump, return flase.

```C++ tab=
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return true;

        int cur_max = nums[0]; /* maximum distance can reach from ith */
        /* L.I.: current step (ith step) must <= cur_max jump */
        for (int i = 0; i < n; i++) {
            if (i > cur_max) /* must goes first */
                return false;

            if (i + nums[i] > cur_max)
                cur_max = i + nums[i];
        }

        return true;
    }
};
```

## Lecture 2 Dynamic Programming on Coordinates

| Problem | Category |
|-------------------------------|--------------------|
| [Unique Paths II](#unique-paths-ii) | 坐标型 |
| [Paint House](#paint-house) | 序列型 ＋ 状态 |
| [Decode Ways](#decode-ways) | 划分型 |
| [Longest Increasing Continuous Subsequence](#longest-increasing-continuous-subsequence) | 序列型 / 划分型 |
| [Minimum Path Sum](#minimum-path-sum) | 坐标型 |
| [Bomb Enemy](#bomb-enemy) | 坐标型 |
| [Counting Bits](#counting-bits) | 坐标型 |

### 坐标型动态规划

* 最简单的动态规划类型
* 给定一个序列或网格
* 需要找到序列中某个/些子序列或网格中的某条路径
    1. 某种性质最大/最小
    1. 计数
    1. 存在性
* 动态规划方程`f[i]`中的下标`i`表示以`a[i]`为结尾的满足条件的子序列的性质. `f[i][j]` 中的
  下标 `i`, `j` 表示以格子`(i, j)`为结尾的满足条件的路径的性质
    1. 最大值/最小值
    2. 个数
    3. 是否存在
* 坐标型动态规划的初始条件`f[0]`就是指以`a[0]`为结尾的子序列的性质

### Unique Paths II

```C++ tab="C++ Naive DP"
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        int i = 0;
        int j = 0;
        int flag = 0;
        vector<vector<int>> f(m, vector<int>(n));
        if (m == 0 || n == 0) {
            return 0;
        }

        if (obstacleGrid[0][0] == 1) {
            return 0;
        }

        f[0][0] = 1;
        for (i = 1; i < m; i++) {
            if (obstacleGrid[i][0] == 1) {
                f[i][0] = 0;
            } else {
                f[i][0] = f[i-1][0];
            }
        }
        for (j = 1; j < n; j++) {
            if (obstacleGrid[0][j] == 1) {
                f[0][j] = 0;
            } else {
                f[0][j] = f[0][j-1];
            }
        }

        for (i = 1; i < m; i++) {
            for (j = 1; j < n; j++) {
                if (obstacleGrid[i][j] == 1) {
                    f[i][j] = 0;
                } else {
                    f[i][j] = f[i][j-1] + f[i-1][j];
                }
            }
        }

        return f[m-1][n-1];
    }
};
```

```C++ tab="C++ Naive DP Refactored"
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = m > 0 ? obstacleGrid[0].size() : 0;
        if (m == 0 && n == 0) {
            return 0;
        }

        vector<vector<int>> f(m, vector<int>(n, 0));

        f[0][0] = 1;

        if (obstacleGrid[0][0] == 1) {
            return 0;
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (obstacleGrid[i][j] == 1) {
                    f[i][j] = 0;
                } else {
                    // whenever available
                    if (i > 0) {
                        f[i][j] += f[i - 1][j];
                    }

                    if (j > 0) {
                        f[i][j] += f[i][j - 1];
                    }
                }
            }
        }

        return f[m - 1][n - 1];
    }
};
```

```Java tab="Java O(n) space"
// Optimized using a rolling array or single row dp array instead of m x n.
class Solution {
    public int uniquePathsWithObstacles(int[][] a) {
        int m = a.length;
        int n = a[0].length;

        int dp[] = new int[n];
        dp[0] = 1;

        //T[i][j] = T[i-1][j] + T[i][j-1]
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(a[i][j] == 1){
                    dp[j] = 0;
                }else if(j > 0){
                    dp[j] += dp[j-1];
                }
            }
        }

        return dp[n-1];
    }
}
```

### Paint House

* DP last step: considering the optimal solution. The last paint must be one of
  the three colors and the paint cost is minimum.
* State: we can try to name `f[i]` the minimum cost of painting the first `i`
  houses, `i = 0, 1, ... i-1`. However, we cannot know what color could be used for the last house.
* Change the state to: `f[i][k]` is the minium cost of painting the first `i`
  houses and the `i-1`th element is painted as color `k`. This way, we can choose
  the last paint based on this piece of information.
* The state transition fomular: $f[i][k] = f[i - 1][\ell]_{\ell != k} + costs[i - 1][k]$
* The result is minimum of the: $f[i][0],\ f[i][1],\ f[i][2]$

```C++
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int m = costs.size();
        int f[m+1][3];
        /* state: f[i][0] paint i house and the last (i - 1) house is red *
        /* initial value, paint 0 house cost 0 */
        for (int j = 0; j < 3; j++)
            f[0][j] = 0;

        for (int i = 1; i <= m; i++) {
            for (int j = 0; j < 3; j++) { // current color
                f[i][j] = INT_MAX;
                for (int k = 0; k < 3; k++) { // painted color
                    /* cannot paint same color for neighbor house */
                    if (j == k)
                        continue;

                    // ith (index with i - 1) house is painted with color j
                    if (f[i][j] > f[i - 1][k] + costs[i - 1][j])
                        f[i][j] = f[i - 1][k] + costs[i - 1][j];

                    //f[i][j] = min(f[i - 1][k] + costs[i - 1][j], f[i][j]);
                }
            }
        }

        return min(min(f[m][0], f[m][1]), f[m][2]);
    }
};
```

### Decode Ways

4 Ingredients

1. Last step:

    ```text
    A_0, A_1, A_2, ..., A_n-3, [A_n-2, A_n-1]
    A_0, A_1, A_2, ..., A_n-3, A_n-2, [A_n-1]
    ```

2. State: `f[n]`: decode ways of first `n` letters.
3. Smaller problem: `f[n] = f[n - 1] + f[n - 2] | A_n-2,A_n-1` decodable.
4. Init: `f[0] = 1`

!!! Note
    Again the idea in this solution is to update the `f[i][j]` on demand, a
    technique that broadly used in 2-d coordinate based DP problems such as
    [Bomb Enemy](#bomb-enemy), [Unique Paths II](#unique-paths-ii), and
    [Minimum Path Sum](#minimum-path-sum).

```C++ tab="C++ DP"
class Solution {
public:
    int numDecodings(string s) {
        int n = s.length();
        int f[n + 1] = {0};
        if (n == 0)
            return 0;

        f[0] = 1;
        for (int i = 1; i <= n; i++) {
            int t = s[i - 1] - '0';
            if (t > 0 && t <= 9) {
                f[i] += f[i - 1];
            }

            if (i > 1){
                int q = (s[i - 2] - '0') * 10 + t;
                if (q >= 10 && q <= 26) {
                    f[i] += f[i - 2];
                }
            }
        }

        return f[n];
    }
};
```

```C++ tab="C++ DP O(1)"
class Solution {
public:
    int numDecodings(string s) {
        if (!s.size() || s.front() == '0') return 0;
        // r2: decode ways of s[0, i-2] , r1: decode ways of s[0, i-1]
        int r1 = 1, r2 = 1;

        // think it as a coordinate bases, not sequence based dp
        for (int i = 1; i < s.size(); i++) {
            // last char in s[0, i] is 0, cannot decode
            if (s[i] == '0') r1 = 0;

            // two-digit letter, add r2 to r1, r2 get the previous r1
            if (s[i - 1] == '1' || s[i - 1] == '2' && s[i] <= '6') {
                r1 = r2 + r1;
                r2 = r1 - r2;
            } else { // one-digit letter, r2 get the previous r1
                r2 = r1;
            }
        }

        return r1;
    }
};
```

### Longest Increasing Continuous Subsequence

4 ingredients:

1. Last step, last element `a[n-1]` could be in the result or not in the result.
2. subproblem, suppose we have the LICS of the first `n - 1` elements. represented as `f[n-1]`.
3. base case and boundary condition, when no char in the string: `f[0] = 1`. empty string have LICS length of 1.
4. order of calculation, calculate small index first.

!!! Note "Not a leetcode"
    The problem is not a leetcode problem, the original problem ask for sequence
    that could be increase or decrease.

!!! Warning "Using index in DP problems"
    Avoid using both index `i - 1` and `i + 1` in a loop invariance, otherwise
    you'll have problem in keeping the loop invariance. Compare the followings.

    ```C++ tab="Incorrect"
    for (int i = n - 1; i >= 0; i--) {
        f[i] = 1;
        if (i < n - 1 && i > 0 && A[i - 1] > A[i]) {
            f[i] = f[i + 1] + 1;
        }
        res2 = max(res2, f[i]);
    }
    ```

    ```C++ tab="Correct"
    for (int i = n - 1; i >= 0; i--) {
        f[i] = 1;
        if (i < n - 1 && A[i] > A[i + 1]) {
            f[i] = f[i + 1] + 1;
        }
        res2 = max(res2, f[i]);
    }
    ```

    ```C++ tab="Correct"
    for (int i = 0; i < n; i++) {
        f[i] = 1;
        if (i > 0 && A[i - 1] > A[i]) {
            f[i] = f[i - 1] + 1;
        }
        res2 = max(res2, f[i]);
    }
    ``` 

```C++ tab="C++ DP solution"
class Solution {
public:
    int longestIncreasingContinuousSubsequence(vector<int>& A) {
        // Write your code here
        int n = A.size();
        int f[n];
        int res1 = 0;
        int res2 = 0;

        for (int i = 0; i < n; i++) {
            f[i] = 1;
            if (i > 0 && A[i - 1] < A[i]) {
                f[i] = f[i - 1] + 1;
            }
            res1 = max(res1, f[i]);
        }

        for (int i = n - 1; i >= 0; i--) {
            f[i] = 1;
            if (i < n - 1 && A[i] > A[i + 1]) {
                f[i] = f[i + 1] + 1;
            }
            res2 = max(res2, f[i]);
        }

        return max(res1, res2);
    }
};
```

Here we don't have to explicitly wirte `f[i] = max(1, f[i - 1] + 1)`, since we
have the condition `A[i - 1] < A[i]`, we know the `A[i]` will be added to the
`f[i]`, hence simply update `f[i] = f[i - 1] + 1` directly.

### Minimum Path Sum

* This is coordinate based DP. We need to have `f[i][j]` to keep the minimum path sum to the `grid[i][j]`.
* Calculate `f[i][j]` from top to down and from left to right for each row. Space can be optimized.
* Notice we can do the init and first row and first column in nested loops, do not need use multiple for loops.

```C++ tab="C++ DP"
class Solution {
public:
    int minPathSum(vector<vector<int> > &grid) {
        int m = grid.size();
        int n = grid[0].size();
        if (m == 0 || n == 0)
            return 0;

        int f[m][n];

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 && j == 0) {
                    f[i][j] = grid[i][j];
                }

                if (i == 0 && j > 0) {
                    f[i][j] = f[i][j - 1] + grid[i][j];
                }

                if (j == 0 && i > 0) {
                    f[i][j] = f[i - 1][j] + grid[i][j];
                }

                if (i > 0 && j > 0) {
                    f[i][j] = min(f[i - 1][j], f[i][j - 1]) + grid[i][j];
                }
            }
        }

        return f[m - 1][n - 1];
    }
};
```

```C++ tab="C++ DP Space O(n)"
class Solution {
public:
    int minPathSum(vector<vector<int> > &grid) {
        int m = grid.size();
        int n = grid[0].size();
        if (m == 0 || n == 0)
            return 0;

        int f[2][n]; /* only two rows of status */
        int prev = 1;
        int curr = 1;
        for (int i = 0; i < m; i++) {
            // rolling the f array when move to a new row
            prev = curr;
            curr = 1 - curr;  
            for (int j = 0; j < n; j++) {
                f[curr][j] = grid[i][j];

                if (i == 0 && j > 0) {
                    f[i][j] += f[i][j - 1];
                }

                if (j == 0 && i > 0) {
                    f[curr][j] += f[prev][j];
                }

                if (i > 0 && j > 0) {
                    f[curr][j] += min(f[prev][j], f[curr][j - 1]);
                }

            }
        }

        return f[curr][n - 1];
    }
};
```

!!! Note
    1. 在处理二维坐标型DP问题的时候，对于第一行或者第一列的计算我们可以把它放到loop中。但是要加一个条件。
    这个条件就是 `if (i > 0)`, 这个条件保证了在更新数组值的时候下标不会越界。并且很好的诠释了我们的动机（就是对于第一行第一列仅仅是初始化)。
    2. 对于1中提到的trick，另外一个好处就是对于第一行或者第一列条件判断的时候，这里还有一个技巧就是先把自身的‘性质’或者‘值’加上，
    如果有上一行我们再去加上一行的‘性质’或‘值’。这里需要逆向思维.
    3. 在多种情况需要分开讨论的时候我们可以先‘一票否决’最明显情况，然后讨论剩下的情况。

### Bomb Enemy

* Breakdown the problem into smaller (simpler) problems.
* Take the special steps (i.e. different properties of the cell) as normal ones
  in the loop, deal with the special step when doing the calculation.

```C++
class Solution {
public:
    int maxKilledEnemies(vector<vector<char>>& grid) {
        int m = grid.size();
        if (m == 0) return 0;

        int n = grid[0].size();
        if (n == 0) return 0;

        int f[m][n];
        int res[m][n];
        int ret = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                res[i][j] = 0;
            }
        }

        /* up */
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                f[i][j] = 0;
                if (grid[i][j] == 'W') {
                    f[i][j] = 0;
                } else {
                    if (grid[i][j] == 'E') {
                        f[i][j] = 1;
                    }

                    if (i > 0) {
                        f[i][j] += f[i - 1][j];
                    }
                }
                res[i][j] += f[i][j];
            }
        }

        /* down */
        for (int i = m - 1; i >= 0; i--) {
            for (int j = 0; j < n; j++) {
                f[i][j] = 0;
                if (grid[i][j] == 'W') {
                    f[i][j] = 0;
                } else {
                    if (grid[i][j] == 'E') {
                        f[i][j] = 1;
                    }

                    if (i < m - 1) {
                        f[i][j] += f[i + 1][j];
                    }
                }
                res[i][j] += f[i][j];
            }
        }

        /* left */
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                f[i][j] = 0;
                if (grid[i][j] == 'W') {
                    f[i][j] = 0;
                } else {
                    if (grid[i][j] == 'E') {
                        f[i][j] = 1;
                    }

                    if (j > 0) {
                        f[i][j] += f[i][j - 1];
                    }
                }
                res[i][j] += f[i][j];
            }
        }

        /* right */
        for (int i = 0; i < m; i++) {
            for (int j = n - 1; j >= 0; j--) {
                f[i][j] = 0;
                if (grid[i][j] == 'W') {
                    f[i][j] = 0;
                } else {
                    if (grid[i][j] == 'E') {
                        f[i][j] = 1;
                    }

                    if (j < n - 1) {
                        f[i][j] += f[i][j + 1];
                    }
                }
                res[i][j] += f[i][j];
            }
        }

        /* calculate resutls */
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '0')
                    if (res[i][j] > ret)
                        ret = res[i][j];
            }
        }

        return ret;
    }
};
```

### Counting Bits

* Notice you can use the trick that `i >> 1 == i / 2` to construct the subproblem

| i  | count 1 bits |
|----|--------------|
| 1  | 1 |
| 2  | 1 |
| 3  | 2 |
| 6  | 2 |
| 12 | 2 |
| 25 | 3 |
| 50 | 3 |

```C++
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> f(num + 1, 0);
        if (num == 0) {
            return f;
        }

        for (int i = 1; i <= num; i++) {
            f[i] = f[i >> 1] + (i % 2);
        }

        return f;
    }
};
```

## Lecture 3 Dynamic Programming on Sequences

| Problem | Category |
|-------------------------------|--------------------|
| [Paint House II](#paint-house-ii) | 序列型＋状态 |
| [House Robber](#house-robber) | 序列型＋状态 |
| [House Robber II](#house-robber-ii) | 序列型＋状态 |
| [Best Time to Buy and Sell Stock](#best-time-to-buy-and-sell-stock) | |
| [Best Time to Buy and Sell Stock II](#best-time-to-buy-and-sell-stock-ii) | |
| [Best Time to Buy and Sell Stock III](#best-time-to-buy-and-sell-stock-iii) | 序列型＋状态 |
| [Best Time to Buy and Sell Stock IV](#best-time-to-buy-and-sell-stock-iv) | 序列型＋状态 |
| [Longest Increasing Subsequence](#longest-increasing-subsequence) | 序列型 |
| [Russian Doll Envelopes](#russian-doll-envelopes) | |

### 序列型动态规划

给定一个序列

1. 动态规划方程`f[i]`中的下标`i`表示前`i`个元素`a[0], a[1], ..., a[i-1]`的某种性质
    * 坐标型的`f[i]`表示以`a[i]`为结尾的某种性质
2. 初始化中，`f[0]`表示空序列的性质
    * 坐标型动态规划的初始条件`f[0]`就是指以`a[0]`为结尾的子序列的性质

### Paint House II

* the solution could be the same as the first verison, but it is $O(n\cdot k^2)$. how to make it $O(n\cdot k)$?
* By analyzing the state transition equation, we observed that we want to find a
  minimum value of a set of numbers except one for each house. Put it in english,
  the min cost to paint `i - 1`th house with color `k`, we want to have the min
  cost of painting all previous `i - 1` houses and the `i-1`th house cannot be
  painted as color `k`.

    ```C++
    f[i][1] = min{f[i-1][2] + cost[i-1][1], f[i-1][3] + cost[i-1][1], ..., f[i-1][K] + cost[i-1][1]}
    f[i][2] = min{f[i-1][1] + cost[i-1][2], f[i-1][3] + cost[i-1][2], ..., f[i-1][K] + cost[i-1][2]}
    ...
    f[i][K] = min{f[i-1][1] + cost[i-1][K], f[i-1][2] + cost[i-1][K], ..., f[i-1][K-1] + cost[i-1][K]}
    ```

* We could optimize the solution upon this. Basically, we can maintain the first
  two minimum value of the set `f[i-1][1], f[i-1][2], f[i-1][3], ..., f[i-1][K]`,
  `min1` and `min2` and their index `j1` and `j2`.
    1. There are two cases, the first case is the house `i-1` is painted with the
       same color correspoinding to the minimum value. in this case, we cannot chose
       to paint it with the color corresponding to the minimum cost, we can update the state using the second minimum.
    2. The second case is that we paint the i-1th house with color other than the
       color corresponding to the minimum cost to pain, we can update using the minimum value.

```C++ tab="C++ O(nk)"
class Solution {
public:
    /**
     * @param costs n x k cost matrix
     * @return an integer, the minimum cost to paint all houses
     */
    int minCostII(vector<vector<int>>& costs) {
        int m = costs.size();
        if (m == 0) return 0;
        int k = costs[0].size();
        if (k == 0) return 0;

        int f[m + 1][k];

        int min1;
        int min2;
        int j1 = 0, j2 = 0;

        /* init, 0 house cost nothing. */
        for (int j = 0; j < k; j++)
            f[0][j] = 0;

        for (int i = 1; i <= m; i++) {
            min1 = min2 = INT_MAX; 
            /* from all the colors, find the min1 and min2 */
            for (int j = 0; j < k; j++) {
                /* get the min1 and min2 first */
                if (f[i - 1][j] < min1) {
                    min2 = min1;
                    j2 = j1;
                    min1 = f[i - 1][j];
                    j1 = j;
                } else if (f[i - 1][j] < min2) {
                    min2 = f[i - 1][j];
                    j2 = j;
                }
            }

            /* update the states based on the min1 and min2 */
            for (int j = 0; j < k; j++) {
                if (j != j1) {
                    f[i][j] = f[i - 1][j1] + costs[i - 1][j];
                } else {
                    f[i][j] = f[i - 1][j2] + costs[i - 1][j];
                }
            }
        }

        int res = INT_MAX;
        for (int j = 0; j < k; j++) {
            if (f[m][j] < res)
                res = f[m][j];
        }

        return res;
    }
};
```

```C++ tab="C++ O(nk^2)"
class Solution {
public:
    int minCostII(vector<vector<int>>& costs) {
        int n = costs.size();
        int k = n > 0 ? costs[0].size() : 0;
        if (n == 0 && k == 0)
            return 0;

        if (n == 1 && k == 1)
            return costs[0][0];

        vector<vector<int>> f(n + 1, vector<int>(k, 0));

        for (int i = 0; i < k; i++) {
            f[0][i] = 0; // 0 cost for 0 paint
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < k; j++) {
                f[i][j] = INT_MAX;
                for (int c = 0; c < k; c++) {
                    if (j == c) {
                        continue;
                    }

                    if (f[i][j] > f[i - 1][c] + costs[i - 1][j]) {
                        f[i][j] = f[i - 1][c] + costs[i - 1][j];
                    }
                }
            }
        }

        int res = INT_MAX;
        for (int i = 0; i < k; i++) {
            if (f[n][i] < res)
                res = f[n][i];
        }

        return res;
    }
};
```

### House Robber

* Start with the last step. The last house could be either robbed or not.
    1. If the last house `a[i-1]` is robbed, then we cannot rob `a[i-2]`. we have `f[i] = f[i-2] + a[i-1]`.
    2. If the last house `a[i-1]` is not robbed, then we can rob `a[i-2]`.
       Alternatively, we can skip it to rob `a[i-3]`. We have `f[i] = f[n-1]`,
       in which this `f[n-1]` could not let us know whether `a[i-2]` is robbed or not.
* We add the state of `a[i-1]` to the state transition equation. Thus we have:
    1. `f[i][0]` represent "the maximum money for robbing the first `i` houses and the last house __hasn't__ been robbed."
    2. `f[i][1]` represent "the maximum money for robbing the first `i` houses and the last house __has__ been robbed."
* state transition equations:

    ```text
    f[i][0] = max(f[i-1][0], f[i-1][1])
    f[i][1] = f[i-1][0] + a[i-1]
    ```

```C++ tab="C++"
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return 0;
        int f[n + 1][2] = {0};

        for (int i = 1; i <= n; i++) {
            f[i][0] = max(f[i - 1][0], f[i - 1][1]);
            f[i][1] = f[i - 1][0] + nums[i - 1];
        }

        return max(f[n][0], f[n][1]);
    }
};
```

```C++ tab="C++ optimized"
class Solution {
public:
    long long houseRobber(vector<int> A) {
        int n = A.size();
        if (n == 0) return 0;
        long long f[n+1];

        /* init */
        f[0] = 0;
        f[1] = A[0];

        for (int i = 2; i <= n; i++) {
            f[i] = max(f[i - 1], f[i - 2] + A[i - 1]);
        }

        return f[n];
    }
};
```

Solution 2

* `yes` - first `i` days, robber last day,
* `no` - first `i` days, not robber at last day.

```C++ tab=
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return 0;
        int yes = 0, no = 0;

        for (int i = 1; i <= n; i++) {
            int tmp = no;
            no = max(yes, no);
            yes = tmp + nums[i - 1];
        }

        return max(yes, no);
    }
};
```

### House Robber II

* Following [House Robber](#house-robber), when we try to analyze the last step,
  the house `i - 1` depends on the house `i - 2` and the house `0`. How could we handle this?
* We could enumerate two cases, and reduce the probelm to [House Rober](#house-robber)
    1. house `0` is robbed and house `i-1` is not.
    2. house `i - 1` is robbed and house `0` is not.

```C++
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return 0;

        if (n == 1) // edge case to deal with
            return nums[0];

        vector<int> nums1(nums.begin() + 1, nums.end());
        vector<int> nums2(nums.begin(), nums.end() - 1);

        return max(rob_helper(nums1), rob_helper(nums2));
    }

private:
    int rob_helper(vector<int>& A) {
        int n = A.size();
        if (n == 0)
            return 0;
        int f[n+1] = {0};

        f[0] = 0;
        f[1] = A[0];
        for (int i = 2; i <= n; i++) {
            f[i] = max(f[i - 1], f[i - 2] + A[i - 1]);
        }

        return f[n];
    }
};
```

### Best Time to Buy and Sell Stock

* Force youself to do it by only one pass. You'll find that you need two variables
  to record the minimum value currently found and the maximum profit currently found.

```C++
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        int n = prices.size();
        if (n == 0)
            return 0;

        int min_price = INT_MAX;
        int max_profit = 0;

        for (int i = 0; i < n; i++) {
            if (prices[i] < min_price) {
                min_price = prices[i];
            } else if (prices[i] - min_price > max_profit) {
                max_profit = prices[i] - min_price;
            }
        }

        return max_profit;
    }
};
```

### Best Time to Buy and Sell Stock II

* We can just buy and sell when ever the price is increased in a day.
* Once you can proof the day-by-day subproblem can form the solution. The solution becomes so easy.

```C++ tab="C++"
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        int res = 0;
        for (int i = 1; i < prices.size(); i++) {
            if (prices[i - 1] < prices[i])
                res += prices[i] - prices[i - 1];
        }

        return res;
    }
};
```

```C++ tab="C++ Alternative"
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        int res = 0;
        for (int i = 1; i < prices.size(); i++) {
            res += max(prices[i] - prices[i - 1], 0)
        }

        return res;
    }
};
```

### Best Time to Buy and Sell Stock III

* We can define 5 stages and write state transition equations based on it.

![buy-sell-stock-iii-sol](fig/buy-sell-stock-iii-sol.png)

```C++ tab=""
class Solution {
public:
    /**
     * @param prices: Given an integer array
     * @return: Maximum profit
     */
    int maxProfit(vector<int> &A) {
        int n = A.size();
        if (n == 0) {
            return 0;
        }

        int f[n+1][6];

        /* init */
        f[0][1] = 0;
        f[0][2] = f[0][3] = f[0][4] = f[0][5] = INT_MIN;

        for (int i = 1; i <= n; i++) {
            // stage 1, 3, 5:
            // f[i][j] = max{f[i - 1][j], f[i - 1][j - 1] + A[i - 1] - A[i - 2]}
            for (int j = 1; j <= 5; j += 2) {
                f[i][j] = f[i - 1][j];
                if (i > 1 && j > 1 && f[i - 1][j - 1] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j - 1] + A[i - 1] - A[i - 2]);
                }

            }

            // stage 2, 4:
            // f[i][j] = max{f[i - 1][j] + A[i - 1] - A[i - 2], f[i - 1][j - 1], f[i - 1][j - 2] + A[i - 1] - A[i - 2]}
            for (int j = 2; j < 5; j += 2) {
                f[i][j] = f[i - 1][j - 1];
                if (i > 1 && f[i - 1][j] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j] + A[i - 1] - A[i - 2]);
                }

                if (i > 1 && j > 2 && f[i - 1][j - 2] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j - 2] + A[i - 1] - A[i - 2]);
                }
            }
        }

        return max(f[n][1], max(f[n][3], f[n][5]));
    }
};
```

### Best Time to Buy and Sell Stock IV

* If `k` is larger then `n/2`, it is equivalent to the II.
* This is a generalized solution from [Best Time to Buy and Sell Stock III](#best-time-to-buy-and-sell-stock-iii).

```C++
class Solution {
public:
    int maxProfit(int K, vector<int> &A) {
        int n = A.size();
        if (n == 0) {
            return 0;
        }

        int f[n+1][2 * K + 1 + 1];

        /* special case need to take care of */
        if (K > (n / 2)) {
            int res = 0;
            for (int i = 0; i + 1 < n; i++) {
                if (A[i + 1] - A[i] > 0) {
                    res += A[i + 1] - A[i];
                }
            }
            return res;
        }

        /* init */
        f[0][1] = 0;
        for (int k = 2; k <= 2 * K + 1; k++) {
            f[0][k] = INT_MIN;
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= 2 * K + 1; j += 2) {
                f[i][j] = f[i - 1][j];
                if (i > 1 && j > 1 && f[i - 1][j - 1] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j - 1] + A[i - 1] - A[i - 2]);
                }

            }

            for (int j = 2; j < 2 * K + 1; j += 2) {
                f[i][j] = f[i - 1][j - 1];
                if (i > 1 && f[i - 1][j] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j] + A[i - 1] - A[i - 2]);
                }

                if (i > 1 && j > 2 && f[i - 1][j - 2] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j - 2] + A[i - 1] - A[i - 2]);
                }
            }
        }

        int res = INT_MIN;
        for (int j = 1; j <= 2 * K + 1; j += 2 ) {
            if (f[n][j] > res)
                res = f[n][j];
        }

        return res;
    }
};
```

### Longest Increasing Subsequence

* Comparing to the problem [Longest Increasing Continuous Subsequence](#longest-increasing-continuous-subsequence).
  If the subsequence is not continous. we have to enumerate each of the previous element befor `A[j]`.

Solution 1 DP O(n^2)

```C++ tab="DP"
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return 0;

        int res = 0;
        int f[n] = {0};

        for (int j = 0; j < n; j++) {
            /* case 1: a[j] is the subsequence */
            f[j] = 1;

            /* case 2: LIS from a[0],...a[i] plus a[j] */
            for (int i = 0; i < j; i++) {
                if (nums[i] < nums[j] && f[i] + 1 > f[j])
                    f[j] = f[i] + 1;
            }
            res = max(res, f[j]);
        }

        return res;
    }
};
```

Solution 2 DP with binary search O(nlogn)

* To reduce the complexity, we can try to find if there is any redundant work we
  have been done. or some how we could use some order information to avoid some of the calculation.
* Focus on the real meaning of longest Increasing Subsequence. In fact, you are
  looking for the smallest value before `A[i]` that leads to the longest Increasing Subsequence so far.
* use the state `f[i]` to record the LIS of the array `A[0], ... A[i -1]`. If we
  are at `f[j], j > i`, we are looking for the largest `f[i]` value that have the smallest `A[i]`.

```C++ tab="DP with binary search"
class Solution {
public:
    int longestIncreasingSubsequence(vector<int> nums) {
        int n = nums.size();
        if (n == 0)
            return 0;

        //int f[n];
        // b[i]: when f value is i, smallest a value (ending value)
        int b[n + 1];

        int top = 0;
        b[0] = INT_MIN;
        // O(n)
        for (int i = 0; i < n; i++) {
            // b[0] ~ b[top]
            // last value b[j] which is smaller than A[i]
            int start = 0, end = top;
            int mid;
            int j;

            // O(lgn)
            while (start <= end) {
                mid = start + (end - start) / 2;
                if (b[mid] < nums[i]) {
                    j = mid;
                    start = mid + 1;
                } else {
                    end = mid - 1;
                }
            }
            // b[i]: length is j (f value is j), smallest ending value.
            // A[i] is after it.
            // f[i] = j + 1
            // b[j + 1]: length is (j + 1), smallest ending value.
            // A[i]
            // B[j + 1] >= A[i]
            b[j + 1] = nums[i];
            if (j + 1 > top) {
                top = j + 1;
            }
        }
        // b[top] stores the smallest ending value for an LIS
        return top;
    }
};
```

Solution 3 DP with binary search refactored

* In observing the fact that we can use extra space to keep the "minimum elements
  see so far from nums that is the last element of LIS for the different length of such LISes".
* Different from the regular DP solution, our extra space b is storing element from
  nums, and the element stored in b are not necessary in order.
* The index i of elements in b related to the length of a LIS whose last element
  is `a[i]`. specifically, `i + 1 = length(LIS)`.

```C++ tab="DP with binary search refactored"
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> b;

        for (int i = 0; i < nums.size(); ++i) {
            int begin = 0, end = b.size();
            while (begin != end) {
                int mid = begin + (end - begin) / 2;
                if (b[mid] < nums[i]) {
                    begin = mid + 1;
                } else {
                    end = mid;
                }
            }

            if (begin == b.size()) // nums[i] greater than all element in b
                b.push_back(nums[i]);
            else // begin point to next element no less than the target nums[i].
                b[begin] = nums[i];
        }

        return b.size();
    }
};
```

Solution 4 C++ using lower_bound

* we can use the lower_bound to replace the binary search routine in the above solution.

```C++ tab="C++ using lower_bound"
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> b;
        for (auto a : nums) {
            auto it = lower_bound(b.begin(), b.end(), a);
            if (it == b.end()) b.push_back(a);
            else *it = a;
        }

        return b.size();
    }
};
```

### Russian Doll Envelopes

```C++

```

## Lecture 7

### 403. Frog Jump

* You could use naive solution to iterate all the possible cases and use
  memoization to speed up.
* In that case, you can also use binary search to located a stone in the array
  which help to improve the complexity.
* But the best solution is to use a map to keep all the possible steps for a stone.

```C++ tab="C++ map to set"
class Solution {
public:
    bool canCross(vector<int>& stones) {
        int n = stones.size();
        unordered_map<int, unordered_set<int>> hmap;

        for (int i = 0; i < stones.size(); i++) {
            hmap[stones[i]] = unordered_set<int>();
        }

        hmap[stones[0]].insert(0);

        for (int i = 0; i < stones.size(); i++) {
            for (int k: hmap[stones[i]]) {
                for (int s = k - 1; s <= k + 1; s++) {
                    if (s > 0 && hmap.find(stones[i] + s) != hmap.end()) {
                        hmap[stones[i] + s].insert(s);
                    }
                }
            }
        }

        return hmap[stones[n - 1]].size() > 0;
    }
};
```