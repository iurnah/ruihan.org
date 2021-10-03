# Backtracking

## Introduction

Backtracking algorithm can be used to generate all the subsets of a given set,
all the permutation of a given sequence, and all the combinations of k elements
from a given set with n elements. The algorithms are very similar but differ in
some unique property of each problem.

## Subnets

* How many subsets there are for a set with n elements? ($2^n$)
* How to generate all the subsets if there are NO duplicates in the set?
  (See problem [Subsets](./#subsets))
* How to generate all the UNIQUE subsets if there are duplicates in the set?
  (See problem [Subsets II](./#subsets-ii))
* What are the application of subset?

## Permutation

* How many permutations (number of ordering) of a sequence with n elements? ($n!$)
* How to generate all the permutations if there are NO duplicates in the set?
  (See problem [Permutations](./#permutations))
* How to generate all the UNIQUE permutations if there are duplicates in the set?
  (See problem [Permutations II](./#permutations-ii))
* What are the applications of permutation? (upper bound of sorting algorithms)

## Combination

* How many combinations of k elements from a set of n elements? ($\binom{n}{k}$)
* How to derive the formula $\binom{n}{k} = \frac{n!}{k! (n - k)!}$?
    1. Choose k elements from the set one after another without putting back:
       $n (n - 1) \dots (n - k + 1) = \frac{n!}{(n - k)!}$
    2. Calculate the number of ways to choose k elements as $\binom{n}{k}$, then
       order the k elements: $\binom{n}{k} \times k!$
    3. Equalize the two give us: $\binom{n}{k} = \frac{n!}{k! (n - k)!}$
* What’s the relation between the subset and combination?
* For `k` from `0` to `n`, get all the combinations, all those combinations will
  form the powerset of the original set of n elements. We have $\sum_{k = 0}^{n}\binom{n}{k} = \binom{n}{0} + \binom{n}{1}, \dots, \binom{n}{n} = 2^n\text{(number of all set)}$
* How to generate all the combination if there are NO duplicates in the set?
  (See problem [Combinations](./#combinations))
* How to generate all the UNIQUE combinations if there are duplicates in the set?
  (See problem [Combination Sum](./#combination-sum), [Combination Sum II](./#combination-sum-ii),
  [Combination Sum III](./#combination-sum-iii), [Backpack VI](./#backpack-vi))
* What are the applications of combination? (calculate the upper bound of sorting algorithms)

## Partition

* How many ways can we partition a set of `n` elements into `r` groups, with the
  `i`-th group have $n_i$ elements. ($\frac{n!}{n_1! n_2!, \dots, n_r!}$)
    1. We will look at the problem this way: note the numbfer of different ways
       of partitioning as $C$ , each partition have $n_1, n_2, \dots, n_r$ elements.
       We align $n_1, n_2, \dots, n_r$ together to form a `n` elements sequence
       and there are $n!$ of such sequences. Remeber for each partition, there
       are $n_1!$ different sequences. So that we have so we have $C \cdot n_1! \cdot n_2! \dots n_r! = n!$,
       and $\frac{n!}{n_1! n_2!, \dots, n_r!}$
* When the `r = 2`, the partition problem essentially becomes a combination problem.
* What are the applications of partition? (calculate the upper bound of sorting algorithms)

## Summary

**Subset**, **permutation**, and **combination** problems can be solved using a
single code template. Other problems that are involving multiple steps, and asking
for multiple eligible results that fulfill certain criteria, could also use this
template. The subtlety arise when deal with replicates element in the problems.

There are some general principles that I summarized for solving it.

1. The recursive helper function prototype:

```C++
helper(vector<int> nums, int n, [int cur], vector<int> &subset, vector<vector<int>> &result);
```

1. The "*select action*" could be done through update the index `int cur` for
   each level of recursive call.
2. Each `for` iteration (invoke helper function) is to "*select*" an element in
   this level, it recursively adding more elements to the temporary result.
3. To remove duplicate result if duplicate element presented, we first need to
   be clear about where the duplicate results come from. e.g. In generating the
   power subsets, the duplicated results due to repeatedly selecting the same
   element from different original position. (consecutive positions, since it is
   sorted first). Therefore, the idea to avoid duplicate results is to ***not***
   select the duplicate elements for a second time to form the same pattern, we
   use the following template check statement to achieve this.

```c++
if (i != cur && nums[i] == nums[i - 1]){
        continue;
}
```

!!! note "4 elements of backtracking"
    1. What is the iteration? (the for loop, the same level of node in the tree)
    2. What is the recursion? (what index will be advanced?)
    3. What is counted in the result? (what should be pushed back?)
    4. When is the result returned? (the cutting condition)

!!! note "traceability of backtracking"
    By using recursion and iteration together in this backtrack technique, we
    can imagine the problem as growing a recursive tree.

    - To grow the node at the same level, an index in a for loop can do the job.
    - To grow a child of a node, a recursive function should be called.
      More specifically, we can advance the index by passing it into the recursive
      function. This index increment let the tree grow vertically.

## Problems

### Remove Invalid Parenthesis

Decide what to search
Relation with BFS

### Subsets

=== "C++"

    ```c++
    class Solution {
    public:
        vector<vector<int>> subsets(vector<int>& nums) {
            int n = nums.size();
            vector<vector<int> > results;
            vector<int> subset;

            helper (nums, n, 0, subset, results);

            return results;
        }

        /* helper to get the permutation from curr to n - 1, total is n - curr */
        void helper (vector<int> nums, int n, int curr,
                    vector<int>& subset, vector<vector<int> >& results) {

            results.push_back(subset);

            for (int i = curr; i < n; i++) {
                subset.push_back(nums[i]);
                helper(nums, n, i + 1, subset, results);
                subset.pop_back();
            }
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def subsets(self, nums):
            """
            :type nums: List[int]
            :rtype: List[List[int]]
            """
            n = len(nums)
            results = []

            self.helper(nums, n, 0, [], results)

            return results

        def helper(self, nums, n, curr, currSubset, results):

            results.append(list(currSubset))

            for i in range(curr, n):
                currSubset.append(nums[i])
                self.helper(nums, n, i + 1, currSubset, results)
                currSubset.pop()
    ```

### Subsets II

=== "C++"

    ```c++
    class Solution {
    public:
        vector<vector<int>> subsetsWithDup(vector<int>& nums) {
            int n = nums.size();
            vector<vector<int>> results;
            vector<int> subset;

            sort (nums.begin(), nums.end());

            helper (nums, n, 0, subset, results);

            return results;
        }

        void helper(vector<int> nums, int n, int curr,
                vector<int>& subset, vector<vector<int>>& results) {

            results.push_back(subset);

            for (int i = curr; i < n; i++) {
                if (i > curr && nums[i] == nums[i - 1]) {
                    continue;
                }

                subset.push_back(nums[i]);
                helper(nums, n, i + 1, subset, results);
                subset.pop_back();
            }
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def subsetsWithDup(self, nums):
            """
            :type nums: List[int]
            :rtype: List[List[int]]
            """
            n = len(nums)
            results = []
            nums.sort()
            self.helper(nums, n, 0, [], results)

            return results


        def helper(self, nums, n, curr, currSubset, results):
            results.append(list(currSubset))

            for i in range(curr, n):
                if (i > curr) and (nums[i] == nums[i - 1]):
                    continue

                currSubset.append(nums[i])
                self.helper(nums, n, i + 1, currSubset, results)
                currSubset.pop()
    ```

### Permutations

=== "C++ Backtracking"

    ```c++
    class Solution {
    public:
        vector<vector<int> > permute(vector<int> nums) {
            vector<vector<int> > results;
            vector<int> permutation;
            int n = nums.size();
            if (n == 0)
                return results;

            sort(nums.begin(), nums.end());
            helper(nums, n, permutation, results);

            return results;
        }

        void helper(vector<int> nums, int n, vector<int> &permutation,
                vector<vector<int> > &results) {

            if (permutation.size() == n) {
                results.push_back(permutation);
                return;
            }

            for (int i = 0; i < n; i++) {
                vector<int>::iterator it;
                it = find(permutation.begin(), permutation.end(), nums[i]);
                if(it == permutation.end()){
                    permutation.push_back(nums[i]);
                    helper(nums, n, permutation, results);
                    permutation.pop_back();
                }
            }
        }
    };
    ```

=== "Use 'visited' variable"

    ```c++
    class Solution {
    public:
        vector<vector<int>> permute(vector<int>& nums) {
            int n = nums.size();
            vector<vector<int> > results;
            vector<int> permute;
            vector<bool> visited(n, false);

            helper (nums, n, visited, permute, results);

            return results;
        }

        void helper (vector<int> nums, int n, vector<bool> visited,
                    vector<int>& permutation, vector<vector<int> >& results) {

            if (permutation.size() == n) {
                results.push_back(permutation);
                return;
            }

            for (int i = 0; i < n; i++) {
                if (visited[i]) {
                    continue;
                }

                visited[i] = true;
                permutation.push_back(nums[i]);
                helper (nums, n, visited, permutation, results);
                permutation.pop_back();
                visited[i] = false;
            }
        }
    };
    ```

### Permutations II

* The line `27` is very tricky and important. It can be understand as following:
  using `!visited[i - 1]` makes sure when duplicates are selected, the order is
  ascending (index from small to large). However, using `visited[i - 1]` means
  the descending order.
* You cannot using the find to check whether the element is presetend, you have
  to use the visited "bit map" to record the states.

=== "C++"

    ```c++  hl_lines="27"
    class Solution {
    public:
        vector<vector<int>> permuteUnique(vector<int>& nums) {
            int n = nums.size();
            vector<vector<int>> results;
            vector<int> permutation;
            vector<bool> visited(n, 0);
            if (n == 0)
                return results;

            sort(nums.begin(), nums.end());

            helper(nums, n, visited, permutation, results);

            return results;
        }

        void helper(vector<int> nums, int n, vector<bool> visited,
                    vector<int>& permutation, vector<vector<int>>& results) {

            if (permutation.size() == n) {
                results.push_back(permutation);
                return;
            }

            for (int i = 0; i < n; i++) {
                if (visited[i] || (i > 0 && nums[i] == nums[i - 1] && !visited[i - 1])) { // 1st
                //if (visited[i] || (i > 0 && nums[i] == nums[i - 1] && visited[i - 1])) { // 2nd
                    // the condition enfoce the duplicates only added once to the result.
                    // 1st is saying A[i - 1] is added, A[i] is not, then we add A[i].
                    // 2nd is saying none of A[i - 1] and A[i] is added, then we add A[i].
                    // duplicated element only add once in all subtrees.
                    // e.g. 1 2 3 4 4 4 5 6 7. the 4, 4, 4 should be only selected once
                    continue;
                }

                visited[i] = true;
                permutation.push_back(nums[i]);
                helper(nums, n, visited, permutation, results);
                permutation.pop_back();
                visited[i] = false;
            }
        }
    };
    ```

### Combinations

* Notice the `=` in the for loop, here the `i` is not array index, but number we
  are enumerating.

```c++
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> results;
        vector<int> comb;

        helper(n, k, 1, comb, results);

        return results;
    }

    void helper(int n, int k, int cur, vector<int>& comb,
                vector<vector<int>>& results) {

        if (comb.size() == k) {
            results.push_back(comb);
            return;
        }

        for (int i = cur; i <= n; i++) {
            comb.push_back(i);
            helper(n, k, i + 1, comb, results);
            comb.pop_back();
        }
    }
};
```

### Combination Sum

* Notice in line 23, the recursion is on `i` again even though it has been selected
  already.

```c++
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> results;
        vector<int> result;

        sort(candidates.begin(), candidates.end());
        helper(candidates, 0, target, result, results);

        return results;
    }

    void helper(vector<int> nums, int cur, int target, vector<int>& result,
                vector<vector<int>>& results) {
        if (target == 0) {
            results.push_back(result);
            return;
        }

        for (int i = cur; i < nums.size(); i++) {
            if (nums[i] <= target) {
                result.push_back(nums[i]);
                helper(nums, i, target - nums[i], result, results);
                result.pop_back();
            }
        }
    }
};
```

### Combination Sum II

* Notice in this problem, duplicate number could be in the original array, don't
  worry, we will treat the same element in different position differently.
* Notice how this code can be changed from the Combinations Sum, compare to the
  solution we only add the if check in line 21, and call the hlepr function with
  `i + 1` in line 27.

```c++ hl_lines="21 27"
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> results;
        vector<int> result;

        sort(candidates.begin(), candidates.end());
        helper(candidates, 0, target, result, results);

        return results;
    }

    void helper(vector<int> nums, int cur, int target, vector<int>& result,
                vector<vector<int>>& results) {
        if (target == 0) {
            results.push_back(result);
            return;
        }

        for (int i = cur; i < nums.size(); i++) {
            if (i > cur && nums[i] == nums[i - 1]) {
                continue;
            }

            if (nums[i] <= target) {
                result.push_back(nums[i]);
                helper(nums, i + 1, target - nums[i], result, results);
                result.pop_back();
            }
        }
    }
};
```

### Combination Sum III

* Very similar to the [Combination Sum](#combination-sum). Instead of giving an
  array, this problem gives numbers from `1` to `9` and no duplicate selection allowed.
* The meaning of this problem is also similar to the problem [K Sum](#k-sum),
  while K Sum is asking "how many", so it is solved using Dynamic Programming.
* differ with [Combination Sum](#combination-sum) in the for loop bounds and the
  recursive call, remember in [Combination Sum](#combination-sum), the recursive
  function called with `i` instead of `i + 1` because duplication is allowed in
  that problem.

```c++
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> results;
        vector<int> result;

        sort(candidates.begin(), candidates.end());
        helper(candidates, 0, target, result, results);

        return results;
    }

    void helper(vector<int> nums, int cur, int target, vector<int>& result,
                vector<vector<int>>& results) {
        if (target == 0) {
            results.push_back(result);
            return;
        }

        for (int i = cur; i < nums.size(); i++) {
            if (i > cur && nums[i] == nums[i - 1]) {
                continue;
            }

            if (nums[i] <= target) {
                result.push_back(nums[i]);
                helper(nums, i + 1, target - nums[i], result, results);
                result.pop_back();
            }
        }
    }
};
```

### Combination Sum IV (Backpack VI)

* 这里可以随便取，似乎题目变得无法下手，考虑“最后一步”这个技巧不能用了，因为最后一步可以是任意一个了。
* 但仍然可以用子问题来考虑。先不管最后一步是哪一个，最后一步之前的相加的总和一定是 `Target - x`.
  这样就转化成一个子问题可以用DP来做。
* 具体做法我们可以对于每一个小于“总承重”的重量进行枚举最后一步`x`。可能的`x`是`A[0], ..., A[i - 1]`
  中任意一个.
* Must initialize `f[i] = 0`. Because some of the state won't be updated
  indicates that they are not possible to fill).

```c++
class Solution {
public:
    int backPackVI(vector<int>& nums, int T) {
        int n = nums.size();
        if (n == 0) {
            return 0;
        }
        int f[T + 1];
        f[0] = 1;
        /* for each sub problem */
        for (int i = 1; i <= T; i++) {
            f[i] = 0;
            /* enumerate the last step */
            for (int j = 0; j < n; j++) {
                if (i >= nums[j]) {
                    f[i] += f[i - nums[j]];
                }
            }
        }

        return f[T];
    }
};
```

!!!Note
    即使是简单的一维背包，依然是总承重放入状态(即所开数组与总承重相关)

Print one such combination solution

* Suppose we also interested in print one of the possible solution. How could we
  change the code?
* `f[i]`: 存多少种方式
* `pi[i]`: 如果 `f[i] >= 1`, 最后一个数字可以是`pi[i]`

=== "Print one such combination"

```c++
class Solution {
public:
    int backPackVI(vector<int>& nums, int T) {
        int n = nums.size();
        if (n == 0) {
            return 0;
        }

        int f[T + 1];
        /* pi[i]: 如果i可拼出(f[i] >= 1), 最后一个是pi[i] */
        int pi[T + 1];
        f[0] = 1;

        for (int i = 1; i <= T; i++) {
            f[i] = 0;
            for (int j = 0; j < n; j++) {
                if (i >= nums[j]) {
                    f[i] += f[i - nums[j]];
                    /* 最后一个是nums[j]的可拼出i */
                    if (f[i - nums[j]] > 0) {
                        /* 纪录下来 */
                        pi[i] = nums[j];
                    }
                }
            }
        }

        if (f[T] > 0) {
            int i = T;
            cout << i << "=" << endl;
            while (i != 0) {
                // sum is i now;
                // last number is pi[i]
                // previuos sum is i - pi[i]
                cout << pi[i] << endl;
                i -= pi[i];
            }
        }

        return f[T];
    }
};
```

### 491. Increasing Subsequences

* notice the given array may not sorted. You need to consider duplidates in this case.
* to eliminite duplication, we use a set to keep the "root of the subtree" and make
  sure no duplicates in the same level when grow the tree.

```python
class Solution:
    def findSubsequences(self, nums: List[int]) -> List[List[int]]:
        res = []
        def helper(nums, index, path):
            if len(path) > 1:
                res.append(path[:])

            seen = set()
            for i in range(index, len(nums)):
                if nums[i] in seen: continue

                if not path or nums[i] >= path[-1]:
                    path.append(nums[i])
                    helper(nums, i + 1, path)
                    path.pop()

                seen.add(nums[i])

        helper(nums, 0, [])

        return res
```