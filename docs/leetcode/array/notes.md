# Array

## Category 1 subarray problems

### Max Sum of Subarry No Larger Than K (Quora, geeksforgeeks)

1. This problem in geeksforgeeks as "Maximum sum subarray having sum less than or equal to given sum".
2. It has been discussed here.
3. This problem is the basis to solve the problem 363. Max Sum of Rectangle No Larger Than K.

Solution 1 using prefix sum and set

1. calculate prefix and using a set to store individual prefix sum, (`vector`
   also works). In each iteration, we lookup the value `preSum - k` in the set.
2. Notice we can use binary search to find the smallest element that `>= preSum - k`.
   We can use `lower_bound` to achieve that. Notice if it is asking the sum less
   than k we have to use `upper_bound`

```C++ tab=
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

### Maximum subarray size, such that all subarrays of that size have sum less than k (geeksforgeeks)_

### 363. Max Sum of Rectangle No Larger Than K

Solution 1 iterate the wide of the matrix and using prefix sum and set `lower_bound`.

1. From the problem Max Sum of Subarry No Larger Than K, we have to enumerate
   the width of the sub-matrix and sum up all row elements and get an array
   of length `m`, `m` is the number of rows of the matrix. Then apply the method.

```C++ tab=
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

Solution 2 using merge sort

1. The idea is similar that solution 1. Instead of calculate `preSum` on the
   fly, we finish calculation and pass it to a `mergeSort` routine.
2. The use `mergeSort` here is to find the `A[j] - A[i] <= k` efficiently,
   `O(nlogn)`.

```C++ tab=
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
            /* sums[j - 1] - sums[i] <= k, make sure j - 1 is still in right side */
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

### Count Inversion (course assignment)

Solution 1 using merge sort

1. This problem is the basic of using merge sort to solve lots of hard problems.

```C++ tab=
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

### 315. Count of Smaller Numbers After Self

Solution 1 Merge sort

1. One important point to remember is you have to create pairs out of the array
   element and its index, because during merge sort, when we count each value,
   we don't know where to put those count values in the result vector.
2. The second merge solutions run much faster than the first one.

```C++ tab="C++ Merge sort"
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

```C++ tab="C++ more efficient  Merge sort"
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

```C++ tab="C++ BST"
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

### 327. Count of Range Sum

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

```C++ tab=
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

```
       |--------------|-------------------|
sums:  |start         |mid                |end
       |---|----------|------|------|-----|
           i                 j      k
Because sums[j] - sums[i] >= lower, and sums[k] - sums[i] > upper,
So for the subarray start with i, ending index in [j, k), the range
sum is in [lower, upper]. Notice k should not be included.
```

```C++ tab=
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

### 493. Reverse Pairs

Solution 1 Merge sort using `inplace_merge()`

1. Notice the problem is very similar to count inverse.

```C++ tab=
class Solution {
private:
    int merge_and_count(vector<int>& nums, int start, int end) {
        if (end - start <= 1) return 0;

        int count = 0;
        int mid = start + (end - start) / 2;
        count = merge_and_count(nums, start, mid)
                + merge_and_count(nums, mid, end);

        for (int i = start, j = mid; i < mid; i++) {
            while (j < end && nums[i] > 2L * nums[j]) j++;
            count += j - mid;
        }

        inplace_merge(nums.begin() + start, nums.begin() + mid, nums.begin() + end);

        return count;
    }

public:
    int reversePairs(vector<int>& nums) {
        int n = nums.size();

        return merge_and_count(nums, 0, n);
    }
};
```

Solution 2 Merge sort using tmp buffer cache

Solution 3 BST

Solution 4 BIT

## Category 2 problems buy stock problems

[Most consistent ways of dealing with the series of stock problems](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/discuss/108870/Most-consistent-ways-of-dealing-with-the-series-of-stock-problems)

### 121. Best Time to Buy and Sell Stock

Solution 1 O(n) one pass to find the minimum and in the meantime, find the max profit.

```C++ tab=
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

```C++ tab=
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

- 5 stages: 1. before buy the first  <-- optimal solution could be at this stage
            2. hold the first
            3. sell the first  <-- or at this stage, only bought once,
            4. hold the second
            5. sell the second <-- or at this stage, bought twice.

```C++ tab=
class Solution {
public:
    int maxProfit(vector<int> &A) {
        //1, 3, 5:
        // f[i][j] = max{f[i - 1][j], f[i - 1][j - 1] + A[i - 1] - A[i - 2]}
        // 2, 4:
        // f[i][j] = max{f[i - 1][j] + A[i - 1] - A[i - 2], f[i - 1][j - 1], f[i - 1][j - 2] + A[i - 1] - A[i - 2]}
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

- use `T[i][k][j]` to represent the maximum profit of first i days if we allow
  at most k transactions and the current number of stocks at hand is j (j == 0, 1,
  because hold two stock at the same time is not allowed). So we have:
  `T[i][2][0] = max(T[i - 1][2][0], T[i - 1][2][1] + prices[i - 1]);`
  `T[i][2][1] = max(T[i - 1][2][1], T[i - 1][1][0] - prices[i - 1]);`
  `T[i][1][0] = max(T[i - 1][1][0], T[i - 1][1][1] + prices[i - 1]);`
  `T[i][1][1] = max(T[i - 1][1][1], T[i - 1][0][0] - prices[i - 1]);`

Think: How to ensure you fourmular cover all the possible values?

### 188. Best Time to Buy and Sell Stock IV

### 309. Best Time to Buy and Sell Stock with Cooldown

### 714. Best Time to Buy and Sell Stock with Transaction Fee

### xxx. Determine the buy data and sell data of maximum profit (DD 139)
