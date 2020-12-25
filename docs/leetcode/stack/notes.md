# Stack

## Key problem types

* [monotonous increase stack](https://leetcode.com/problems/sum-of-subarray-minimums/discuss/178876/stack-solution-with-very-detailed-explanation-step-by-step)

## Problems

### 496. Next Greater Element I

* A naive solution will be to use a map to record the index of an element, then
  iterate the `nums1`, find each element in `nums2`, then scan right to get the
  next greater element.
* The best solution will be using a stack to find all the greater element for
  all `nums2` elements and store them in a map, then lookup the elements in
  `num1` from the map.

=== "C++ stack"

```c++
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
        unordered_map<int, int> m;
        stack<int> s;
        vector<int> res;
        for (auto num : nums) {
            while (!s.empty() && num > s.top()) {
                m[s.top()] = num;
                s.pop();
            }
            s.push(num);
        }

        for (int i = 0; i < findNums.size(); ++i) {
            if (m.count(findNums[i]) > 0)
                res.push_back(m[findNums[i]]);
            else
                res.push_back(-1);
        }

        return res;
    }
};
```

### 503. Next Greater Element II

=== "c++ Naive"

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

=== "C++ stack"

    ```c++
    class Solution {
    public:
        vector<int> nextGreaterElements(vector<int>& nums) {
            int n = nums.size();
            stack<int> s;
            vector<int> res(nums.size(), -1);
            for (int i = 0; i < 2 * n; ++i) {
                while (!s.empty() && nums[i % n] > nums[s.top()]) {
                    res[s.top()] = nums[i % n];
                    s.pop();
                }

                if (i < n) s.push(i);
            }

            return res;
        }
    };
    ```

### 907. Sum of Subarray Minimums

    ```c++

    ```