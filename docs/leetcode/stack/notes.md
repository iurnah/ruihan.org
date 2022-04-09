# Stack

## Key problem types

### monotonic stack

* It can be monotonic __increase__ or __decreasing__ based on application.
* The data structure is used to trade off space for time (to achieve O(N) complexity).
* The iteration can be forward and backward, there is no essential difference, but
  can change the way to think of the problem.
* The core is to maintain the invariant and do the work at the right __moment__.
    * The invariant is to keep the element ordered in the stack in strict
      increasing/decreasing order.
    * The work is done at either during the element is popped or after popped.
    * From the [Next Greater Element I] and [Next Greater Element II], we can see
      that no matter the forward or backward iteration, the stack values are strictly
      decreasing.
    * __check whether you need to push the index or the value.__ You can put both
      if needed.
* To simplify the code, we can use sentinel values to eliminate redundant codes.
  Specifically, you can avoid checking null of the stack and directly access the
  stack top value.

### monotonic stack cheat sheet

1. You can iterate the array both forward or backward, forward iteration is simpler.
2. You can push the index to the stack or simply push the array value. Usually, when
   we need to meet index range constrain, we have to push the index to the stack.
3. Be careful about the comparison between the current value and the stack top,
   Make sure choose the correct operators ($>$ or $>=$, $<$ or $<=$).
