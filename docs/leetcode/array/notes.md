# Array

## Category 1 Remove/Contains Duplidate

### Contains Duplicate

### Contains Duplicate II

### Contains Duplicate III

### Find the Duplicate Number

### Remove Duplicates from Sorted Array

### Remove Duplicates from Sorted Array II

### Remove Duplicates from Sorted List II

### Remove Duplicates from Sorted List

### Move Zeroes

## Category 2 Matrix problems

### Spiral Matrix

### Spiral Matrix II

### Search a 2D Matrix

### Search a 2D Matrix II

### Rotate Image

### [Range Sum Query 2D - Mutable](#range-sum-query-2d-mutable)

### [Range Sum Query 2D - Immutable](#range-sum-query-2d-immutable)

### Maximal Square

### Maximal Rectangle

## Category 3 Subarray problems

### Type of subarray problem

* Find a subarray that fulfills a certain property, i.e maximum size subarray, [Longest Substring with At Most Two Distinct Characters](#longest-substring-with-at-most-two-distinct-characters)
    1. Use map or two pointer to solve the problem.
* Split into subarrays that fulfill certain properties, i.e. sum greater than k.

!!! Note "Fixed length subarray indexing"
    To correctly index an array in solving subarray problems are critical. Here is some tips:
    1. To iterate through a subarray of certain size, alwasy using the __"one-of-the-end"__ pattern.
       Namely, the iteration index `i` point to the __"one-off-the-end"__ of the subarray.
       The subarray of size K before the index is started at index `i - k`.
    2. The above convention is especially useful in subarray problems given constrains,
       such as "the subarray size greater than `k`", "maximum sum of non-overlapping subarray", etc.
    3. Example problems
        - [Maximum Average Subarray II](#maximum-average-subarray-ii)
        - [Maximum Sum of Two Non-Overlapping Subarrays](#maximum-sum-of-two-non-overlapping-subarrays)

### Two types of prefix sum

There are two ways to calculate the prefix sum array. Take which ever conveniece
for your when solving a problem.

__Option 1:__ `sums.resize(n, 0);`

```text
nums: [1, 2, 3,  4,  5,  6,  7,  8,  9]
             i               j
sums: [1, 3, 6, 10, 15, 21, 28, 36, 45]
```

In this case, each element `sums[i]` in sums represent the cumulative sum for
indexes `[0, ..., i]`. In other words, `sum[i]` represent cumulative sum up to
element `i` inclusive. When you want to get the range sum `rangeSum(i, j)`, you
can get it in the following way:

```C++
rangeSum(i, j) = sums[j] - sums[i - 1] // i > 0
rangeSum(i, j) = sums[j]               // i == 0
```

work with this option is a little complex, to get the `rangeSum(i, j)`:

```C++
rangeSum(i, j) = sums[j] - sums[i] + nums[i] // i >= 0
```

__Option 2:__ `sums.resize(n + 1, 0);`

In this case, each element `sums[i]` in sums represent the prefix sum of the
first `i` elements in original array nums. When you want to get the range sum by
`rangeSum(i, j)`, you can get it in the following way:

```C++
rangeSum(i, j) = sums[j + 1] - sums[i] // i >= 0
```

### Using prefix sum with map

One of the core trick in solving the following subarray problems is to build a
map from prefix sum to array index for efficient lookup. For example, problems
with keywaords "maximum size equal to K", "differ by K", or "differ by multiple
of k" are solved using this trick. There are two hints.

!!! hint
    When a map is used, it need to be initialized using `<0, -1>`. It is useful
    for handling some of the corner cases such as `[-1, 1], -1` in the problem
    [Maximum Size Subarray Sum Equals k](./#maximum-size-subarray-sum-equals-k).

!!! hint
    It is usually easier to work with these problem when adding dummy element
    at the beginning of the array. For example: using `sums[i]` to represent the
    sum of first `i` element of array `nums`.

### Maximum Subarray

=== "C++ Greedy solution"

    ```C++
    // why this greedy solution works?
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int n = nums.size();
            int sum = 0;
            int max = 0;
            if (n == 0)
                return 0;

            max = nums[0];
            for (int i = 0; i < n; i++) {
                sum += nums[i];
                max = sum > max ? sum : max;
                sum = sum > 0 ? sum : 0;
            }

            return max;
        }
    };
    ```

!!! note
    Why can not compare to `f[i - 1])` to find the maximum. Because including the `f[i - 1]` will skip
    elements, the sum will not from a subarray, but sequence of numbers in the array. This is very similar to
    problems Longest Common Substring and Longest Common Subsequence

=== "C++ DP"

    ```c++ hl_lines="11"
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int n = nums.size();
            int res = INT_MIN;

            int f[n + 1] = {0}; // f[i] = maxSubArray of first i elements
            f[0] = 0;           // initial value

            for (int i = 1; i <= n; i++) {
                f[i] = max(f[i - 1] + nums[i - 1], nums[i - 1]);
                res = max(f[i], res);
            }

            return res;
        }
    };
    // Notice this is a coordinate based DP problem, the meaning of the index i
    // in nums and f are different.
    ```

Kadane's solution

This is a DP solution, it reduced the f array to two variables. Making the problem $O(n)$ in space.
[Discuss about this solution](http://blog.csdn.net/linhuanmars/article/details/21314059),
where it make use of the idea of global maximum and local maximum.

!!! note
    The idea of global maximum and local maximum is very usefull to solve DP problems.
    The local maximum is the maximum sum of a continuous subarray, the global maximum
    is keep the maximum of the local mmaximum.

=== "C++ Kadane's solution" hl_lines="9"

    ```c++
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int n = nums.size();
            int res = INT_MIN;
            int curr = 0;

            for (int i = 0; i < n; i++) {
                curr = max(curr + nums[i], nums[i]);
                res = max(curr, res);
            }

            return res;
        }
    };
    ``` 

Prefix sum solution

The ideas is we have array sums, `sums[i] = A[0] +, ... + A[i]`, called prefix sum.
With one for loop we can find the maxSum so far and the minSum before it.
The difference is the possible results, we collect the maximum of those differences.

=== "C++ prefix sum solution"

    ```c++
    public class Solution {
        public int maxSubArray(int[] A) {
            if (A == null || A.length == 0){
                return 0;
            }

            int max = Integer.MIN_VALUE, sum = 0, minSum = 0;
            for (int i = 0; i < A.length; i++) {
                sum += A[i];
                max = Math.max(max, sum - minSum);
                minSum = Math.min(minSum, sum);
            }

            return max;
        }
    };
    ```

### Maximum Subarray II*

```text
Given an array of integers, find two non-overlapping subarrays which have the
largest sum. The number in each subarray should be contiguous. Return the largest
sum.

Notice
The subarray should contain at least one number

Example
For given [1, 3, -1, 2, -1, 2], the two subarrays are [1, 3] and [2, -1, 2] or
[1, 3, -1, 2] and [2], they both have the largest sum 7.
```

Prefix sum solution

=== "C++ Prefix sum solution"

    ```C++ hl_lines="18 19 29 30"
    class Solution {
    public:
        /*
        * @param nums: A list of integers
        * @return: An integer denotes the sum of max two non-overlapping subarrays
        */
        int maxTwoSubArrays(vector<int> nums) {
            int n = nums.size();
            int minSum = 0;
            int sums = 0;
            int maxSum = INT_MIN;
            int left [n] = {0};
            int right [n] = {0};

            /* calculate the prefix sum */
            for (int i = 0; i < n; i++) {
                sums += nums[i];
                maxSum = max(maxSum, sums - minSum); // minSum is previous calculated
                minSum = min(minSum, sums);
                left[i] = maxSum;
            }

            /* calculate the postfix sum */
            minSum = 0;
            sums = 0;
            maxSum = INT_MIN;
            for (int i = n - 1; i >= 0; i--) {
                sums += nums[i];
                maxSum = max(maxSum, sums - minSum);
                minSum = min(minSum, sums);
                right[i] = maxSum;
            }

            /* iterate the divider line, left[i] stored the maxSubArraySum
            * from nums[0] to nums[i], similar for right[i] */
            maxSum = INT_MIN;
            for (int i = 0; i < n - 1; i++) {
                maxSum = max(maxSum, left[i] + right[i + 1]);
            }

            return maxSum;
        }
    };
    ```

!!! warning
    Cannot swap the highlighted lines. Because the maximum sum is calculated from
    current sum minus the previous minSum.

### Maximum Subarray III*

```text
Given an array of integers and a number k, find k non-overlapping subarrays which
have the largest sum. The number in each subarray should be contiguous.

Return the largest sum.

Notice
The subarray should contain at least one number

Example
Input:
List = [-1,4,-2,3,-2,3]
k = 2
Output: 8
Explanation: 4 + (3 + -2 + 3) = 8
```

DP solution

Use the idea of global maximum and local maximum from [Maximum Subarray](./#maximum-subarray).
See [this artical](https://zhengyang2015.gitbooks.io/lintcode/maximum_subarray_iii_43.html)
for detailed explaination of the solution.

```c++
class Solution {
public:
    /**
     * My initial try: O(n^2 k)
     * Partitioning DP: f[n][k], maximum K subarrays of first n elements.
     * Last partition: A[j] ,... A[n - 1]
     * f[i][k] = max_{0 <= j < i}(f[j][k - 1] + MS(A[j] ,... A[i - 1]))
     * f[0][0] =
     *
     * Solution 2, O(nk)
     * local[i][k]: Max k subarray sum from "first i elements" that include nums[i]
     * global[i][k]: Max k subarray sum from "first i elements" that may not include nums[i]
     *
     * 2 cases:     nums[i - 1] is kth subarray, nums[i - 1] belongs to kth subarray
     * local[i][k] = max(global[i - 1][k - 1], local[i - 1][k]) + nums[i - 1]
     *
     * 2 cases:     not include nums[i - 1], include nums[i - 1]
     * global[i][k] = max(global[i - 1][k], local[i][k])
     */
    int maxSubArray(vector<int> nums, int k) {
        int n = nums.size();

        int local[n + 1][k + 1] = {0};
        int global[n + 1][k + 1] = {0};

        for (int j = 1; j <= k; j++) {
            // first j - 1 elements cannot form j groups, set to INT_MIN.
            local[j - 1][j] = INT_MIN;
            for (int i = j; i <= n; i++) { // must: i >= k.
                local[i][j] = max(global[i - 1][j - 1], local[i - 1][j]) + nums[i - 1];
                // the case when we divide k elements into k groups.
                if (i == j) {
                    global[i][j] = local[i][j];
                } else {
                    global[i][j] = max(global[i - 1][j], local[i][j]);
                }
            }
        }

        return global[n][k];
    }
};
```

### Maximum Subarray Difference*

```text
Given an array with integers.
Find two non-overlapping subarrays A and B, which |SUM(A) - SUM(B)| is the largest.

Return the largest difference.

Notice
The subarray should contain at least one number

Example
For [1, 2, -3, 1], return 6.
```

Prefix sum solution

We use the similar idea for problem [Maximum Subarray II](./#maximum-subarray-ii).
We have to maintain four arrays. from forward maximum and minimum subarray sum
and backward maximum and minimum subarray sum.

=== "C++ Prefix sum solution"

    ```C++
    class Solution {
    public:
        /*
        * @param nums: A list of integers
        * @return: value of maximum difference between two subarrays
        */
        int maxDiffSubArrays(vector<int> nums) {
            int n = nums.size();
            int minSum = 0;
            int sums = 0;
            int maxSum = INT_MIN;

            int left_max[n] = {0};
            int left_min[n] = {0};
            int right_max[n] = {0};
            int right_min[n] = {0};

            for (int i = 0; i < n; i++) {
                sums += nums[i];
                maxSum = max(maxSum, sums - minSum);
                minSum = min(minSum, sums);
                left_max[i] = maxSum;
                //left_min[i] = minSum;
            }

            minSum = INT_MAX;
            sums = 0;
            maxSum = 0;
            for (int i = 0; i < n; i++) {
                sums += nums[i];
                minSum = min(minSum, sums - maxSum);
                maxSum = max(maxSum, sums);
                //left_max[i] = maxSum;
                left_min[i] = minSum;
            }

            minSum = 0;
            sums = 0;
            maxSum = INT_MIN;
            for (int i = n - 1; i >= 0; i--) {
                sums += nums[i];
                maxSum = max(maxSum, sums - minSum);
                minSum = min(minSum, sums);
                right_max[i] = maxSum;
                //right_min[i] = minSum;
            }

            minSum = INT_MAX;
            sums = 0;
            maxSum = 0;
            for (int i = n - 1; i >= 0; i--) {
                sums += nums[i];
                minSum = min(minSum, sums - maxSum);
                maxSum = max(maxSum, sums);
                //right_max[i] = maxSum;
                right_min[i] = minSum;
            }

            int diff = INT_MIN;
            for (int i = 0; i < n - 1; i++) {
                diff = max(left_max[i] - right_min[i + 1], diff);
                diff = max(right_max[i + 1] - left_min[i], diff);
            }

            return diff;
        }
    };
    ```

### Maximum Product Subarray

DP solution

* It is similar to the problem [Maximum Subarray](./#maximum-subarray). Notice
  the negative number, min multiply a minus number could become the largest product.

```c++
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        int max_pro[n] = {0};
        int min_pro[n] = {0};
        int result = nums[0];

        max_pro[0] = nums[0];
        min_pro[0] = nums[0];

        for (int i = 1; i < n; i++) {
            if (nums[i] > 0) {
                max_pro[i] = max(max_pro[i - 1] * nums[i], nums[i]);
                min_pro[i] = min(min_pro[i - 1] * nums[i], nums[i]);
            } else {
                max_pro[i] = max(min_pro[i - 1] * nums[i], nums[i]);
                min_pro[i] = min(max_pro[i - 1] * nums[i], nums[i]);
            }

            result = max(result, max_pro[i]);
        }

        return result;
    }
};
```

Constant space solution

Without need to check whether `nums[i]` is positive is negative, we can just find
the maximum or minium of three cases.

```c++
class Solution {
public:
    /*
     * @param nums: An array of integers
     * @return: An integer
     */
    int maxProduct(vector<int> nums) {
        int n = nums.size();
        int res = nums[0];
        int cur_max = nums[0];
        int cur_min = nums[0];

        for (int i = 1; i < n; i++) {
            int tmp = cur_max;
            cur_max = max(max(cur_max * nums[i], nums[i]), cur_min * nums[i]);
            cur_min = min(min(cur_min * nums[i], nums[i]), tmp * nums[i]);
            res = max(res, cur_max);
        }

        return res;
    }
};
```

### Subarray Product Less Than K

### Subarray Sum*

```text
Given an integer array, find a subarray where the sum of numbers is zero.
Your code should return the index of the first number and the index of the last
number.

Notice
There is at least one subarray that it's sum equals to zero.

Example
Given [-3, 1, 2, -3, 4], return [0, 2] or [1, 3].
```

Hash solution

use a hash table to keep the prefix sum. Once we see another prefix sum that
exists in the hash table, we discovered the subarray that sums to zero. However,
pay attention to the indexing, because it requires to return the original array's index.

```c++
class Solution {
public:
    /**
     * @param nums: A list of integers
     * @return: A list of integers includes the index of the first number
     *          and the index of the last number
     */
    vector<int> subarraySum(vector<int> nums){
        int n = nums.size();
        vector<int> res(2, 0);
        int sum = 0;
        unordered_map<int, int> map;

        map[0] = -1; //important
        for (int i = 0; i < n; i++) {
            sum += nums[i];
            if (map.count(sum) != 0) {
                res[0] = map[sum] + 1;
                res[1] = i;
                break;  
            }

            map[sum] = i;
        }

        return res;
    }
};
// test cases
//    [-1, 2, 3, -3] A
//  [0,-1, 1, 4,  1] sum
//         i      j
```

!!! note
    Pay attention to the initial value and initializethe `map[0] = -1`; This can
    be validated with an edge case. The time complexity is O(n)

Prefix sum solution

Calculate the prefix sum first and then use the prefix sum to find the subarray.
This solution is $O(n^2)$

```c++
class Solution {
public:
    vector<int> subarraySum(vector<int> nums){
        int n = nums.size();
        vector<int> res(2, 0);
        vector<int> sum(n + 1, 0);
        sum[0] = 0;

        for (int i = 1; i <= n; i++) {
            sum[i] = sum[i - 1] + nums[i - 1];
        }

        for (int i = 0; i < n; i++) {
            for (int j = i; j <= n; j++) {
                if (j > 1 && sum[j] - sum[i] == 0) {
                    res[0] = i;
                    res[1] = j - 1;
                    break;
                }
            }
        }

        return res;
    }
};
```

### Minimum Size Subarray Sum

Accumulative sum solution

Using accumulative sum and another moving pointer to check both the sum and the
length of the subarray.

```C++
class Solution {
public:
    int minSubArrayLen(int s, vector<int>& nums) {
        int n = nums.size();
        if (n == 0)
            return 0;

        int sum = 0;
        int res = INT_MAX;
        int left = 0;

        for (int i = 0; i < n; i++) {
            sum += nums[i];
            while (sum >= s) {
                res = min(res, i - left + 1);
                sum -= nums[left++];
            }
        }

        return res != INT_MAX ? res : 0;
    }
};
```

### Maximum Size Subarray Sum Equals k

Similar to [Continuous Subarray Sum](./continuous-subarray-sum)

Hash solution

Use a hash table to keep `<sums, i>` entries. Look up using `sum - k`. We only
add to the hash table for the first time a value is appeared. It ensures the
length of the found subarray is the largest. Notice you also have to initialize
the hash with value `<0, -1>` to handle the edge case.

```C++
class Solution {
public:
    int maxSubArrayLen(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0)
            return 0;

        unordered_map<int, int> map;
        int sum = 0;
        int left = 0;
        int res = INT_MIN;

        map[0] = -1;
        for (int i = 0; i < n; i++) {
            sum += nums[i];
            if (map.count(sum - k) != 0) {
                left = map[sum - k] + 1;
                res = max(res, (i - left + 1));
            }

            if (map.count(sum) == 0) {
                map[sum] = i;
            }
        }

        return res != INT_MIN ? res : 0;
    }
};
/* test cases:
1. [-1, 1], -1 如果没有初始化hash，这个case就会错误
   [-1, 0] sums
2. [-1], 0
3. [-1], -1
4. if return the result will be 1 if there is no res variable
   [1, 1, 0], 1
   [1, 2, 2], 1
*/
```

### Subarray Sum Equals K

Prefix sum solution

Use prefix sum to find the subarray sum. Two pointer to check all the possible subarray sum.

=== "C++ prefix sum solution"

```C++
public class Solution {
    public int subarraySum(int[] nums, int k) {
        int count = 0;
        int[] sum = new int[nums.length + 1];
        sum[0] = 0;

        for (int i = 1; i <= nums.length; i++)
            sum[i] = sum[i - 1] + nums[i - 1];

        for (int start = 0; start < nums.length; start++) {
            for (int end = start + 1; end <= nums.length; end++) {
                if (sum[end] - sum[start] == k)
                    count++;
            }
        }
        return count;
    }
}
```

Hash solution

Use a map to store the prefix sum and a counter. The idea is while calculating
prefix sums, if we find an `sums - k` exist in the map, we found one of target
subarray. The subtilty is for a particular prefix sum, there might be multiple
earlier prefix sums differ from it by k. We should take this into account.
Compare to the hash solution for problem [Subarray Sum](./subarray-sum).

```text
/*
  k =  2
  i =  1,  2,  3
sum =  1,  2,  3
cnt =  0,  1,  2
key =  1,  2,  3
val =  1,  1,  1
the reason that the cnt += map[sum - k], not cnt += 1 is that
the prefix sum "sum - k" has been shown up for total of map[sum - k] times.
All those prefix sum could be result of distinct subarrays between current
prefix sum and previous prefix sum "sum - k"
*/
```

```C++
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        // key=prefix sum, val=appearance
        unordered_map<int, int> map;

        int cnt = 0;
        int sum = 0;

        map[0] = 1;
        for (int i = 0; i < n; i++) {
            sum += nums[i];
            if (map.count(sum - k) != 0) {
                cnt += map[sum - k];
            }

            map[sum] += 1;
        }

        return cnt;
    }
};
```

!!! warning
    Notice you have to initialize the `map[0] = 1`; this is because for cases such
    as `[1, 1, 1]`, when `i = 1`, `sum = 2`, `[1,1]` should be counted as one subarray.
    Without setting `map[0] = 1` at first hand, it will give incorrect results.

### Subarray Sums Divisible by K

* preSum is the basis of continuous subarray profblem.
* One pass solution should explore a property of modulo `(preSum[j] - preSum[i]) % K == 0`
  indicate the when removing `K` from the larger value `preSum[j]` `n` times, we
  get the smaller value, then `preSum[j] % K == preSum[i] % K`. This property
  makes the one pass solution possible.
* Once the modulo property is found, we can use Hash map to assist our counting.
  The ideas is to keep counting the remainder, once we have seen the same
  remainder in the map, the new index and all the found indexes can be used to
  retrive one solution. The count keep in the map show how many of those can be.
  
=== "One pass solution"

```c++
class Solution {
public:
    int subarraysDivByK(vector<int>& A, int K) {
        int n = A.size();
        if (n == 0 || K == 0) {
            return 0;
        }

        int preSum = 0;
        unordered_map<int, int> mp;
        mp[0] = 1;
        int count = 0;

        for (int i = 0; i < n; i++) {
            preSum += A[i];

            int reminder = preSum % K;
            // deal with negative values
            if (reminder < 0)
                reminder += K;

            if (mp.find(reminder) != mp.end()) {
                count += mp[reminder];
            }

            mp[reminder] += 1;
        }

        return count;
    }
};
```

==="Naive solution O(n^2)"

```C++
class Solution {
public:
    int subarraysDivByK(vector<int>& A, int K) {
        int n = A.size();
        if (n == 0 || K == 0) {
            return 0;
        }

        vector<long> preSum(n + 1, 0);
        preSum[0] = 0;
        for (int i = 0; i < n; i++) {
            preSum[i + 1] = preSum[i] + A[i];
        }
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j <= n; j++) {
                if ((preSum[j] - preSum[i]) % K == 0) {
                    cnt++;
                }
            }
        }

        return cnt;
    }
};
```

### Max Sum of Subarry No Larger Than K*

1. This problem in geeksforgeeks as "Maximum sum subarray having sum less than or equal to given sum".
2. It has been discussed here.
3. This problem is the basis to solve the problem 363. Max Sum of Rectangle No Larger Than K.

Solution 1 using prefix sum and set

1. calculate prefix and using a set to store individual prefix sum, (`vector`
   also works). In each iteration, we lookup the value `preSum - k` in the set.
2. Notice we can use binary search to find the smallest element that `>= preSum - k`.
   We can use `lower_bound` to achieve that. Notice if it is asking the sum less
   than k we have to use `upper_bound`

```C++
int maxSumSubarryNoLargerThanK (int A[], int n, int k) {
    set<int> preSumSet;
    preSumSet.insert(0);

    int res = 0, preSum = 0;
    for (int i = 0; i < n; ++i) {
        preSum += A[i];
        set<int>::iterator siter = preSumSet.lower_bound(preSum - k);
        if (siter != preSumSet.end() {
            res = max(res, preSum - *siter);
        }
        preSumSet.insert(preSum);
    }

    return res;
}
```

### Max Sum of Rectangle No Larger Than K

Solution 1 iterate the wide of the matrix and using prefix sum and set `lower_bound`.

1. To optimize it with the brute force solution, you will find this problem is a
   combination of the problem Maximum Sum Rectangular Submatrix in Matrix and
   problem Max Sum of Subarry No Larger Than K.
2. From the problem Max Sum of Subarry No Larger Than K, we have to enumerate
   the width of the sub-matrix and sum up all row elements and get an array
   of length `m`, `m` is the number of rows of the matrix. Then apply the method.

```C++
class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        if (matrix.empty()) return 0;
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;
        int res = INT_MIN;

        for (int l = 0; l < n; ++l) {
            vector<int> sums(m, 0);
            for (int r = l; r < n; ++r) {
                for (int i = 0; i < m; ++i) {
                    sums[i] += matrix[i][r];
                }

                set<int> preSumSet;
                preSumSet.insert(0);
                int preSum = 0, curMax = INT_MIN;
                for (int sum : sums) {
                    preSum += sum;
                    set<int>::iterator it = preSumSet.lower_bound(preSum - k);
                    if (it != preSumSet.end()) {
                        curMax = max(curMax, preSum - *it);
                    }
                    preSumSet.insert(preSum);
                }

                res = max(res, curMax);
            }
        }

        return res;
    }
};
```

!!! note
    1. The complexity is $n⋅n⋅(m+m\log m)=O(n⋅n⋅m\log m)$
    2. Notice the use of lower_bound, this function return iterator point to
    element greater than or equal to the value curSum - k, if use upper_bound,
    it will return iterator points to element greater than curSum - k, which
    would miss the equal to K case.

Solution 2 using merge sort

1. The idea is similar that solution 1. Instead of calculate `preSum` on the
   fly, we finish calculation and pass it to a `mergeSort` routine.
2. The use `mergeSort` here is to find the `A[j] - A[i] <= k` efficiently,
   `O(nlogn)`.
3. The complexity is n⋅n⋅(m+m⋅\log m)=O(n⋅n⋅m⋅\log m)

```C++
class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;
        int res = INT_MIN;
        vector<long long> sums(m + 1, 0);

        for (int l = 0; l < n; ++l) {
            vector<long long>sumInRow(m, 0);
            for (int r = l; r < n; ++r) {
                for (int i = 0; i < m; ++i) {
                    sumInRow[i] += matrix[i][r];
                    sums[i + 1] = sums[i] + sumInRow[i];
                }
                res = max(res, mergeSort(sums, 0, m + 1, k));
                if (res == k) return k;
            }
        }

        return res;
    }

    int mergeSort(vector<long long>& sums, int start, int end, int k) {
        if (end == start + 1) return INT_MIN;

        int mid = start + (end - start) / 2;
        int res = mergeSort(sums, start, mid, k);
        if (res == k) return k;

        res = max(res, mergeSort(sums, mid, end, k));
        if (res == k) return k;

        long long cache[end - start];

        int j = mid, c = 0, t = mid;
        for (int i = start; i < mid; ++i) {
            /* search first time sums[j] - sums[i] > k */
            while (j < end && sums[j] - sums[i] <= k) ++j;
            /* sums[j - 1] - sums[i] <= k, make sure j - 1 is in right side */
            if (j - 1 >= mid) {
                res = max(res, (int)(sums[j - 1] - sums[i]));
                if (res == k) return k;
            }
            /* parallel merge */
            while (t < end && sums[t] < sums[i]) {
                cache[c++] = sums[t++];
            }
            cache[c++] = sums[i];
        }

        /* parallel merge */
        for (int i = start; i < t; ++i) {
            sums[i] = cache[i - start];
        }

        return res;
    }
};
```

### Maximum Sum Rectangular Submatrix in Matrix*

### Subarray Sum Closest*

### Shortest Unsorted Continuous Subarray (Count inversions)

Solution 1 using merge sort

1. This problem is the basic of using merge sort to solve lots of hard problems.

```C++
long long merge_and_count(int A[], int start, int end) {
    if (end - start <= 1) return 0;

    long long count = 0;
    int mid = start + (end - start) / 2;
    count = merge_and_count(A, start, mid)
            + merge_and_count(A, mid, end);

    int j = mid;
    int cache[end - start];
    for (int i = start, k = 0; i < mid; i++) {
        while (j < end && A[i] > A[j]) {
            cache[k++] = A[j++];
            count += mid - i;
        }

        cache[k++] = A[i];
    }
    // copy cache[0, j - start) to A[start, j)
    // if j < end, then A[j, end) in final place.
    for (int i = start; i < j; i++) {
        A[i] = cache[i - start];
    }

    return count;
}
```

### Count Inversion (course assignment)

Solution 1 using merge sort

1. This problem is the basic of using merge sort to solve lots of hard problems.

```c++
long long merge_and_count(int A[], int start, int end) {
    if (end - start <= 1) return 0;

    long long count = 0;
    int mid = start + (end - start) / 2;
    count = merge_and_count(A, start, mid)
            + merge_and_count(A, mid, end);

    int j = mid;
    int cache[end - start];
    for (int i = start, k = 0; i < mid; i++) {
        while (j < end && A[i] > A[j]) {
            cache[k++] = A[j++];
            count += mid - i;
        }

        cache[k++] = A[i];
    }
    // copy cache[0, j - start) to A[start, j)
    // if j < end, then A[j, end) in final place.
    for (int i = start; i < j; i++) {
        A[i] = cache[i - start];
    }

    return count;
}
```

### Count of Smaller Numbers After Self

Solution 1 Merge sort

1. One important point to remember is you have to create pairs out of the array
   element and its index, because during merge sort, when we count each value,
   we don't know where to put those count values in the result vector.
2. The second merge solutions run much faster than the first one.

=== "C++ Merge sort"

    ```c++
    class Solution {
    public:
        vector<int> countSmaller(vector<int>& nums) {
            int n = nums.size();
            vector<int> res(n, 0);
            vector<pair<int, int> > vp;

            for (int i = 0; i < n; i++) {
                vp.emplace_back(nums[i], i);
            }

            merge_sort_count(vp, 0, n, res);

            return res;
        }

    private:
        void merge_sort_count(vector<pair<int, int> >& nums, int start, int end,
                            vector<int>& res) {
            if (end - start <= 1)
                return;

            int mid = start + (end - start) / 2;

            merge_sort_count(nums, start, mid, res);
            merge_sort_count(nums, mid, end, res);

            vector<pair<int, int> > cache(end - start, pair<int, int>(0, 0));

            int j = mid, k = 0, t = mid;
            for (int i = start; i < mid; i++) {
                j = mid;
                while (j < end && nums[i].first > nums[j].first) { // found smaller elements
                    res[nums[i].second]++;
                    j++;
                }

                while (t < end && nums[i].first > nums[t].first) {
                    cache[k++] = nums[t++];
                }

                cache[k++] = nums[i];
            }

            for (int i = start; i < j; i++) {
                nums[i] = cache[i - start];
            }

            return;
        }
    };
    ```

=== "C++ more efficient  Merge sort"

    ```C++
    class Solution {
    public:
        vector<int> countSmaller(vector<int>& nums) {
            int n = nums.size();
            vector<int> res(n, 0);
            vector<pair<int, int> > vp;

            for (int i = 0; i < n; i++) {
                vp.emplace_back(nums[i], i);
            }

            mergeSort(vp, 0, n, res);

            return res;
        }

        void mergeSort(vector<pair<int, int>>& x, int start,
                    int end, vector<int>& res) {
            if (end - start <= 1) return;

            int mid = start + (end - start) / 2;
            mergeSort(x, start, mid, res);
            mergeSort(x, mid, end, res);

            vector<pair<int, int> > cache(end - start, pair<int, int>(0, 0));

            int i = start, j = mid, k = 0;
            while(i < mid && j < end) {
                if (x[i].first <= x[j].first) {
                    cache[k++] = x[i];
                    res[x[i].second] += j - mid;
                    ++i;
                } else {
                    cache[k++] = x[j++];
                }
            }

            while(i < mid) {
                cache[k++] = x[i];
                res[x[i].second] += end - mid;
                ++i;
            }

            while(j < end)
                cache[k++] = x[j++];

            for(i = start, k = 0; i < end; ++i, ++k) {
                x[i] = cache[k];
            }
        }
    };
    ```

=== "C++ BST"

    ```C++
    class Solution {
    public:
        class TreeNode {
        public:
            int val, smallerCnt;
            TreeNode* left, *right;
            TreeNode(int v, int s) : left(NULL), right(NULL), val(v), smallerCnt(s){}
        };
        vector<int> countSmaller(vector<int>& nums) {
            int n = nums.size();
            if(n == 0) return {};
            vector<int> res(n, 0);
            TreeNode* root = NULL;

            for(int i = n - 1; i >= 0; --i)
                root = insert(root, nums[i], i, 0, res);

            return res;
        }

    private:
        TreeNode* insert( TreeNode* node, int val, int idx, int preSum, vector<int>& res) {
            if(node == NULL) {
                node = new TreeNode(val, 0);
                res[idx] = preSum;
            } else if(node->val > val) {
                node->smallerCnt++;
                node->left = insert(node->left, val, idx, preSum, res);
            } else {
                node->right = insert(node->right, val, idx,
                                    preSum + node->smallerCnt + ((node->val < val)? 1: 0), res);
            }

            return node;
        }
    };
    ```

### Continuous Subarray Sum

Hash solution

Once see a multiple of K, you should consider the modulor operation `%`

The values put into to the hash only for the first time, this is similar to the
case in the problem Maximum Size Subarray Sum Equals k.

=== "C++ Hash soution" hl_lines="11"

    ```c++
    class Solution {
    public:
        bool checkSubarraySum(vector<int>& nums, int k) {
            int n = nums.size();
            if (n == 0)
                return false;

            unordered_map<int, int> map;
            int sum = 0;

            map[0] = -1; // test case [0, 0], 0
            for (int i = 0; i < n; i++) {
                sum += nums[i];

                if (k != 0)
                    sum = sum % k;

                if (map.count(sum) != 0) {
                    if (i - map[sum] > 1) {
                        return true;
                    }
                } else {
                    map[sum] = i;
                }
            }

            return false;
        }
    };
    ```

### Contiguous Array

Similar problems:

* [Continuous Subarray Sum](./#continuous-subarray-sum)
* [Maximum Size Subarray Sum Equals k](./#maximum-size-subarray-sum-equals-k)

Hash solution

This problem is very similar to the problem [Continuous Subarray Sum](./#continuous-subarray-sum).
However, there is a trick to calculate the cummulative sum, treat `0` as `-1`.

```C++
class Solution {
public:
    int findMaxLength(vector<int>& nums) {
        int n = nums.size();
        int res = 0;
        int cnt = 0;
        unordered_map<int, int> map;

        map[0] = -1; // test case: [0, 1]
        for (int i = 0; i < n; i++) {
            cnt += nums[i] == 0 ? -1 : 1;
            if (map.count(cnt) != 0) {
                res = max(res, i - map[cnt]);
            } else {
                map[cnt] = i;
            }
        }

        return res;
    }
};
```

### Split Array with Equal Sum

Cummulative sum soluiton

Because of the symetric property of the head subarray and trailing subarray, we
can calculate cumulative sum from both direction. This can help to fix the index
`i` and `k`. we can enumerate the index `j` in between.

=== "C++ cummulateive sum solution"

    ```C++
    class Solution {
    public:
        bool splitArray(vector<int>& nums) {
            int n = nums.size();
            int sum1[n] = {0};
            int sum2[n] = {0};

            sum1[0] = nums[0];
            for (int i = 1; i < n; i++) {
                sum1[i] = sum1[i - 1] + nums[i];
            }

            sum2[n - 1] = nums[n - 1];
            for (int i = n - 2; i >= 0; i--) {
                sum2[i] = sum2[i + 1] + nums[i];
            }

            // notice the index bounds
            for (int i = 1; i < n - 5; i++) {
                for (int k = n - 2; k > i + 3; k--) {
                    if (sum1[i] - nums[i] == sum2[k] - nums[k]) {
                        for (int j = i + 2; j < k - 1; j++) {
                            int sumij = sum1[j] - nums[j] - sum1[i];
                            int sumjk = sum2[j] - nums[j] - sum2[k];
                            if (sumij == sumjk) {
                                return true;
                            }
                        }
                    }
                }
            }

            return false;
        }
    };
    ```

### 410. Split Array Largest Sum

Similar problems:

* [Copy Books (linktcode)](./#copy-books-lintcode).

DP solution

* Notice the edge case: `[1, INT_MAX]`, use `double` can avoid integer overflow.

Binary Search solution

* This is a greedy search solution that use binary search to accelerate the
  search speed
* The goal is to "minimize the largest sub-array sum". It is different from
  [Divide Chocolate](./#divide-chocolate), which is maximize the smallest sum.
* The bisection condition is not `A[m] < target` any more. It is a function to
  check whether the constrain can meet given a guess value `mid`.

=== "C++ DP"

    ```c++
    /**
    * equivalent to the lintcode copy books problem
    *
    * last step: mth subarray A[j], ..., A[i - 1].
    * State: f[m][n]: minmax sum of m subarrays that include n elements
    * Equation: f[m][n] = min_{0<=j<n}(max(f[m - 1][j], sum(A[j], ..., A[n - 1])))
    * Init: f[0][n] = INT_MAX;
    *       f[0][0] = 0;
    * NB: notice a special case: [1, 2147483247], 2
    *     the sum will overflow in the state update, You use a double type  
    */
    class Solution {
    public:
        int splitArray(vector<int>& nums, int m) {
            int n = nums.size();

            double f[m + 1][n + 1];
            f[0][0] = 0;
            for (int i = 1; i <= n; i++) {
                f[0][i] = INT_MAX;
            }

            double sum = 0;
            for (int k = 1; k <= m; k++) {
                f[k][0] = 0;
                for (int i = 1; i <= n; i++) {
                    sum = 0;
                    f[k][i] = INT_MAX;
                    for (int j = i; j >= 0; j--) { //j = i, mean sum = 0.
                        f[k][i] = min(f[k][i], max(f[k - 1][j], sum));
                        if (j > 0) {
                            sum += nums[j - 1];
                        }
                    }
                }
            }

            return f[m][n];
        }
    };
    ```

=== "C++ binary search"

    ```C++
    class Solution {
    public:
        int splitArray(vector<int>& nums, int m) {
            int total = 0;
            int mx = 0;
            for (int num: nums) {
                total += num;
                mx = max(mx, num);
            }

            int l = mx, r = total;
            while (l < r) {
                int mid = l + (r - l) / 2;

                if (!canCut(nums, mid, m - 1)) {
                    l = mid + 1;
                } else {
                    r = mid;
                }
            }

            return l;
        }

        // whether m cuts are possible, notice the greedy property of this check
        // you should notice that if not possible, it is because mid is too small,
        // not because it is too large.
        bool canCut(vector<int>& nums, int mid, int m) {
            int sum = 0;
            for (int num: nums) {
                if (num > mid) return false;
                else if (sum + num <= mid) sum += num; // each cut is greedy
                else { // cut is ok so far
                    m--;
                    if (m < 0) return false; // more element after all cuts.

                    sum = num; // init the next group sum
                }
            }

            return true;
        }
    };
    ```
=== "C++ binary search (count the # of cuts)"

    ```c++
    class Solution {
    public:
        int splitArray(vector<int>& nums, int m) {
            int l = *max_element(nums.begin(), nums.end());
            int r = accumulate(nums.begin(), nums.end(), 0);
            
            while (l < r) {
                int mid = l + (r - l) / 2;
                int s = 0;
                int c = 0;
                
                // count the possible cuts
                for (int n : nums) {
                    if ((s += n) > mid) {
                        s = n;
                        if (++c > m - 1) {
                            break;
                        }
                    }
                }
                
                if (c > m - 1) {
                    l = mid + 1;
                } else {
                    // the "> mid" above guarantee the "no greater than"
                    // the guess value, if c == m - 1, mid could be the result
                    r = mid;
                }
            }
            
            return l;
        }
    };
    ```

### Copy books (lintcode)

Description

Given n books and the ith book has `A[i]` pages. You are given `k` people to
copy the `n` books. the `n` books list in a row and each person can claim
a continuous range of the `n` books. For example, one copier can copy the books
from ith to jth continuously, but he can not copy the 1st book, 2nd book and 4th
 book (without the 3rd book).

They start copying books at the same time and they all cost 1 minute to copy 1
page of a book. What's the best strategy to assign books so that the slowest
copier can finish at the earliest time?

Example

Given array A = [3,2,4], k = 2. Return 5 (First person spends 5 minutes to copy
book 1 and book 2 and the second person spends 4 minutes to copy book 3.)

Solution 1 Binary search

See the solution for [410. Split Array Largest Sum](#410-split-array-largest-sum)

Solution 2 DP solution

There are `i` books, consider the last copier, he can copy `A[j], ..., A[i-1]`.
The first `k-1` copier copy `A[0], ..., A[j - 1]`.

* Define state: `f[k][i]`, meaning the k-th copier copy `i` books.
* State transition equation: $f[k][i] = \min_{0 \le j \le i} \max(f[k - 1][j], A[j] + ... + A[i - 1])$

=== "C++ DP solution"

    ```C++ hl_lines="34 35 36 37 38 39"
    class Solution {
    public:
        /**
        * last step: last copier copy A[j], ... A[i-1]
        * first k-1 copier --> A[0], ... A[j - 1].
        * f[k][i]: k copier copy i books.
        * f[k][i] = \min_{0 \le j \le i} \max(f[k - 1][j], A[j] + ... + A[i - 1])
        */
        int copyBooks(vector<int> &pages, int K) {
            // write your code here
            int n = pages.size();
            if (n == 0) {
                return 0;
            }

            if (K > n) {
                K = n;
            }

            int f[K + 1][n + 1];

            /* init */
            f[0][0] = 0;
            for (int j = 1; j <= n; j++) {
                f[0][j] = INT_MAX;
            }

            int sum = 0;
            for (int k = 1; k <= K; k++) {
                f[k][0] = 0;
                for (int i = 1; i <= n; i++) {
                    sum = 0;
                    f[k][i] = INT_MAX;
                    for (int j = i; j >= 0; j--) {
                        f[k][i] = min(f[k][i], max(f[k - 1][j], sum));
                        if (j > 0) {
                            sum += pages[j - 1];
                        }
                    }
                }
            }

            return f[K][n];
        }
    };
    ```

!!! note
    We have to enumerate the index `j`, the highlighted code used a clever technique
    to optimize this task. It enumerate `j` backwards. While this seems impossible
    at the first glance, how can you calculate the states from right to left in DP?
    Notice the index `j` is in the upper row (row `k-1`). Once we are in the `k`-th row, the
    values in the `k-1`-th row are all given.

### Maximum Average Subarray I

Prefix sum solution

```c++
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();
        double sums[n] = {0};
        double max_avg = INT_MIN;

        sums[0] = nums[0];
        for (int i = 1; i < n; i++) {
            sums[i] = sums[i - 1] + nums[i];
        }

        for (int i = k - 1; i < n; i++) {
            double avg = (sums[i] - sums[i - k + 1] + nums[i - k + 1]) / k;
            max_avg = max(max_avg, avg);
        }

        return max_avg;
    }
};
```

### Maximum Average Subarray II

Prefix sum solution

This is still a brute force solution.

* time complexity: $O(n^2)$
* space complexity: $O(n)$

Space optimized solution

* We could avoid using the prefix sum array and only use two variables to record
  the prefix sum at any particular instance. One for record prefix sum of exact
  `k` elements. Another for the inner loop to check whether removing an element
  from the beginning will make a new maximum value or not.

Space optimized solution

* We could avoid using the prefix sum array and only use two variables to record
  the prefix sum at any particular instance. One for record prefix sum of exact
  `k` elements. Another for the inner loop to check whether removing an element
  from the beginning will make a new maximum value or not.

Deque solution

=== "C++ prefix sum solution"

    ```C++
    class Solution {
    public:
        double findMaxAverage(vector<int>& nums, int k) {
            int n = nums.size();
            vector<int> sums = nums;

            for (int i = 1; i < n; ++i) {
                sums[i] = sums[i - 1] + nums[i];
            }

            double res = (double) sums[k - 1] / k;
            for (int i = k; i < n; ++i) {
                double t = sums[i];
                if (t > res * (i + 1)) res = t / (i + 1);
                for (int j = 0; j < i - k + 1; ++j) {
                    t = sums[i] - sums[j];
                    if (t > res * (i - j)) res = t / (i - j);
                }
            }

            return res;
        }
    };
    ```

=== "C++ space optimized"

    ```C++
    class Solution {
    public:
        double findMaxAverage(vector<int>& nums, int k) {
            int n = nums.size();
            /* range is half open */
            double sumsAll = accumulate(nums.begin(), nums.begin() + k, 0);
            double sums = sumsAll, res = sumsAll / k;

            for (int i = k; i < n; ++i) {
                sumsAll += nums[i];
                sums = sumsAll;
                if (sums > res * (i + 1)) res = sums / (i + 1);
                for (int j = 0; j < i - k + 1; ++j) {
                    sums -= nums[j];
                    if (sums > res * (i - j)) res = sums / (i - j);
                }
            }

            return res;
        }
    };
    ```

=== "C++ binary search soluiton"

    ```C++
    class Solution {
    public:
        double findMaxAverage(vector<int>& nums, int k) {
            int n = nums.size();
            double upper = INT_MIN, lower = INT_MAX;

            for (auto num : nums) {
                upper = max(upper, (double)num);
                lower = min(lower, (double)num);
            }

            while (lower + 0.00001 < upper) {
                double mid = lower + (upper - lower) / 2;
                if (isLarger(nums, mid, k)) { // is average value >= mid?
                    lower = mid;
                } else {
                    upper = mid;
                }
            }

            return lower;
        }

        /* return true if a greater average value is possible */
        bool isLarger(vector<int>& nums, double mid, int k) {
            int n = nums.size();
            double sums = 0, prev = 0, prev_min = 0;
            for (int i = 0; i < k; i++) {
                sums += nums[i] - mid;
            }

            if (sums >= 0) {
                return true;
            }
            /*
            * we keep looking for whether a subarray sum of length >= k in array
            * "sums" is possible to be greater than zero. If such a subarray exist,
            * it means that the target average value is greater than the "mid"
            * value. We look at the front part of sums that at least k element
            * apart from i. If we can find the minimum of the sums[0, 1, ..., i - k]
            * and check if sums[i] - min(sum[0, 1, ..., i - k]) >= 0. If this is the
            * case, it indicate there exist a subarray of length >= k with sum
            * greater than 0 in sums, we can return ture, otherwise, false.
            */
            for (int i = k; i < n; i++) {
                sums += nums[i] - mid;
                prev += nums[i - k] - mid;
                prev_min = min(prev_min, prev);
                if (sums >= prev_min)
                    return true;
            }

            return false;
        }
    };
    ```

=== "C++ deque solution"

    ```C++
    class Solution {
    public:
        double findMaxAverage(vector<int>& nums, int k) {
            int n = nums.size();
            vector<double> sums(n, 0);
            deque<int> q;

            sums[0] = nums[0];
            for (int i = 1; i < n; ++i)
                sums[i] = sums[i - 1] + nums[i];

            double res = sums[n - 1] / n;
            for (int j = k - 1; j < n; ++j)
            {
                while(q.size() >= 2 &&
                    density(sums, q[q.size() - 2], q.back() - 1) >=
                    density(sums, q.back(), j - k)) {

                    q.pop_back();
                }

                q.push_back(j - k + 1);

                while(q.size() >= 2 &&
                    density(sums, q[0], j) <= density(sums, q[1], j)) {

                    q.pop_front();
                }

                res = max(res, density(sums, q.front(), j));
            }

            return res;
        }

    private:
        double density(vector<double>& sums, int l, int r) {
            if (l == 0)
                return sums[r] / (r + 1);

            return (sums[r] - sums[l - 1]) / (r - l + 1);
        }
    };
    ```

!!! note
    Notice the initial value of prev_min is set to 0 not INT_MAX;
    Try to understand why set the initial value of prev_min to INT_MAX cannot
     pass the test case: `[8,9,3,1,8,3,0,6,9,2]`, 8.

### Range Sum Query - Immutable

Prefix sum solution

Use prefix sum to record the accumulative sum of the array in the constructor.
The algorithm is $O(n)$ in space and $O(1)$ in time.

```C++
class NumArray {
private:
    vector<int> sums;
    int n;
public:
    NumArray(vector<int> nums) {
        n = nums.size();
        sums.resize(n + 1, 0);

        sums[0] = 0;
        for (int i = 1; i <= n; ++i) {
            sums[i] = sums[i - 1] + nums[i - 1];
        }
    }

    int sumRange(int i, int j) {
        return sums[j + 1] - sums[i];
    }
};
/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray obj = new NumArray(nums);
 * int param_1 = obj.sumRange(i,j);
 */
```

### Range Sum Query - Mutable

Segment tree solution

Using segment tree, the solution is given at [Leetcode Solution](https://leetcode.com/problems/range-sum-query-mutable/solution/).

=== "C++ segment tree"

    ```C++
    class NumArray {
    private:
        vector<int> tree;
        int n;
    public:
        NumArray(vector<int> nums) {
            n = nums.size();
            tree.resize(2 * n, 0);

            for (int i = n, j = 0; i < 2 * n; ++i, ++j) {
                tree[i] = nums[j];
            }

            for (int i = n - 1; i > 0; --i) {
                tree[i] = tree[2 * i] + tree[2 * i + 1];
            }
        }

        void update(int i, int val) {
            int pos = n + i;
            int left = 0;
            int right = 0;

            tree[pos] = val;
            while (pos > 0) {
                left = pos;
                right = pos;
                if (pos % 2 == 0) {
                    right = pos + 1;
                }

                if (pos % 2 == 1) {
                    left = pos - 1;
                }

                tree[pos / 2] = tree[left] + tree[right];
                pos /= 2;
            }
        }

        int sumRange(int i, int j) {
            int left = i + n;
            int right = j + n;
            int sum = 0;

            while (left <= right) {
                if (left % 2 == 1) {
                    sum += tree[left];
                    left++;
                }

                if (right % 2 == 0) {
                    sum += tree[right];
                    right--;
                }

                left /= 2;
                right /= 2;
            }

            return sum;
        }
    };

    /**
    * Your NumArray object will be instantiated and called as such:
    * NumArray obj = new NumArray(nums);
    * obj.update(i,val);
    * int param_2 = obj.sumRange(i,j);
    */
    ```

Binary Indexed Tree solution I

Using Binary indexed tree, we are able to solve it optimally in $O(\log n)$ .
The solution originally from [here](https://leetcode.com/problems/range-sum-query-mutable/discuss/75753/java-using-binary-indexed-tree-with-clear-explanation)

```c++
class NumArray {
private:
    vector<int> arr;
    vector<int> BIT;
    int n;
public:
    NumArray(vector<int> nums) {
        arr = nums;
        n = nums.size();
        BIT.resize(n + 1, 0);

        for (int i = 0; i < n; i++) {
            init(i, arr[i]);
        }
    }

    void init(int i, int val) {
        i++;
        while(i <= n) {
            BIT[i] += val; // BIT[i]  = nums[0, i - 1];
            i += i & (-i);
        }
    }

    void update(int i, int val) {
        int diff = val - arr[i];
        arr[i] = val;
        init(i, diff);
    }

    int sumRange(int i, int j) {
        return getSum(j) - getSum(i - 1);
    }

    int getSum(int i) {
        i++;
        int sum = 0;
        while(i > 0) {
            sum += BIT[i];
            i -= i & (-i);
        }

        return sum;
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray obj = new NumArray(nums);
 * obj.update(i,val);
 * int param_2 = obj.sumRange(i,j);
 */
```

Binary Indexed Tree solution II

Similar to the above solution, We have combined the `init` and `update`. To make
it consistant with the solution with problem [Range Sum Query 2D - Mutable](#range-sum-query-2d-mutable)

```c++
class NumArray {
private:
    vector<int> arr;
    vector<int> BIT;
    int n;
public:
    NumArray(vector<int> nums) {
        n = nums.size();
        BIT.resize(n + 1, 0);
        arr.resize(n, 0);

        for (int i = 0; i < n; i++) {
            update(i, nums[i]);
        }
    }

    /* We can combine the init and update like this */
    void update(int i, int val) {
        int diff = val - arr[i];
        arr[i] = val; // here we initialize arr[i]

        i++;
        while(i <= n) {
            BIT[i] += diff; // BIT[i]  = nums[0, i - 1];
            i += i & (-i);
        }
    }

    int sumRange(int i, int j) {
        return getSum(j) - getSum(i - 1);
    }

    int getSum(int i) {
        i++;
        int sum = 0;
        while(i > 0) {
            sum += BIT[i];
            i -= i & (-i);
        }

        return sum;
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray obj = new NumArray(nums);
 * obj.update(i,val);
 * int param_2 = obj.sumRange(i,j);
 */
```

### Range Sum Query 2D - Immutable

Prefix sum solution

Extended from the 1d array, we can use the prefix sum of the 2d matrix. we use
extra space to store the accumulative sum of the submatrix with upper left
coordinate `(0, 0)` and lower right coordinate `(i, j)`.

=== "C++ prefix sum solution"

```c++
class NumMatrix {
private:
    vector<vector<int> > dp;
public:
    NumMatrix(vector<vector<int>> matrix) {
        int m = matrix.size();
        if (m == 0) return;

        int n = matrix[0].size();

        //dp = vector<vector<int> (m + 1, vector<int>(n + 1, 0));
        dp.resize(m + 1, vector<int>(n + 1, 0));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                dp[i + 1][j + 1] = dp[i][j + 1] + dp[i + 1][j] + matrix[i][j] - dp[i][j];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        return dp[row2 + 1][col2 + 1] - dp[row2 + 1][col1] - dp[row1][col2 + 1] + dp[row1][col1];
    }
};

/**
 * Your NumMatrix object will be instantiated and called as such:
 * NumMatrix obj = new NumMatrix(matrix);
 * int param_1 = obj.sumRegion(row1,col1,row2,col2);
 */
```

!!! note
    This version create the `m + 1` by `n + 1` dp array to record the prefix sum.
    The code is clean and elegant.

Alternative prefix sum solution

The idea is the same, in the following solution, we have a `m` by `n` 2d array
to record the accumulative sum. See how complex the code is.

=== "C++ prefix sum solution"

```c++
class NumMatrix {
private:
    vector<vector<int>> dp;
    int m;
    int n;
public:
    NumMatrix(vector<vector<int>> matrix) {
        m = matrix.size();
        if (m == 0)
            return;
        n = matrix[0].size();
        dp.resize(m, vector<int>(n, 0));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                dp[i][j] = matrix[i][j];
                if (i > 0) {
                    dp[i][j] += dp[i - 1][j];
                }

                if (j > 0) {
                    dp[i][j] += dp[i][j - 1];
                }

                if (i > 0 && j > 0) {
                    dp[i][j] -= dp[i - 1][j - 1];
                }
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        int res = 0;
        res = dp[row2][col2];

        if (row1 > 0) {
            res -= dp[row1 - 1][col2];
        }

        if (col1 > 0) {
            res -= dp[row2][col1 - 1];
        }

        if (row1 > 0 && col1 > 0 ) {
            res += dp[row1 - 1][col1 - 1];
        }

        return res;
    }
};

/**
 * Your NumMatrix object will be instantiated and called as such:
 * NumMatrix obj = new NumMatrix(matrix);
 * int param_1 = obj.sumRegion(row1,col1,row2,col2);
 */
```

!!! note
    This version is a little complex. However, the way it was written reflects a
    very important practice when operating on a 2d array, that is: to check the
    validation of the array.

### Range Sum Query 2D - Mutable

Binary Indexed Tree solution

We use 2D version of Binary Index Tree. Some of the explaination can be found at
[Topcoder tutorial](https://www.topcoder.com/community/competitive-programming/tutorials/binary-indexed-trees/)

=== "C++ BIT solution"

    ```c++
    class NumMatrix {
    private:
        vector<vector<int> > nums;
        vector<vector<int> > tree;
        int m;
        int n;

    public:
        NumMatrix(vector<vector<int>> matrix) {
            if (matrix.size() == 0 || matrix[0].size() == 0)
                return;
            m = matrix.size();
            n = matrix[0].size();

            tree.resize(m + 1, vector<int>(n + 1, 0));
            nums.resize(m, vector<int>(n, 0));

            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    update(i, j, matrix[i][j]);
                }
            }
        }

        void update(int row, int col, int val) {
            if (m == 0 || n == 0) return;
            int diff = val - nums[row][col];
            nums[row][col] = val;
            for (int i = row + 1; i <= m; i += i & (-i)) {
                for (int j = col + 1; j <= n; j += j & (-j)) {
                    tree[i][j] += diff;
                }
            }
        }

        int sumRegion(int row1, int col1, int row2, int col2) {
            if (m == 0 || n == 0)
                return 0;

            return getSum(row2 + 1, col2 + 1) - getSum(row1, col2 + 1) - getSum(row2 + 1, col1) + getSum(row1, col1);
        }

        int getSum(int row, int col) {
            int sum = 0;
            for (int i = row; i > 0; i -= i & (-i)) {
                for (int j = col; j > 0; j -= j & (-j)) {
                    sum += tree[i][j];
                }
            }

            return sum;
        }
    };

    /**
    * Your NumMatrix object will be instantiated and called as such:
    * NumMatrix obj = new NumMatrix(matrix);
    * obj.update(row,col,val);
    * int param_2 = obj.sumRegion(row1,col1,row2,col2);
    */
    ```

### Count of Range Sum

Solution 1 Merge sort using `inplace_merge()`

1. The core is to figure out how to calculate the result while merging. It is based
   on the fact that the left half and right half are all sorted. Using the ordering
   information we are able to locate two points in the right half `j` and `k`,
   between which will fulfill the requirement.
2. Several important points need to be made. 1) calculation of prefix sum of the
   array. The length is `n + 1` not `n`? 2) the range passed to the merge
   subroutine are open-end `[start, end)`.
3. The base case of the subrouine. It return zero becuase the case has been
   counted in the for loop, we don't need to count it again. Not because the
   base case is `0`.

```c++
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<long> sums(n + 1, 0);

        for(int i = 0; i< n; i++)
            sums[i + 1] = sums[i] + nums[i];

        return mergeSort(sums, 0, n + 1, lower, upper);
    }

    int mergeSort(vector<long>& sums, int start, int end, int lower, int upper) {
        if(end - start <= 1) return 0; // note the meaning of this 0.

        int mid = start + (end - start) / 2;
        count = mergeSort(sums, start, mid, lower, upper)
                + mergeSort(sums, mid, end, lower, upper);

        int m = mid, n = mid, count = 0;
        for(int i = start; i < mid; i++) {
            while(m < end && sums[m] - sums[i] < lower) m++;
            while(n < end && sums[n] - sums[i] <= upper) n++;
            count += n - m;
        }

        inplace_merge(sums.begin() + start, sums.begin() + mid, sums.begin() + end);

        return count;
    }
};
```

Solution 2 Merge sort using tmp buffer cache

1. Here is how the count is making sense.

```text
       |--------------|-------------------|
sums:  |start         |mid                |end
       |---|----------|------|------|-----|
           i                 j      k
Because sums[j] - sums[i] >= lower, and sums[k] - sums[i] > upper,
So for the subarray start with i, ending index in [j, k), the range
sum is in [lower, upper]. Notice k should not be included.
```

```c++
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        long sums[n + 1] = {0};

        if (n == 0)
            return 0;

        for (int i = 0; i < n; i++) {
            sums[i + 1] = sums[i] + nums[i];
        }

        /* n + 1 is the one pass the last element of sums */
        return countByMergeSort(sums, 0, n + 1, lower, upper);
    }

    /* This function will return sorted array sums[start], ... sums[end - 1] */
    int countByMergeSort(long sums[], int start, int end, int lower, int upper) {
        if (end - start <= 1) return 0;

        int mid = start + (end - start) / 2;
        int count = countByMergeSort(sums, start, mid, lower, upper)
                    + countByMergeSort(sums, mid, end, lower, upper);

        long cache[end - start] = {0};

        int j = mid, k = mid, t = mid;
        for (int i = start, r = 0; i < mid; ++i, ++r) {
            while (k < end && sums[k] - sums[i] < lower) k++;
            while (j < end && sums[j] - sums[i] <= upper) j++;

            count += j - k; /* calculate the result */

            /* Merge left and right to get sorted array {sums[start], .. sums[end - 1]}.
             * Because left part of sums[start] to sums[mid] are already sorted,
             * use cache here to merge prefix of the right part: sum[mid] to sums[t]
             * with left part upto sums[i] for all i = {start, mid - 1}. */
            while (t < end && sums[t] < sums[i])
                cache[r++] = sums[t++];

            cache[r] = sums[i];

        } /* after this for loop, cache will have partially sorted array
           * cache = sums_left = {sums[start], ... sums[t - 1]} element
           * of which will be in their final sorted positions.
           * array sums_right = {sums[t], sums[end - 1]} is also
           * in their final sorted positions. */

        /* Since the sums_left is sorted, it have size of t - start,
         * here we copy exactly t - start element from cache to sums.
         */
        for (int i = start; i < t; i++)
            sums[i] = cache[i - start];

        return count;
    }
};
```

Solution 3 BST

Solution 4 BIT

### Maximum Sum of Two Non-Overlapping Subarrays

=== "Brute Force Iterate"

    ```c++
    class Solution {
    public:
        int maxSumTwoNoOverlap(vector<int>& A, int L, int M) {
            int n = A.size();
            if (L == 0 || M == 0) {
                return 0;
            }

            vector<int> preSum(n, 0);
            preSum[0] = A[0];
            for (int i = 1; i < n; i++) {
                preSum[i] = preSum[i - 1] + A[i];
            }

            int res = 0;
            // iterate the L using index i
            for (int i = 0; i < n - L + 1; i++) {
                int Lsum = 0;
                if (i == 0) {
                    Lsum = preSum[i + L - 1];
                } else {
                    Lsum = preSum[i + L - 1] - preSum[i - 1];
                }

                int Msum = 0;
                // iterate the left M array using index j
                for (int j = 0; j < i - M; j++) {
                    int tmp = 0;
                    if (j == 0) {
                        tmp = preSum[j + M - 1];
                    } else {
                        tmp = preSum[j + M - 1] - preSum[j - 1];
                    }
                    Msum = max(Msum, tmp);
                }
                // iterate the right M array using index j
                for (int j = i + L; j < n - M + 1; j++) {
                    Msum = max(Msum, preSum[j + M - 1] - preSum[j - 1]);
                }

                res = max(res, Msum + Lsum);
            }

            return res;
        }
    };
    ```

=== "One pass"

    ```c++
    class Solution {
    public:
        int maxSumTwoNoOverlap(vector<int>& A, int L, int M) {
            int n = A.size();
            if (L == 0 || M == 0) {
                return 0;
            }

            vector<int> preSum(n, 0);
            preSum[0] = A[0];
            for (int i = 1; i < n; i++) {
                preSum[i] = preSum[i - 1] + A[i];
            }

            int res = INT_MIN;
            int Lmax = INT_MIN;
            int Mmax = INT_MIN;
            //
            for (int i = L + M; i <= n; i++) {
                // L is front, M is back
                if (i == L + M) {
                    Lmax = preSum[L - 1];
                } else {
                    Lmax = max(Lmax, preSum[i - M - 1] - preSum[i - L - M - 1]);
                }
                // M is front, L is back
                if (i == L + M) {
                    Mmax = preSum[M - 1];
                } else {
                    Mmax = max(Mmax, preSum(i - L - 1) - preSum[i - M - L - 1]);
                }

                res = max({res,
                        Lmax + preSum[i - 1] - preSum[i - M - 1],
                        Mmax + preSum[i - 1] - preSum[i - L - 1]})
            }
            return res;
        }
    };
    ```

=== "DP solution"

```c++
//TODO
```

### Maximum Sum of 3 Non-Overlapping Subarrays

### [Longest Substring Without Repeating Characters](../../string/notes/#longest-substring-without-repeating-characters)

### [Longest Substring with At Most Two Distinct Characters](../../string/notes/#longest-substring-with-at-most-two-distinct-characters)

### [Longest Substring with At Most K Distinct Characters](../../string/notes/#longest-substring-with-at-most-k-distinct-characters)

### Subarrays with K Different Integers

* If you use sliding window to solve this problem, there are lots of corner cases.
* for example: `[1, 1, 2, 1, 2, 1, 3, 1], K = 2`, how can you ensure you count all the
  subarray?
* Remember this trick that you can use the `atMostKDistinct(A, K) - atMostKDistinct(A, K - 1)`.

```c++
class Solution {
public:
    int subarraysWithKDistinct(vector<int>& A, int K) {
        return subarrayWithAtMostKDistinct(A, K)
                 - subarrayWithAtMostKDistinct(A, K - 1);
    }
    int subarrayWithAtMostKDistinct(vector<int>& A, int K) {
        unordered_map<int, int> count;

        int i = 0;
        int res = 0;
        for (int j = 0; j < A.size(); ++j) {
            if (!count[A[j]]++) K--;
            while (k < 0) {
                if (!--count[A[j]]) K++;
                i++;
            }
            res += j - i + 1;
        }

        return res;
    }
};
```

### Number of Substrings Containing All Three Characters

### Count Number of Nice Subarrays

### Replace the Substring for Balanced String

### Binary Subarrays With Sum

### Fruit Into Baskets

### Shortest Subarray with Sum at Least K

### [Minimum Size Subarray Sum](#minimum-size-subarray-sum)

### Substring with Concatenation of All Words

### Max Consecutive Ones II

### Max Consecutive Ones III

## Category 4 K Sum problems

### Two Sum

### Two Sum II - Input array is sorted

### Two Sum III - Data structure design

### Two Sum IV - Input is a BST

### 3Sum

### 3Sum Closest

### 3Sum Smaller

### 4Sum

### 4Sum II

### K Sum

### Target Sum

## Cagegory 5 2D arry (matrix, grid) problems

### Perfect Rectangle

### Trapping Rain Water

### Trapping Rain Water II

### Container With Most Water

### Largest Rectangle in Histogram

### Maximal Rectangle

### Maximal Square

### The Skyline Problem

### Smallest Rectangle Enclosing Black Pixels

### Rectangle Area

### [Max Sum of Rectangle No Larger Than K](#max-sum-of-rectangle-no-larger-than-k)

## Category 6 stock buying problems

[Most consistent ways of dealing with the series of stock problems](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/discuss/108870/Most-consistent-ways-of-dealing-with-the-series-of-stock-problems)

### 121. Best Time to Buy and Sell Stock

Solution 1 O(n) one pass to find the minimum and in the meantime, find the max profit.

```c++
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        int n = prices.size();
        if (n == 0) return 0;

        int res = 0, low = prices[0];

        for (int i = 1; i < n; i++) {
            res = max(res, prices[i] - low);
            low = min(low, prices[i]);
        }

        return res;
    }
};
```

### 122. Best Time to Buy and Sell Stock II

Solution 1 Greedy since you can buy as many times as you can

```c++
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        int n = prices.size();
        int res = 0;

        for (int i = 0; i < n - 1; i++) {
            if (prices[i + 1] - prices[i]) {
                res += prices[i + 1] - prices[i];
            }
        }

        return res;
    }
};
```

### 123. Best Time to Buy and Sell Stock III

You can now buy at most twice. how to max the profit.

Solution 1 Dynamic programming

* 5 stages: 1. before buy the first  <-- optimal solution could be at this stage
            2. hold the first
            3. sell the first  <-- or at this stage, only bought once,
            4. hold the second
            5. sell the second <-- or at this stage, bought twice.

```c++
class Solution {
public:
    int maxProfit(vector<int> &A) {
        //1, 3, 5:
        // f[i][j] = max{f[i - 1][j], f[i - 1][j - 1] + A[i - 1] - A[i - 2]}
        // 2, 4:
        // f[i][j] = max{f[i - 1][j] + A[i - 1] - A[i - 2],
        //               f[i - 1][j - 1], f[i - 1][j - 2] + A[i - 1] - A[i - 2]}
        int n = A.size();
        if (n == 0) {
            return 0;
        }

        int f[n+1][6];

        /* init */
        f[0][1] = 0;
        f[0][2] = f[0][3] = f[0][4] = f[0][5] = INT_MIN;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= 5; j += 2) {
                f[i][j] = f[i - 1][j];
                if (i > 1 && j > 1 && f[i - 1][j - 1] != INT_MIN) {
                    f[i][j] = max(f[i][j], f[i - 1][j - 1] + A[i - 1] - A[i - 2]);
                }

            }

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

Solution 2

Use `T[i][k][j]` to represent the maximum profit of first i days if we allow at
most k transactions and the current number of stocks at hand is `j` (`j == 0, 1`
because hold two stocks at the same time is not allowed). So we have:

```text
T[i][2][0] = max(T[i - 1][2][0], T[i - 1][2][1] + prices[i - 1]);
T[i][2][1] = max(T[i - 1][2][1], T[i - 1][1][0] - prices[i - 1]);
T[i][1][0] = max(T[i - 1][1][0], T[i - 1][1][1] + prices[i - 1]);
T[i][1][1] = max(T[i - 1][1][1], T[i - 1][0][0] - prices[i - 1]);
```

Think: How to ensure you fourmular to cover all the possible values?

### 188. Best Time to Buy and Sell Stock IV

### 309. Best Time to Buy and Sell Stock with Cooldown

### 714. Best Time to Buy and Sell Stock with Transaction Fee

### Determine the buy data and sell data of maximum profit (DD 139)