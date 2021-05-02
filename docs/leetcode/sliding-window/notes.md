# Sliding Window

## Problem types

### Monotonic deque to acheive O(n)

* If we need to find the max/min from a sliding window. The first idea would be
  use ordered container to store them and then access the max/min. But it usually
  requires $O(nlogn)$ or even worse. While we use deque in sliding window problem
  we can achieve runtime complexity of $O(n)$ e.g. [1425. Constrained Subsequence Sum](),
  [1438. Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit]()
* The general idea is to use deque to keep only the necessary information. During
  each iteration, we use the window size and the order info to keep the invariant:
  1. All elements that are not possible to become a solution are removed from queue;
  2. All elements in the queue form a valid window size, so that we can calculate
  the solution.
* Another caveat is when we maintain the invariant, should we use `while` or use
  `if`? This is realative easy to determine in practice, you just need to verify
  whether the invariant is maintained correctly. Use `if` for one operation and
  `while` for multiple operations.

## Problems

### 209. Minimum Size Subarray Sum

* Solution 1: Using two pointers

```c++
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int res = n + 1;

        int left = 0;
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += nums[i];
            while (sum >= target) {
                res = min(res, i - left + 1);
                sum -= nums[left];
                left++;
            }
        }

        return res == n + 1 ? 0 : res;
    }
};
```

### 239. Sliding Window Maximum

* Solution 1 Use Deque

```c++
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> res;
        deque<int> deq;

        for (int i = 0; i < n; i++) {
            // keep the non increasing element in the deque
            while (!deq.empty() && nums[i] >= nums[deq.back()]) {
                deq.pop_back();
            }
            deq.push_back(i);
            // i reach to the size of the window, add result
            if (i >= k - 1) res.push_back(nums[deq.front()]);
            // using if (not while) because add one element to the deque
            if (deq.front() <= i - k + 1) deq.pop_front(); // to ensure the window size is k
        }

        return res;
    }
};
```

### 862. Shortest Subarray with Sum at Least K

### 904. Fruit Into Baskets

### 992. Subarrays with K Different Integers

### 930. Binary Subarrays With Sum

### 1004. Max Consecutive Ones III

### 1234. Replace the Substring for Balanced String

### 1248. Count Number of Nice Subarrays

### 1358. Number of Substrings Containing All Three Characters

### 1425. Constrained Subsequence Sum

* Solution 1 Use ordered data structure to keep the min and max.
* Solution 2 Use deque to optimize into linear complexity.

=== "C++ multiset O(nlogn)"

    ```c++
    class Solution {
    public:
        int constrainedSubsetSum(vector<int>& nums, int k) {
            int n = nums.size();

            multiset<int> s{INT_MIN};
            int res = INT_MIN;
            vector<int> dp(n);

            for (int i = 0; i < n; ++i) {
                if (i > k)
                    // note here j - i <= k, len <= k + 1, i - len is
                    // the first legitimate element before ith element
                    s.erase(s.equal_range(dp[i - k - 1]).first);

                dp[i] = max(*s.rbegin(), 0) + nums[i];
                s.insert(dp[i]);
                res = max(res, dp[i]);
            }

            return res;
        }
    };
    ```

=== "C++ monotonic deque O(n)"

    ```c++
    class Solution {
    public:
        int constrainedSubsetSum(vector<int>& nums, int k) {
            int n = nums.size();
            vector<int> dp(n);
            deque<int> q;
            int res = INT_MIN;

            for (int i = 0; i < n; ++i) {
                if (i > k && q.front() == i - k - 1) {
                    q.pop_front();
                }

                dp[i] = (q.empty() ? 0 : max(dp[q.front()], 0)) + nums[i];
                // maintain invariant in the sliding window
                while (!q.empty() && dp[i] >= dp[q.back()])
                    q.pop_back();

                q.push_back(i);
                res = max(res, dp[i]);
            }

            return res;
        }
    };
    ```

### 1438. Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit

* Solution 1 Use ordered data structure to keep the min and max.
* Solution 2 Use deque to optimize into linear complexity.

=== "C++ multiset O(nlogn)"

    ```c++
    class Solution {
    public:
        int longestSubarray(vector<int>& nums, int limit) {
            multiset<int> mset;
            int left = 0;
            int res = 0;

            for (int i = 0; i < nums.size(); ++i) {
                mset.insert(nums[i]);
                // maintain the invariant
                while (*mset.rbegin() - *mset.begin() > limit) {
                    mset.erase(mset.equal_range(nums[left++]).first);
                }
                res = max(res, i - left + 1);
            }

            return res;
        }
    };
    ```

=== "C++ deque O(n)"

    ```c++
    class Solution {
    public:
        int longestSubarray(vector<int>& nums, int limit) {
            deque<int> min_deq;
            deque<int> max_deq;
            int left = 0;
            int res = 0;
            // O(n), enque once, deque once.
            for (int i = 0; i < nums.size(); ++i) {
                // invariant: only keep the min in the deque
                while (!min_deq.empty() && nums[i] < min_deq.back())
                    min_deq.pop_back();
                min_deq.push_back(nums[i]);
                // invariant: only keep the max in the deque
                while (!max_deq.empty() && nums[i] > max_deq.back())
                    max_deq.pop_back();
                max_deq.push_back(nums[i]);
                // invariant: move the left point to meet the constrain
                while (max_deq.front() - min_deq.front() > limit) {
                    if (max_deq.front() == nums[left]) max_deq.pop_front();
                    if (min_deq.front() == nums[left]) min_deq.pop_front();
                    left++;
                }

                // up date the result
                res = max(res, i - left + 1);
            }

            return res;
        }
    };
    ```