4. Sometimes you need to look more than the stack top, for example [Trapping Rain Water](#42.-trapping-rain-water).

## Problems

### 42. Trapping Rain Water

* Solution 1 monotonic Stack
* Solution 2 scan the tallest from left and tallest from right, then canculate.

=== "C++ monotonic stack"

    ```c++
    class Solution {
    public:
        int trap(vector<int>& height) {
            int n = height.size();
            int res = 0;
            stack<int> s;

            for (int i = 0; i < n; ++i) {
                while (!s.empty() && height[s.top()] < height[i]) {
                    int curr = height[s.top()];
                    s.pop();

                    int prev = s.empty() ? 0 : height[s.top()];

                    int h = min(prev, height[i]) - curr;

                    int w = s.empty() ? 0 : i - s.top() - 1;

                    res += h * w;
                }

                s.push(i);
            }

            return res;
        }
    };
    ```

=== "C++ Naive"

    ```c++
    class Solution {
    public:
        int trap(vector<int>& height) {
            int n = height.size();
            vector<int> left(n, 0);
            vector<int> right(n, 0);
            if (n < 1)
                return 0;

            left[0] = height[0];
            for (int i = 1; i < n; ++i) {
                left[i] = max(left[i - 1], height[i]);
            }

            right[n - 1] = height[n - 1];
            for (int i = n - 2; i >= 0; --i) {
                right[i] = max(right[i + 1], height[i]);
            }

            int res = 0;
            for (int i = 0; i < n; ++i) {
                int h = min(left[i], right[i]) - height[i];
                res += h > 0 ? h : 0;
            }

            return res;
        }
    };
    ```

### 84. Largest Rectangle in Histogram

* Solution 1 Scan to left and right from each index
* Solution 2 Using monotonous increase stack

=== "C++ naive solution"

    ```c++
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            int n = heights.size();
            vector<int> left(n);
            vector<int> right(n);
            int res = 0;

            for (int i = 0; i < n; ++i) {
                int j = i;
                while (j > 0 && heights[i] <= heights[j - 1]) {
                    j = left[j - 1];
                }
                left[i] = j;

                j = i;
                while (j < n - 1 && heights[i] <= heights[j + 1]) {
                    j = right[j + 1];
                }
                right[i] = j;
            }

            for (int i = 0; i < n; ++i) {
                res = max(res, heights[i] * (right[i] - left[i] + 1));
            }

            return res;
        }
    };
    ```

=== "C++ monotonous stack"

    ```c++
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            int n = heights.size();
            stack<int> s;
            int res = 0;

            for (int i = 0; i < n; ++i) {
                int height = 0;
                while(!s.empty() && heights[i] < heights[s.top()]) {
                    // calculate the area determined by the stack top hist
                    // when pop it off the stack
                    int height = heights[s.top()];
                    s.pop();
                    int width = 0;
                    if (!s.empty()) {
                        width = i - s.top() - 1;
                    } else {
                        width = i;
                    }

                    res = max(res, height * width);
                }

                s.push(i);
            }

            // pop the rest of the elemnt off the stack
            while(!s.empty()) {
                int height = heights[s.top()];
                s.pop();
                int width = 0;
                if (!s.empty()) {
                    width = n - s.top() - 1;
                } else {
                    width = n;
                }

                res = max(res, height * width);
            }

            return res;
        }
    };
    ```

=== "C++ Use monotonic stack with sentinel"

    ```c++
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            int n = heights.size();
            stack<int> s;
            int res = 0;

            vector<int> hist;

            // add sentinel element at begining and end
            heights.insert(heights.begin(), 0);
            heights.push_back(0);
            s.push(0);

            for (int i = 1; i < n + 2; ++i) {
                int height = 0;
                while(heights[s.top()] > heights[i]) {
                    // calculate the area determined by the stack top hist
                    // when pop it off the stack
                    int height = heights[s.top()];
                    s.pop();
                    int width = i - s.top() - 1;
                    res = max(res, height * width);
                }

                s.push(i);
            }

            return res;
        }
    };
    ```

### 85. Maximal Rectangle

Solution 1 use monotonic stack. We can treat it as a 2D version of [84. Largest Rectangle in Histogram]()

=== "C++ monotonic stack"

    ```c++
    class Solution {
    public:
        int getMaxArea(vector<int> v) {
            int n = v.size();
            int res = 0;
            stack<int> s;

            for (int i = 0; i < n; ++i) {
                while (!s.empty() && v[s.top()] > v[i]) {
                    int height = v[s.top()];
                    s.pop();

                    int width = s.empty() ? i : i - s.top() - 1;

                    res = max(res, height * width);
                }

                s.push(i);
            }

            // pop the rest of the elements
            while (!s.empty()) {
                int height = v[s.top()];
                s.pop();

                int width = s.empty() ? n : n - s.top() - 1;

                res = max(res, height * width);
            }

            return res;
        }

        int maximalRectangle(vector<vector<char>>& matrix) {
            int m = matrix.size();
            int n = m == 0 ? 0 : matrix[0].size();

            int res = 0;

            vector<int> v(n, 0);
            // aggregate vertically then canculte area.
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    if (matrix[r][c] == '1') {
                        v[c]++;
                    } else {
                        v[c] = 0;
                    }

                }

                res = max(res, getMaxArea(v));
            }

            return res;
        }
    };
    ```

=== "C++ Monotonic stack use a sentinel at end"

    ```c++ hl_lines="7 9"
    class Solution {
    public:
        int getMaxArea(vector<int> v) {
            int n = v.size();
            int res = 0;
            stack<int> s;
            v.push_back(-1);

            for (int i = 0; i <= n; ++i) {
                while (!s.empty() && v[s.top()] > v[i]) {
                    int height = v[s.top()];
                    s.pop();

                    int width = s.empty() ? i : i - s.top() - 1;

                    res = max(res, height * width);
                }

                s.push(i);
            }

            return res;
        }

        int maximalRectangle(vector<vector<char>>& matrix) {
            int m = matrix.size();
            int n = m == 0 ? 0 : matrix[0].size();

            int res = 0;

            vector<int> v(n, 0);
            // aggregate vertically then canculte area.
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    if (matrix[r][c] == '1') {
                        v[c]++;
                    } else {
                        v[c] = 0;
                    }
                }

                res = max(res, getMaxArea(v));
            }

            return res;
        }
    };
    ```

### 221. Maximal Square

```c++
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size();
        int n = m == 0 ? 0 : matrix[0].size();
        int res = 0;

        vector<int> v(n, 0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == '1') {
                    v[j]++;
                } else {
                    v[j] = 0;
                }
            }

            res = max(res, maxArea(v));
        }

        return res;
    }

    int maxArea(vector<int> v) {
        int n = v.size();
        int res = 0;
        stack<int> s;
        v.insert(v.begin(), 0);
        v.push_back(0);

        for (int i = 0; i < n + 2; ++i) {
            while (!s.empty() && v[s.top()] > v[i]) {
                int h = v[s.top()];
                s.pop();

                int w = s.empty() ? i : i - s.top() - 1;

                int d = min(h, w);

                res = max(res, d * d);
            }
            s.push(i);
        }

        return res;
    }
};
```

### 496. Next Greater Element I

* A naive solution will be to use a map to record the index of an element, then
  iterate the `nums1`, find each element in `nums2`, then scan right to get the
  next greater element.
* To improve the time complexity, we can use binary search to find the next
  greater element
* The best solution will be using a stack to find all the greater element for
  all `nums2` elements and store them in a map, then lookup the elements in
  `num1` from the map.

=== "Python monotonic stack"

    ```python
    class Solution:
        def nextGreaterElement(self, nums1: List[int], nums2: List[int]) -> List[int]:
            stk = []
            d = {x: -1 for x in nums2}

            for num in nums2:
                while stk and stk[-1] < num:
                    d[stk.pop()] = num

                stk.append(num)

            return [d[i] for i in nums1]
    ```

=== "C++ monotonic stack forward"

    ```c++
    class Solution {
    public:
        vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
            unordered_map<int, int> nextG;
            vector<int> res;
            stack<int> s;

            for (int i = 0; i < nums2.size(); ++i) {
                // decreasing stack
                while (!s.empty() && s.top() < nums2[i]) {
                    nextG[s.top()] = nums2[i];
                    s.pop();
                }

                s.push(nums2[i]);
            }

            // clear the stack
            while (!s.empty()) {
                nextG[s.top()] = -1;
                s.pop();
            }

            for (int i = 0; i < nums1.size(); ++i) {
                res.push_back(nextG[nums1[i]]);
            }

            return res;
        }
    };
    ```

=== "C++ monotonic stack backward"

    ```c++
    class Solution {
    public:
        vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
            unordered_map<int, int> nextG;
            stack<int> s;

            for (int i = nums2.size() - 1; i >= 0; --i) {
                while (!s.empty() && s.top() <= nums2[i]) {
                    s.pop();
                }
                // next greater element after ith element
                nextG[nums2[i]] = s.empty() ? -1 : s.top();

                s.push(nums2[i]);
            }

            vector<int> res;
            for (int i = 0; i < nums1.size(); ++i) {
                res.push_back(nextG[nums1[i]]);
            }

            return res;
        }
    };
    ```

### 503. Next Greater Element II

=== "C++ monotonic stack backward"

    ```c++
    class Solution {
    public:
        vector<int> nextGreaterElements(vector<int>& nums) {
            int n = nums.size();
            stack<int> s;
            vector<int> res(n, -1);

            for (int i = 0; i < 2 * n; ++i) {
                while (!s.empty() && nums[i % n] > nums[s.top()]) {
                    res[s.top()] = nums[i % n];
                    s.pop();
                }
                // notice the index is pushed to the stack
                s.push(i % n);
            }

            return res;
        }
    };
    ```

=== "C++ monotonic decreasing stack"

    ```c++
    class Solution {
    public:
        vector<int> nextGreaterElements(vector<int>& nums) {
            int n = nums.size();
            stack<int> s;
            vector<int> res(n, -1);

            for (int i = 2 * n - 1; i > 0; --i) {
                while (!s.empty() && nums[i % n] >= nums[s.top()]) {
                    s.pop();
                }

                res[i % n] = s.empty() ? -1 : nums[s.top()];

                s.push(i % n);
            }

            return res;
        }
    };
    ```

=== "C++ Naive"

    ```c++
    class Solution {
    public:
        vector<int> nextGreaterElements(vector<int>& nums) {
            int n = nums.size();
            vector<int> res;

            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                int flag = 0;
                while (j != i) {
                    if (nums[j] > nums[i]) {
                        res.push_back(nums[j]);
                        flag = 1;
                        break;
                    }
                    j++;

                    j %= n; // circular interate
                }

                if (!flag) {
                    res.push_back(-1);
                }
            }

            return res;
        }
    };
    ```

### 739. Daily Temperatures

Solution 1: Use monotonic stack, index are pushed to the stack for calculate distance

=== "Python monotonic stack"

    ```python
    class Solution:
        def dailyTemperatures(self, temperatures: List[int]) -> List[int]:
            n = len(temperatures)
            stk = []
            res = [0] * n

            for i, t in enumerate(temperatures):
                while stk and temperatures[stk[-1]] < t:
                    res[stk[-1]] = i - stk[-1]
                    stk.pop()

                stk.append(i)

            return res
    ```

=== "C++ monotonic stack"

    ```c++
    class Solution {
    public:
        vector<int> dailyTemperatures(vector<int>& T) {
            int n = T.size();
            vector<int> res(n, 0);
            stack<int> s;

            for (int i = 0; i < n; ++i) {
                while (!s.empty() && T[i] > T[s.top()]) {
                    res[s.top()] = i - s.top();
                    s.pop();
                }

                s.push(i);
            }

            return res;
        }
    };
    ```

### 901. Online Stock Span

Solution 1 Monotonic stack

1. Iterate left to right, keep the current result together with the element in
   the stack (pop each element meet the constrain off the stack)
2. The stack have strict decreasing values

=== "C++ monotonic stack"

    ```python
    class StockSpanner:

        def __init__(self):
            self.stk = []

        def next(self, price: int) -> int:
            res = 1
            while self.stk and self.stk[-1][0] <= price:
                res += self.stk.pop()[1]

            self.stk.append((price, res))

            return res

    # Your StockSpanner object will be instantiated and called as such:
    # obj = StockSpanner()
    # param_1 = obj.next(price)
    ```

=== "C++ monotonic stack"

    ```c++
    class StockSpanner {
        stack<pair<int, int>> s;
        int count = 0;
    public:
        StockSpanner() {
        }

        int next(int price) {

            count = 1;

            while (!s.empty() && s.top().first <= price) {
                count += s.top().second;
                s.pop();
            }

            s.push({price, count});

            return count;
        }
    };
    ```

### 907. Sum of Subarray Minimums

* Solution 1 [monotonous increase stack](https://leetcode.com/problems/sum-of-subarray-minimums/discuss/178876/stack-solution-with-very-detailed-explanation-step-by-step)

=== "C++ monotonic stack"

    ```c++
    class Solution {
    public:
        int sumSubarrayMins(vector<int>& A) {
            stack<pair<int, int>> s1, s2;
            vector<int> left(A.size()), right(A.size());

            // distance between next smaller element on the left
            for (int i = 0; i < A.size(); i++) {
                int count = 1;
                // maintains a monotonic increase stack
                while (!s1.empty() && s1.top().first > A[i]) {
                    count += s1.top().second;
                    s1.pop();
                }
                s1.push({A[i], count});
                left[i] = count;
            }

            for (int i = A.size() - 1; i >= 0; i--) {
                int count = 1;
                while (!s2.empty() && s2.top().first >= A[i]) {
                    count += s2.top().second;
                    s2.pop();
                }
                s2.push({A[i], count});
                right[i] = count;
            }

            int res = 0;
            int MOD = 1e9 + 7;
            for (int i = 0; i < A.size(); i++) {
                res = (res + A[i] * left[i] * right[i]) % MOD;
            }

            return res;
        }
    };
    ```

=== "Java monotonic stack"

    ```c++
    class Solution {
    public:
        int sumSubarrayMins(vector<int>& arr) {
            int n = arr.size();
            stack<int> s;
            int res = 0;
            int MOD = 1e9 + 7;

            for (int i = 0; i <= n; ++i) {
                while (!s.empty() && (i == n || arr[s.top()] > arr[i])) {
                    int m = s.top();
                    s.pop();

                    int l = s.empty() ? m + 1 : m - s.top();
                    int r = i - m;

                    res += arr[m] * l * r;

                }
                s.push(i);
            }

            return res % MOD;
        }
    };
    ```

=== "Java monotonic stack"

    ```java
    public int sumSubarrayMins(int[] A) {
        long res = 0;
        Deque<Integer> stack = new ArrayDeque<>();

        for(int i = 0; i <= A.length; i++) {
            while(!stack.isEmpty() && (i == A.length || A[stack.peek()] > A[i])) {
                int mid = stack.pop();
                int L = mid - (stack.isEmpty() ? -1 : stack.peek());
                int R = i - mid;
                res += (long) A[mid] * L * R;
            }

            stack.push(i);
        }
        return (int) (res % 1_000_000_007);
    }
    ```

### 1019. Next Greater Node In Linked List

=== "C++ monotonic stack"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode() : val(0), next(nullptr) {}
    *     ListNode(int x) : val(x), next(nullptr) {}
    *     ListNode(int x, ListNode *next) : val(x), next(next) {}
    * };
    */
    class Solution {
    public:
        vector<int> nextLargerNodes(ListNode* head) {
            stack<int> s, idx;
            ListNode *curr = head;
            int len = 0;

            while (curr) {
                len++;
                curr = curr->next;
            }

            int i = 0;
            vector<int> res(len, 0);
            curr = head;
            while (curr != nullptr) {
                int val = curr->val;
                while (!s.empty() && val > s.top()) {
                    res[idx.top()] = val;
                    s.pop(), idx.pop();
                }

                s.push(val), idx.push(i);

                curr = curr->next;
                i++;
            }

            return res;
        }
    };
    ```

### 402. Remove K Digits

Solution 1 using monotonic stack. Think of how to remove the lagest digit from
left to right? Notice the speical case: __10200__.

```c++
class Solution {
public:
    string removeKdigits(string num, int k) {
        int n = num.length();
        int m = n - k;
        string res = "";

        for (int i = 0; i < n; ++i) {
            while (k && !res.empty() && res.back() > num[i]) {
                res.pop_back();
                --k;
            }
            res.push_back(num[i]);
        }
        res.resize(m);

        while (!res.empty() && res[0] == '0') {
            res.erase(res.begin());
        }

        return res.empty() ? "0" : res;
    }
};
```

### 1130. Minimum Cost Tree From Leaf Values

### 1425. Constrained Subsequence Sum

### 1776. Car Fleet II

Solution I Monotonic stack

* Notice in this problem the invariant is much more complicated. Beside the
  order (so that the cars can collide), you also have to make sure the next car
  haven't collide ealier to some lower speed, you have to look at the global
  optimal solution. For example, car_1, car_2, car_3, when you look at only car_1
  and car_2, you may find that they are going to collide at t1, but car_2 and
  car_3 could collide much erlier, the correct solution could be smaller than t1.

=== "C++ monotonic stack"

    ```c++ hl_lines="18"
    class Solution {
    public:
        vector<double> getCollisionTimes(vector<vector<int>>& cars) {
            auto collideT = [&](int i, int j) -> double {
                // cars[i][0] < cars[j][0], cars[i][1] > cars[j][0]
                return static_cast<double>(cars[j][0] - cars[i][0]) / (cars[i][1] - cars[j][1]);
            };

            int n = cars.size();
            stack<int> s;
            vector<double> res(n, -1);

            for (int i = n - 1; i >= 0; --i) {
                // not only you have to be fast to catch next car,
                // but also the next car have not collided before, if it collided before
                // you have to look for the slower cars (which could be collided much ealier)
                while (!s.empty() && (cars[i][1] <= cars[s.top()][1] ||
                                    (s.size() > 1 && collideT(i, s.top()) > res[s.top()]))) {
                    s.pop();
                }
                res[i] = s.empty() ? -1 : collideT(i, s.top());
                s.push(i);
            }

            return res;
        }
    };
    ```

=== "C++ Brute force"

    ```c++
    class Solution {
    public:
        vector<double> getCollisionTimes(vector<vector<int>>& cars) {
            int n = cars.size();
            vector<double> ans(n, -1.0);

            for (int i = n - 1; i >= 0; --i) {
                int p = cars[i][0];
                int v = cars[i][1];

                for (int j = i - 1; j >= 0; --j) {
                    int p_j = cars[j][0];
                    int v_j = cars[j][1];
                    int dp = p - p_j;
                    int dv = v_j - v;
                    if (dv <= 0) break;
                    double t = (double) dp / dv;
                    if (ans[j] < 0 || t < ans[j])
                        ans[j] = t;
                    else
                        break;

                }
            }

            return ans;
        }
    };
    ```

### 2104. Sum of Subarray Ranges

Solution 1: Brute force
Solution 2: Monotonic stack

1. using a sentinel to simplify the code (reducing null checks)
2. use the appropriate values for the sentinel based on the goal

=== "Python monotonic stack"

    ```python
    class Solution:
        def subArrayRanges(self, nums: List[int]) -> int:
            res = 0
            stk = [] # push index to this stack
            inf = float('inf')
            A = [-inf] + nums + [-inf]
            # count the number of subarray include the minimum (A[j])
            # range is max - min, we aggregate min as negative numbers
            for i, n in enumerate(A):
                while stk and A[stk[-1]] > n:
                    j = stk.pop()
                    k = stk[-1]
                    res -= A[j] * (j - k) * (i - j)

                stk.append(i)

            stk.clear()
            A = [inf] + nums + [inf]

            # count the number of subarray include the maximum (A[j])
            for i, n in enumerate(A):
                while stk and A[stk[-1]] < n:
                    j = stk.pop()
                    k = stk[-1]
                    res += A[j] * (j - k) * (i - j)

                stk.append(i)

            return res
    ```

=== "Python brute force"

    ```python
    class Solution:
        def subArrayRanges(self, nums: List[int]) -> int:
            res = 0
            n = len(nums)

            for i in range(n):
                mx, mn = nums[i], nums[i]
                for j in range(n)[i:]:
                    mx = max(mx, nums[j])
                    mn = min(mn, nums[j])
                    res += mx - mn

            return res
    ```