# Binary Search

## Binary search problem characteristics

1. Ordered binary search. You need to find an index or array element where ordering
   info is available, either explicitly (sorted array) or implicitly (partially
   sorted or other special info).
2. monotony pattern. If the ordering info isn't available, but you can exclude
   "all" the possible cases from left or right by a condition comparing `f(mid)`
   to the `target`.

## Binary search problem solving techniques

1. Clarify that you are trying to find the first one or find the last one.
2. Clarify that you are trying to move the index or move the value
   (i.e. kth smallest number in a multiplicative table).
3. Use an "ordering abstraction" `vless(target, f(mid))`. This ordering
   abstraction will produce a boolean array that indicate the ordering
   information between the target value and the `f(mid)`.
4. Decide whether the left or right part the `f(mid)` should fall into.
   The principle to determine the predicate is simple: don't rule out
   the possible result (maintain the loop invariant).
5. Shrink the range accordingly based on the predicate decided in step 3.
6. Test the case that the search range is small, such as only have one or two elements.

## Binary search practical use case

1. Find whether the given target is in the array.
2. Find the position of the first value equal to the given target.
3. Find the insertion position of the given target in the array.
4. Find the position of the last value equal to the given target.
5. Find the total number of x in a sorted array.
6. Find the last element less than the target.
7. Find the first element greater than the target.

## Binary search in C++ STL

1. `lower_bound`: return iterator point to the element __no less__ than the target.
2. `upper_bound`: return iterator point to the element __greater__ than the target.
3. `equal_range`: return a pair of iterators, the first of which is `lower_bound`,
   the second is `upper_bound`.
4. `binary_search`: return true if an element equivalent to `val` is found, and
   false otherwise.

## Caveat of binary search implementation

1. Specify the range: `[start, end)` or `[start, end]`? C++ STL used `[start, end)`
   to denote a range, which bring in many conveniences. We will stick on this convention.
2. Which while loop condition? `start < end`? `start <= end`? `start != end`? `start + 1 < end`?
3. The calculation of the `mid`. `mid = start + (end - start) / 2` or `mid = (start + end) / 2`?
4. To proof `mid` is always in the range `[begin, end)`.
5. The "bisection": `start = mid + 1`, `start = mid`, or `end = mid - 1` or `end = mid`?
6. Where is the result? `start`? `end`? How to make sure?

## A "universal" binary search implementation

Despite the above caveats, just remember that there are two versions of binary
search one can write based on the range `[begin, end)` and `[begin, end]`.
Iterator type in C++ using the former, it have many benefits in reduce the code
complexity. Among all the binary search implementation you have seen, the
following one is the most powerful version and it equivalent to C++ STL
`lower_bound` algorithm.

```c++
/**
* return an index to an element no less than x. Be more specifically, if there is
* an element in the given array equal to x, it returns the index of first such
* element; if there is no element that is equal to x, it returns the index
* where x can be inserted into the position without changing the ordering of
* the elements.
*
* All possible return value for calling this function with array.size() == n is
* [0, 1, ..., n - 1, n]
*
*/
size_t binary_search(int x, vector<int>& array, size_t n)
{
    size_t begin = 0, end = n;
    while (begin != end) {
        size_t mid = begin + (end - begin) / 2;
        if (array[mid] < x) {
            begin = mid + 1;
        } else {
            end = mid;
        }
    }

    return begin;
}
```

### Important observations about this implementation

1. `mid` cannot less than `begin`, they can be equal. This will ensure
   `begin = mid + 1` in the if statement at least reduce the size of [begin, end] by 1.
    * Informal proof: `if (array[mid] < x)`, it indicate x can only possible be in
    array[mid + 1, mid + 2, ... n - 1]. `mid + 1` is at least 1 greater than begin.  

2. `mid` and `end` never equal inside the while loop, `mid < end` is always hold.
   This will ensure `end = mid` in the else statement at least reduce the size of
   `[begin, end]` by 1.
    * Informal proof: we have `begain < end`, so `begin + end < 2 * end`, thus
    `(begin + end) / 2 < end`, because integer divisioin truncate down, `mid =
    (begin + end) / 2` always less than end.

3. `begin` and `end` never cross.
    * Informal proof: Inside the while loop, at the begining, we have `begin <
    end`. If the current iteration executes the `if` condition, `begin = mid +
    1` at most advance `begin` to `end` but not exceed `end`. If it execute the
    `else` condition, `end = mid` would at worst case change `end` point to the
    minimum value of `mid`, because we have `begin <= mid`. Thus, we can conclude
    that executing the statement `end = mid` will not change `end` less than `begin`,
    at worst equal to `begin`.

### Claims regarding this binary search routine

1. The range `[begin, end)` is used, which comply to the convention used in C++ iterator.
2. It is impossible that `mid == end`. If they are equal, `array[n]` is invalid memory access.
3. We use the loop condition `while (begin != end)` to indicate that once the
   loop terminates, we have `begin == end`. By checking whether `begin` is a valid
   index to the array or not, we can know whether `x` is greater than all the
   elements in the array or not. If we want to check whether `x` is found in the
   array, we simply check `array[begin] == x`. However, this condition is based
   on the assumption that `begin < end` initially. Considering that, using
   `while (begin < end)` is better if you cannot ensure `begin < end` before the loop.
4. Setting `begin = mid + 1` reduces the size of the remaining interested
   sub-array and maintains the invariant, which is if x in the array, x is in
   `[begin, end)`.
5. Setting `end = mid` reduces the size of the remaining interested sub-array
   (mid never equal to end) and maintains the invariant, which is if x in the array,
   x is in `[begin, end)`. This claim is a little hard to absorb. On way to
   understand is like the following: ~~Because we need keep searching x in the
   range `[begin, mid]` if we get in the else statement. In the else case there
   are two possibilities: 1) `array[mid] > x`. 2) `array[mid] = x`. For 1) it indicates
   `x` is in `[begin, mid)`, setting `end = mid` maintains the loop invariant correctly,
   which is that `x` is in the shrinked range. For the 2) it is a little complex.
   If `array[mid]` is the only element equal to x, setting `end = mid` appears violate
   the loop invariant by exclude x from the range `[begin, end)`. however, remember
   `array[mid]` is the only element equal to x, after the while loop, `begin = end`,
   we have the `x` found by `begin` even though theoretically `[begin, end)` is already
   an empty range since `begin = end` and `array[begin] = array[end] = x`. If there
   are more values are equal to `x` before and after the element `array[mid]` the
   loop will always end up finding the first `x` value in the array.
6. If we use `end = mid + 1`. Try test case `[1, 3, 5, 7]`, with `x = 0`. deadloop
   will accur. i.e. `begin = 0, mid = 1, end = 2`.

## Category 1 Binary search on sorted arrays

To solve this type of binary search problem. You should focus on the following:

1. Come up test cases to verify your solution.
2. Be able to find which side to drop for each iteration.
3. Be extremly careful "off by 1" bugs. (1. reasoning: is mid value possible to be
   the solution or not. 2. exercise test cases: especially short ones)

### 34. Search for a Range

=== "C++ Use STL function"

    ``` c++
    class Solution {
    public:
        vector<int> searchRange(vector<int>& nums, int target) {
            vector<int> res(2, -1);
            int low = lower_bound(nums.begin(), nums.end(), target) - nums.begin();
            int high = upper_bound(nums.begin(), nums.end(), target) - nums.begin();
            if (low == high)
                return res;

            return {low, hight - 1};
        }
    };
    ```

=== "C++ Implementation of binary search"

    ```c++
    class Solution {
    public:
        vector<int> searchRange(vector<int>& nums, int target) {
            vector<int> res(2, -1);

            int low = lower_bound(nums, target);
            //int high = lower_bound(nums, target + 1); // also works.
            int high = upper_bound(nums, target);
            if (low == high) {
                return res;
            }

            return {low, high - 1};
        }

        int lower_bound(vector<int>& nums, int target) {
            if (nums.size() == 0) return 0;
            int l = 0, r = nums.size();

            while (l < r) {
                int m = l + (r - l) / 2;
                if (nums[m] < target) {
                    l = m + 1;
                } else {
                    r = m;
                }
            }

            return l;
        }

        int upper_bound(vector<int>& nums, int target) {
            if (nums.size() == 0) return 0;
            int l = 0, r = nums.size();

            while (l < r) {
                int m = l + (r - l) / 2;
                if (nums[m] <= target) {
                    l = m + 1;
                } else {
                    r = m;
                }
            }

            return l;
        }
    };
    ```

=== "Python solution"

    ```python
    class Solution(object):
        def searchRange(self, nums, target):
            """
            :type nums: List[int]
            :type target: int
            :rtype: List[int]
            """
            if len(nums) == 0:
                return [-1, -1]

            begin = 0
            end = len(nums)

            while begin != end:
                mid = begin + (end - begin) / 2
                if nums[mid] < target:
                    begin = mid + 1
                else:
                    end = mid
            if begin == len(nums):
                return [-1, -1]

            if nums[begin] == target:
                lower = begin
            else:
                lower = -1

            begin = 0
            end = len(nums)

            while begin != end:
                mid = begin + (end - begin) / 2
                if nums[mid] <= target:
                    begin = mid + 1
                else:
                    end = mid

            if nums[begin - 1] == target:
                upper = begin - 1
            else:
                upper = -1

            return [lower, upper]
    ```

### 35. Search Insert Position

=== "C++ solution lower_bound"

```c++
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        if (nums.size() == 0) return 0;
        int l = 0, r = nums.size();

        while (l < r) {
            int m = l + (r - l) / 2;
            if (nums[m] < target) {
                l = m + 1;
            } else {
                r = m;
            }
        }

        return l;
    }
};
```

### 33. Search in Rotated Sorted Array

How to locate the sorted half?

1. If left half is sorted, check where the target `t` is like be.
   else if right half is sorted, check where the target `t` is like to be.
   else if mid element is equal to left or right. Remove one of them.
2. Although no duplicate, should consider short input like `[3 1], 1` will have the equal case.
=== "C++"

    ```c++
    /**
    t = 1       t = 3        t = 5       t = 4       t = -1
    5 1 2 3 4   5 1 2 3 4    5 1 2 3 4   5 1 2 3 4   5 1 2 3 4
    5 1               3 4    5 1               3 4   5 1
    1               3      5                   4     1 <--need check
    */
    class Solution {
    public:
        int search(vector<int>& A, int t) {
            if (A.empty()) return -1;

            int l = 0, r = A.size() - 1;

            while (l < r) {
                int m = l + (r - l) / 2;
                if (A[m] == t) return m;
                if (A[l] < A[m]) { // left is sorted
                    if (A[l] <= t && t < A[m]) {
                        r = m - 1;
                    } else {
                        l = m + 1;
                    }
                } else if (A[m] < A[r]) { // right is sorted
                    if (A[m] < t && t <= A[r]) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                } else { // if equal, remove one. case: [3, 1], 1
                    if (A[l] == A[m]) l++;
                    if (A[m] == A[r]) r--;
                }
            }

            return A[l] == t ? l : -1;
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def search(self, nums, target):
            """
            :type nums: List[int]
            :type target: int
            :rtype: int
            """
            if len(nums) == 0:
                return -1

            left = 0
            right = len(nums) - 1

            while left < right:
                mid = left + (right - left) / 2

                if nums[mid] == target:
                    return mid;

                if nums[left] < nums[mid]:
                    if nums[left] <= target and target < nums[mid]:
                        right = mid - 1
                    else:
                        left = mid + 1

                elif nums[mid] < nums[right]:
                    if nums[mid] < target and target <= nums[right]:
                        left = mid + 1
                    else:
                        right = mid - 1

                else:
                    if nums[left] == nums[mid]:
                        left += 1
                    if nums[right] == nums[mid]:
                        right -= 1

            if nums[left] == target:
                return left

            return -1
    ```

### 81. Search in Rotated Sorted Array II

How to locate the sorted half?

=== "C++"

    ```c++
    class Solution {
    public:
        bool search(vector<int>& A, int t) {
            if (A.empty())
                return false;

            int l = 0, r = A.size() - 1;

            while (l < r) {
                int m = l + (r - l) / 2;
                if (A[m] == t) return true;
                if (A[l] < A[m]) {
                    if (A[l] <= t && t < A[m]) {
                        r = m - 1;
                    } else {
                        l = m + 1;
                    }
                } else if (A[m] < A[r]) {
                    if (A[m] < t && t <= A[r]) {
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                } else {
                    if (A[l] == A[m]) l++;
                    if (A[m] == A[r]) r--;
                }
            }

            return A[l] == t? true : false;
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def search(self, nums, target):
            """
            :type nums: List[int]
            :type target: int
            :rtype: int
            """
            if len(nums) == 0:
                return False

            left = 0
            right = len(nums) - 1

            while left < right:
                mid = left + (right - left) / 2

                if nums[mid] == target:
                    return True

                if nums[left] < nums[mid]:
                    if nums[left] <= target and target < nums[mid]:
                        right = mid - 1
                    else:
                        left = mid + 1

                elif nums[mid] < nums[right]:
                    if nums[mid] < target and target <= nums[right]:
                        left = mid + 1
                    else:
                        right = mid - 1

                else:
                    if nums[left] == nums[mid]:
                        left += 1
                    if nums[right] == nums[mid]:
                        right -= 1

            if nums[left] == target:
                return True

            return False
    ```

### 153. Find Minimum in Rotated Sorted Array

Try to locate the valley which contains the min.

1. Notice when `A[0] < A[n - 1]`, return `A[0]`.
2. Draw a monotonic curve and then split the curve into two half, swith the order. This can help you to write the code.

=== "C++"

    ```c++
    class Solution {
    public:
        int findMin(vector<int>& A) {
            int l = 0, r = A.size() - 1;
            while (l < r) {
                if (A[l] < A[r]) // serve as base case.
                    return A[l];

                int m = l + (r - l) / 2;

                if (A[m] > A[r]) { // also works. looking for not sorted half
                    l = m + 1;
                } else if (A[m] < A[r]) { // don't really need if statement
                    r = m;
                }
            }

            return A[l];
        }
    };
    ```
=== "Python"

    ```python
    class Solution(object):
        def findMin(self, nums):
            """
            :type nums: List[int]
            :rtype: int
            """
            if len(nums) == 0:
                return -1

            left = 0
            right = len(nums) - 1
            while left < right:
                if nums[left] == nums[right]:
                    return nums[left]

                mid = left + (right - left) / 2

                if nums[mid] > nums[right]:
                    left = mid + 1
                else:
                    right = mid

            return nums[left]
    ```

### 154. Find Minimum in Rotated Sorted Array II

Locate the valley which contains the min.

1. Since duplicates exist. we cannot use the observation `A[l] == A[r]`.
2. Here we deal with duplicates using decrease by one step.

=== "C++"

    ```c++
    class Solution {
    public:
        int findMin(vector<int>& A) {
            int l = 0, r = A.size() - 1;

            while (l < r) {
                int m = l + (r - l) / 2;

                if (A[m] > A[r]) {
                    l = m + 1;
                } else if (A[m] < A[r]) {
                    r = m;
                } else {
                    r--;
                }
            }

            return A[l];
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def findMin(self, nums):
            """
            :type nums: List[int]
            :rtype: int
            """
            if len(nums) == 0:
                return -1

            left = 0
            right = len(nums) - 1

            while left < right:
                mid = left + (right - left) / 2

                if nums[mid] > nums[right]:
                    left = mid + 1
                elif nums[mid] < nums[right]:
                    right = mid
                else:
                    right -= 1

            return nums[left]
    ```

### 162 Find Peak Element

Use Binary search

1. Use the neighboring relation to determin which side a peak value may occur then eliminate the other side.

```c++
class Solution {
public:
    int findPeakElement(vector<int>& A) {
        int l = 0, r = A.size() - 1;
        while (l < r) {
            int m = l + (r - l) / 2;
            if (A[m] < A[m + 1]) {
                l = m + 1;
            } else if (A[m] > A[m + 1]) {
                r = m;
            }
        }

        return l;
    }
};
```

### 278. First Bad Version

Binary search

1. Notice how this can be related to the ordering abstraction.

```c++
// Forward declaration of isBadVersion API.
bool isBadVersion(int version);
class Solution {
public:
    int firstBadVersion(int n) {
        int l = 0, r = n;
        while (l < r) {
            int m = l + (r - l) / 2;
            if (!isBadVersion(m)) {
                l = m + 1;
            } else {
                r = m;
            }
        }

        return l;
    }
};
```


### 74. Search a 2D Matrix

Binary search

1. We can view the matrix as a big sorted array and then binary search the target.
2. Notice test your finished routine using edge cases. (i.e. the initial value
   of end)

```c++
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        if (m == 0 || n == 0) return false;

        int start = 0, end = m * n - 1;
        while (start < end) {
            int mid = start + (end - start) / 2;

            int i = mid / n, j = mid % n;
            if (matrix[i][j] < target) {
                start = mid + 1;
            } else {
                end = mid;
            }
        }

        return matrix[start / n][start % n] == target ? true : false;
    }
};
```

### 240. Search a 2D Matrix II

Binary search to exclude whole column or whole row

1. the key is you decide where to start the compare. If you start from left bottom or right top, the solution should be abvious.
2. Notice the idea is from binary search, if ordering info available, we want to exclude as many as impossible values as we can.

```c++
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        if (m == 0 || n == 0) return false;

        int x = m - 1, y = 0;
        while (x >= 0 && y < n) {
            if (matrix[x][y] == target) {
                return true;
            }

            if (matrix[x][y] < target) {
                y++;
            } else {
                x--;
            }
        }

        return false;
    }
};
```

### 302. Smallest Rectangle Enclosing Black Pixels

=== "C++ Brute Force"

```c++
class Solution {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int m = image.size();
        int n = m ? image[0].size() : 0;
        int top = m, bottom = 0, left = n, right = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (image[i][j] == '1') {
                    top = min(top, i);
                    bottom = max(bottom, i + 1);
                    left = min(left, j);
                    right = max(right, j + 1);
                }
            }
        }

        return (right - left) * (bottom - top);
    }
};
```

Binary search

1. Notice the binary search idea is related to the problem Smallest Good Base
   and Wood Cut.
2. The basic idea is to search each of `1` from 4 directions. First, make
   sure you can search one boundary and the others are similar. For example, to
   search the first row that contains `1`, we can look at the whole column/row
   to see whether this col/row have `1`. Because we are searching the first row
   that have `1` top down, bisec based on the count of `1` on each row we can
   know whether we ignore the upper half or the lower half.

=== "C++ Binary Search"

    ```c++
    class Solution {
    public:
        int minArea(vector<vector<char>>& image, int x, int y) {
            int m = image.size();
            int n = m ? image[0].size() : 0;

            int top = bsearch_byrows(image, 0, x, 0, n, true); // search top
            int bottom = bsearch_byrows(image, x + 1, m, 0, n, false);
            int left = bsearch_bycols(image, 0, y, top, bottom, true);
            int right = bsearch_bycols(image, y + 1, n, top, bottom, false);

            return (bottom - top) * (right - left);
        }

        int bsearch_byrows(vector<vector<char>>& image, int x, int y,
                        int left, int right, bool white2black) {
            while (x < y) {
                int m = (x + y) / 2;
                int k = left;
                while (k < right && image[m][k] == '0') k++;
                if (k < right == white2black) { // mth row have '1'
                    y = m;
                } else {
                    x = m + 1;
                }
            }

            return x;
        }

        int bsearch_bycols(vector<vector<char>>& image, int x, int y,
                        int top, int bottom, bool white2black) {
            while (x < y) {
                int m = (x + y) / 2;
                int k = top;
                while (k < bottom && image[k][m] == '0') k++;
                if (k < bottom == white2black) { // mth column have '1'
                    y = m;
                } else {
                    x = m + 1;
                }
            }

            return x;
        }
    };
    ```

=== "Python Binary Search"

    ```python
    class Solution(object):
        def minArea(self, image, x, y):
            """
            :type image: List[List[str]]
            :type x: int
            :type y: int
            :rtype: int
            """
            m = len(image)
            n = 0
            if m != 0:
                n = len(image[0])

            top = self.bsearch_row(image, 0, x, 0, n, True)
            bottom = self.bsearch_row(image, x + 1, m, 0, n, False)
            left =  self.bsearch_col(image, 0, y, top, bottom, True)
            right = self.bsearch_col(image, y + 1, n, top, bottom, False)

            return (bottom - top) * (right - left)

        def bsearch_row(self, image, start, end, lower, upper, white2black):
            while start < end:
                m = (start + end) / 2
                k = lower

                while k < upper and image[m][k] == '0':
                    k += 1

                if (k < upper) == white2black:
                    end = m
                else:
                    start = m + 1

            return start

        def bsearch_col(self, image, start, end, lower, upper, white2black):
            while start < end:
                m = (start + end) / 2
                k = lower

                while k < upper and image[k][m] == '0':
                    k += 1

                if (k < upper) == white2black:
                    end = m
                else:
                    start = m + 1

            return start
    ```

=== "C++ BFS"

    ```c++
    class Solution {
    public:
        int minArea(vector<vector<char>>& image, int x, int y) {
            int m = image.size();
            int n = m ? image[0].size() : 0;

            int top = m, bottom = 0, left = n, right = 0;

            int xx[4] = {-1, 0, 1, 0};
            int yy[4] = {0, 1, 0, -1};

            queue<pair<int, int>> q;
            q.push({x, y});
            image[x][y] = '0';

            while (!q.empty()) {
                pair<int, int> t = q.front(); q.pop();

                top = min(top, t.first);
                bottom = max(bottom, t.first + 1);
                left = min(left, t.second);
                right = max(right, t.second + 1);

                for (int k = 0; k < 4; ++k) {
                    int a = t.first + xx[k];
                    int b = t.second + yy[k];
                    if (a >= 0 && a < m && b >= 0 && b < n && image[a][b] == '1') {
                        q.push({a, b});
                        image[a][b] = '0';
                    }
                }
            }

            return (right - left) * (bottom - top);
        }
    };
    ```

=== "Python BFS"

    ```python
    from collections import deque

    class Solution(object):
        def minArea(self, image, x, y):
            """
            :type image: List[List[str]]
            :type x: int
            :type y: int
            :rtype: int
            """
            m = len(image)
            n = 0
            if m != 0:
                n = len(image[0])

            xx = [-1, 0, 1, 0]
            yy = [0, -1, 0, 1]

            top = m
            bottom = 0
            left = n
            right = 0

            q = deque()
            q.append([x, y])
            image[x][y] = '0'

            while len(q) > 0:
                t = q.popleft()

                top = min(top, t[0])
                bottom = max(bottom, t[0] + 1)
                left = min(left, t[1])
                right = max(right, t[1] + 1)

                for k in range(4):
                    a = t[0] + xx[k]
                    b = t[1] + yy[k]
                    if a >= 0 and a < m and b >= 0 and b < n and image[a][b] == '1':
                        q.append([a, b])
                        image[a][b] = '0'

            return (right - left) * (bottom - top)
    ```

=== "C++ DFS"

    ```c++
    class Solution {
    private:
        int m, n;
        int top, bottom, left, right;
    public:
        int minArea(vector<vector<char>>& image, int x, int y) {
            m = image.size();
            n = m ? image[0].size() : 0;
            top = m, bottom = 0, left = n, right = 0;

            dfs_helper(image, x, y);

            return (right - left) * (bottom - top);
        }

        void dfs_helper(vector<vector<char>>& image, int x, int y) {
            if (x < 0 || x >= m || y < 0 || y >= n || image[x][y] == '0') {
                return;
            }

            image[x][y] = '0';
            top = min(top, x);
            bottom = max(bottom, x + 1);
            left = min(left, y);
            right = max(right, y + 1);

            dfs_helper(image, x - 1, y);
            dfs_helper(image, x, y + 1);
            dfs_helper(image, x + 1, y);
            dfs_helper(image, x, y - 1);
        }
    };
    ```

### 363. Max Sum of Rectangle No Larger Than K

Iterate the wide of the matrix and using prefix sum and set `lower_bound`.

1. From the problem Max Sum of Subarry No Larger Than K, we have to enumerate
   the width of the sub-matrix and sum up all row elements and get an array
   of length `m`, `m` is the number of rows of the matrix. Then apply the method.

=== "C++ presum"

    ```c++
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

merge sort

1. The idea is similar that solution 1. Instead of calculate `preSum` on the
   fly, we finish calculation and pass it to a `mergeSort` routine.
2. The use `mergeSort` here is to find the `A[j] - A[i] <= k` efficiently,
   `O(nlogn)`.

=== "C++ Merge Sort"

    ```c++
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
                while (j < end && sums[j] - sums[i] <= k) ++j; // search first time sums[j] - sums[i] > k
                if (j - 1 >= mid) { // sums[j - 1] - sums[i] <= k, make sure j - 1 is still in right side
                    res = max(res, (int)(sums[j - 1] - sums[i]));
                    if (res == k) return k;
                }
                while (t < end && sums[t] < sums[i]) {
                    cache[c++] = sums[t++];
                }
                cache[c++] = sums[i];
            }

            for (int i = start; i < t; ++i) {
                sums[i] = cache[i - start];
            }

            return res;
        }
    };
    ```

## Category 2 Using ordering abstraction

### 69. Sqrt(x)

Solution 1 using ordering abstraction definition

To find a square root of a integer `x` using binary search. We need to
first determin the range [left, right] that the target value `sqrt(x)` may in.
The potential range we can search is `[0, x/2 + 1]`.

Then we should clarify this binary search is the "find the first one" type or
the "find the last one" type. Basically, we want to determine our ordering
abstraction `f(target, g(i))` that is able to produce a boolean array. The
boolean array have true part and false part seperated. Here `target = sqrt(x)`
and `g(i) = i`. We define `f(sqrt(x), i) = true` when `i <= sqrt(x)` and
`f(sqrt(x), i) = false` when `i > sqrt(x)`. This came from the following
intuition: We are looking for the "last" integer whose square is less than `x`.
Why not the otherwise? Because if you change to find the "first" integer whose
square is greater than the `x` from right section of the boolean array, it is
hard to define our ordering abstraction `f`. Of cause, we can search the "first"
integer whose square is greater than `x` and find the previous integer next to
it as the solution, but this later solution is a bit complex and counter
intuitive. We prefer the first definition of ordering abstraction. Although a
workable solution following the second ordering abstraction is also given below.

For example: to solve the sqrt(8) and sqrt(9) using our definition:

```text
         k, i =  0  1  2  3  4  5  6  7  8  9  10  n = 11
            A = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
f(sqrt(8), k) = [T  T  T  F  F]
f(sqrt(9), k) = [T  T  T  T  F]
```

The binary search routine will be:

```c++
class Solution {
public:
    int mySqrt(int x) {
        int l = 0, r = x / 2 + 1;
        while (l < r) {
            // int m = l + (r - l) / 2; // will deadloop for 4, why?
            int m = r - (r - l) / 2;
            if (m <= x / m) {
                l = m;
            } else {
                r = m - 1;
            }
        }

        return l;
    }
};
```

Solution 2 using the alternative ordering abstraction definition

Second ordering abstraction (find first value whose square is greater than x)

```text
         k, i =  0  1  2  3  4  5  6  7  8  9  10  n = 11
            A = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
f(sqrt(8), k) = [F  F  F  T  T]
f(sqrt(9), k) = [F  F  F  F  T]
```

```c++
class Solution {
public:
    int mySqrt(int x) {
        if (x == 0) return 0;     // should handle, but will got division by zero in line 9.
        int l = 0, r = x / 2 + 2; // r = x / 2 + 1 will not working for x = 1, have to have the one past last;  
        while (l < r) {
            //int m = r - (r - l) / 2; // will dead loop for 4
            int m = l + (r - l) / 2;
            if (m > x / m) {
                r = m;
            } else {
                l = m + 1;
            }
        }

        return l - 1;
    }
};
```

### 367. Valid Perfect Square

Solution 1 Binary search using ordering abstraction

1. Notice you have to run tests for cases from 1 to 5.

=== "C++"

    ```c++
    class Solution {
    public:
        bool isPerfectSquare(int num) {
            if (num == 1) return true;
            int begin = 1, end = num / 2;
            while (begin < end) {
                //long long mid = begin + (end - begin) / 2; // not working, deadloop for 5
                long long mid = end - (end - begin) / 2;
                if (mid * mid == num)
                    return true;

                if (mid * mid < num) {
                    begin = mid;
                } else {
                    end = mid - 1;
                }
            }

            return false;
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def isPerfectSquare(self, num):
            """
            :type num: int
            :rtype: bool
            """
            if num == 1:
                return True

            lower = 1
            upper = num / 2

            while lower < upper:
                mid = upper - (upper - lower) / 2
                if mid * mid == num:
                    return True

                if mid * mid < num:
                    lower = mid
                else:
                    upper = mid - 1


            return False
    ```

### 633. Sum of Square Numbers

Solution 1 Binary search

1. Once you have derived value `b` from `a` and `c`, you can binary search `b`.

```c++
class Solution {
public:
    bool judgeSquareSum(int c) {
        if (c == 0) return true;
        for (long long a = 0; a * a <= c; ++a) {
            int b = c - (int) (a * a);

            int l = 0, r = b / 2 + 1;
            while (l < r) {
                long long m = r - (r - l) / 2;
                if (m * m == b)
                    return true;

                if (m * m < b) {
                    l = m;
                } else {
                    r = m - 1;
                }
            }
        }

        return false;
    }
};
```

Solution 2 Two pointers

1. Notice this square sum can be found efficiently using two pointers.

```c++
class Solution {
public:
    bool judgeSquareSum(int c) {
        int a = 0, b = sqrt(c);
        while(a <= b){
            int sum = a * a + b * b;
            if(sum < c) a++;
            else if(sum > c) b--;
            else return true;
        }

        return false;
    }
};
```

Solution 3 Using set

1. Keep inserting the value into a set, in the meantime also look up the other

```c++
class Solution {
public:
    bool judgeSquareSum(int c) {
        set<int> s;
        for (int i = 0; i <= sqrt(c); ++i) {
            s.insert(c - i*i);
            if (s.count(i*i)) return true;
        }
        return false;
    }
};
```

### 658. Find K Closest Elements

Solution 1 Binary search

1. Compare to problem 475. Heaters
2. Our search target is to find the starting index of the subarray of length K.

```c++
class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int start = 0, end = arr.size() - k;
        while (start < end) {
            int mid = start + (end - start) / 2;
            // looking for a "mid" that
            if (x - arr[mid] > arr[mid + k] - x) {
                start = mid + 1;
            } else {
                end = mid;
            }
        }

        return vector<int>(arr.begin() + start, arr.begin() + start + k);
    }
};
```

Solution 2 Binary search and Two pointers

* We first use binary search to locate the x value then expand to left and right
  looking for the k closest elements
* Notice the `i < 0` in the if condition, it is very important to be there.
  otherwise the array index will be out of bound.

```c++
class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int index = lower_bound(arr.begin(), arr.end(), x) - arr.begin();
        int i = index - 1, j = index;
        while (k--) {
            if (i < 0 || j < arr.size() && abs(arr[j] - x) < abs(arr[i] - x)) {
                j++;
            } else {
                i--;
            }
        }

        return vector<int>(arr.begin() + i + 1, arr.begin() + j);
    }
};
```

### 611. Valid Triangle Number

* The main idea comes from the triangle lateral property, in which the triple
  should fullfil: `a + b > c`, `a + c > b`, and `b + c > a`. Once we sort it. We
  are able to gain some advantages that we don't have to check all these 3
  relations. Instead, we should only take care of `A[i] + A[j] > A[k]`, in which
  `i < j < k`.
* Because we sorted the array, we can also fix the `i` and `j`, using binary
  search to find the `k` in the ragne of `A[j + 1] ~ A[n - 1]`. We can use our
  classic binary search template to achieve the goal.

```c++
class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        int n = nums.size();
        int res = 0;

        sort(nums.begin(), nums.end());
        for (int i = 0; i < n - 2; ++i) {
            for (int j = i + 1; j < n - 1; ++j) {
                int l = j + 1, r = n; // range of all possible k, notice l start with j + 1
                int t = nums[i] + nums[j];

                while (l < r) {
                    int m = l + (r - l) / 2;
                    if (nums[m] < t) {
                        l = m + 1;
                    } else {
                        r = m;
                    }
                }

                res += l - j - 1; // notice the count start from j + 1 to l - 1.
            }
        }

        return res;
    }
};
```

## Category 3 Using ordering abstration (monotonicity)

### 287. Find the Duplicate Number

Solution 1 Binary search

* The problem asking for better than `O(n^2)` we could check to see whether
  binary search will work.
* If you count how many values `<=` the mid elements of `[1, ..., n-1]`, it will
  give you enough information to discard part of the array.
* Here you should distinguish what will be split and what will be searched. The
  answer is the `[1, ..., n-1]` sequence, not the given array. The simple proof
  of why it works can be put in this the following way.
* If the count of elements that `<=mid` in the array is less than `mid`, we can
  learn that the duplicate is in the higher end. If the count is greater, we can
  know that the duplicate element is in the lower end of the sequence `[1, ..., n-1]`.

```c++
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int begin = 1, end = nums.size() - 1;
        while (begin < end) {
            int mid = begin + (end - begin) / 2;

            int count = 0;
            for (int a : nums) {
                if (a <= mid) ++count;
            }

            if (count <= mid) // "=" for [1,2,2]
                begin = mid + 1;
            else
                end = mid;
        }

        return begin;
    }
};
```

Solution 2 tortoise and hare algorithm

* This problem is very similar to the the find circle in linked list. Generally,
  if you repeate `A[A[i]]`, the out put will show some periodic patterns. In
  fact you can imagine a rho shaped sequence.
* Image there is a function `f(i) = A[i]`, it mapping from `1, 2, 3, ... n` to
  `1, 2, 3, ... n`. Try to traverse `A[i]`, you will finally get circle through
  some same sequence of elements again and again, thus you obtain a rho shaped
  sequency like a circle in a linked list. The reason of it being a rho shape is
  becuase at least one element you will not come back to it if you leave it.
* [Find Duplicate](http://www.keithschwarz.com/interesting/code/find-duplicate/FindDuplicate.python.html)

```c++
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;

        int slow = 0, fast = 0, find = 0;

        while(slow != fast || (slow == 0 && fast == 0)) {
            slow = nums[slow];
            fast = nums[nums[fast]];
        }

        while (slow != find) {
            slow = nums[slow];
            find = nums[find];
        }

        return find;
    }
};
```

### 360. Sort Transformed Array

### 374. Guess Number Higher or Lower

=== "C++ binary search"

    ```c++ 
    // Forward declaration of guess API.
    // @param num, your guess
    // @return -1 if my number is lower, 1 if my number is higher, otherwise return 0
    int guess(int num);

    class Solution {
    public:
        int guessNumber(int n) {
            int start = 1, end = n;
            while(start < end) {
                int mid = start + (end - start) / 2;
                if (guess(mid) == 0)
                    return mid;

                if (guess(mid) == 1) {
                    start = mid + 1;
                } else {
                    end = mid;
                }
            }

            return start;
        }
    };
    ```
=== "Python"

    ```python
    # The guess API is already defined for you.
    # @param num, your guess
    # @return -1 if my number is lower, 1 if my number is higher, otherwise return 0
    # def guess(num):

    class Solution(object):
        def guessNumber(self, n):
            """
            :type n: int
            :rtype: int
            """
            begin = 0
            end = n

            while begin != end:
                mid = begin + (end - begin) / 2
                if guess(mid) == 0:
                    return mid

                if guess(mid) == 1:
                    begin = mid + 1
                else:
                    end = mid

            return begin
    ```

### 475. Heaters

Sort then brute force

1. The solution we are looking for is the max value of the smallest house-heater distance.
2. Think through what is the distance you want to keep, min or max

=== "C++"

    ```c++
    class Solution {
    public:
        int findRadius(vector<int>& houses, vector<int>& heaters) {
            int m = houses.size();
            int n = heaters.size();
            sort(houses.begin(), houses.end());
            sort(heaters.begin(), heaters.end());

            int res = INT_MIN;
            int i, j = 0;
            for (i = 0; i < m; ++i) {
                while (j < n - 1 && abs(heaters[j + 1] - houses[i]) <= abs(heaters[j] - houses[i])) {
                    j++;
                }

                res = max(res, abs(houses[i] - heaters[j]));
            }

            return res;
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def findRadius(self, houses, heaters):
            """
            :type houses: List[int]
            :type heaters: List[int]
            :rtype: int
            """
            m = len(houses)
            n = len(heaters)

            houses.sort()
            heaters.sort()

            i = 0
            j = 0
            res = 0
            for i in range(m):
                while j < n - 1 and abs(heaters[j + 1] - houses[i]) <= abs(heaters[j] -  houses[i]):
                    j += 1

                res = max(res, abs(houses[i] - heaters[j]))

            return res
    ```

Binary search the neighboring heaters get max of min

1. Notice we cannot sort hourses and then search each heater's position. A special
   cases `[1, 2, 3] 2`, the result is `0` whereis it should be `1`.

=== "C++"

    ```c++
    class Solution {
    public:
        int findRadius(vector<int>& houses, vector<int>& heaters) {
            int n = heaters.size();

            sort(heaters.begin(), heaters.end());

            int res = INT_MIN;
            for (int house : houses) {
                int start = 0, end = n;

                while (start < end) {
                    int mid = start + (end - start) / 2;
                    if (heaters[mid] < house)
                        start = mid + 1;
                    else
                        end = mid;
                }

                int dist1 = (start == n) ? INT_MAX : heaters[start] - house;
                int dist2 = (start == 0) ? INT_MAX : house - heaters[start - 1];
                res = max(res, min(dist1, dist2));
            }

            return res;
        }
    };
    ```

=== "Python"

    ```python
    class Solution(object):
        def findRadius(self, houses, heaters):
            """
            :type houses: List[int]
            :type heaters: List[int]
            :rtype: int
            """
            m = len(houses)
            n = len(heaters)

            heaters.sort()

            i = 0
            j = 0
            res = float('-inf')
            for i in range(m):
                start = 0
                end = n
                while start != end:
                    mid = start + (end - start) / 2
                    if heaters[mid] < houses[i]:
                        start = mid + 1
                    else:
                        end = mid

                dist1 = float('inf')
                dist2 = float('inf')
                if start != n:
                    dist1 = heaters[start] - houses[i]
                if start != 0:
                    dist2 = houses[i] - heaters[start - 1]

                res = max(res, min(dist1, dist2))

            return res
    ```

### [410. Split Array Largest Sum](../../array/notes/#410-split-array-largest-sum)

### 1011. Capacity To Ship Packages Within D Days

Binary solution

Same as the [410. Split Array Largest Sum](../../array/notes/#410-split-array-largest-sum)

```c++
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int D) {
        int n = weights.size();
        if (n < D) return 0;

        int l = *max_element(weights.begin(), weights.end());
        int h = accumulate(weights.begin(), weights.end(), 0);

        while (l < h) {
            int m = (l + h) / 2;

            int c = 1; // need cut D-1 times
            int sum = 0;
            for (int w: weights) {
                if (sum + w > m) {
                    sum = 0;
                    c++;
                }
                sum += w;
            }

            if (c > D) {
                l = m + 1;
            } else {
                h = m;
            }
        }

        return l;
    }
};
```

### 875. Koko Eating Bananas

Binary search

Using the monotonic guessing approach, notice the trick in counting whether the
given guess value is possible.

=== "C++ binary search"

    ```c++
    class Solution {
    public:
        int minEatingSpeed(vector<int>& piles, int H) {
            int N = piles.size();
            if (N > H)
                return 0;

            int l = 1;
            long r = 10e9;

            while (l < r) {
                int k = l + (r - l) / 2;

                int hour = 0;
                for (int p : piles) {
                    if (k >= p) {
                        hour++;
                    } else {
                        hour += (p + k - 1) / k;
                    }
                }

                if (hour > H) { // K is too large,
                    l = k + 1;
                } else {
                    r = k;
                }
            }

            return l;
        }
    };
    ```

=== "C++ binary search simplified"

    ```c++
    class Solution {
    public:
        int minEatingSpeed(vector<int>& piles, int H) {
            int N = piles.size();
            if (N > H)
                return 0;

            int l = 1;
            long r = 10e9;

            while (l < r) {
                int k = l + (r - l) / 2;

                int hour = 0;
                for (int p : piles) {
                    hour += (p + k - 1) / k;
                }

                if (hour > H) { // K is too large,
                    l = k + 1;
                } else {
                    r = k;
                }
            }

            return l;
        }
    };
    ```

### 1539. Kth Missing Positive Number

Naive Solution

* using multiple variables and keep loop invariant.

Binary Search

* Observe the relation: total missing positives before `A[m]` is `A[m] -  1 - m`
  because the index `m` and `A[m]` is related to the missing positives thus can
  be used for counting.
* the bisection condition can be interpreted as a boolean predicate: "whether the
  number of missing positives before `A[m]` is __no less__ than `k`?"

=== "Naive Solution"

    ```c++ 
    class Solution {
    public:
        int findKthPositive(vector<int>& arr, int k) {
            if (arr.empty()) return k;
            int missing_cnt = arr[0] - 1;
            if (missing_cnt >= k) return k;
            int prev = arr[0];
            for (int i = 1;  i < arr.size(); ++i ) {
                if (!(arr[i] == prev || arr[i] == prev + 1)) {
                    int skip = arr[i] - prev - 1;
                    if (missing_cnt + skip >= k) {
                        return prev + k - missing_cnt;
                    }
                    missing_cnt +=skip;
                }
                prev = arr[i];
            }
            return (prev + k - missing_cnt);
        }
    };
    ```

=== "Binary Search"

    ```c++
    class Solution {
    public:
        int findKthPositive(vector<int>& arr, int k) {
            int l = 0, r = arr.size();
            while (l < r) {
                int m = l + (r - l) / 2;

                if (arr[m] - 1 - m < k) {
                    l = m + 1;
                } else {
                    r = m;
                }
            }

            return l + k;
        }
    };
    ```

### 1482. Minimum Number of Days to Make m Bouquets

Solution Binary Search

* Use a subroutine to compute whether the constrain can be meet or not.
* The search is looking for the whether m bouquets is possible, meet the binary
  pattern "no less than". So that we use `if(cnt_m < m)` and the return values is l.

=== "C++ binary search"

    ```c++
    class Solution {
    public:
        int minDays(vector<int>& bloomDay, int m, int k) {
            int l = *min_element(bloomDay.begin(), bloomDay.end());
            int r = *max_element(bloomDay.begin(), bloomDay.end());

            if (bloomDay.size() < m * k) return -1;

            while (l < r) {
                int mid = l + (r - l) / 2;

                int cnt_k = 0;
                int cnt_m = 0;
                for (int d: bloomDay) {
                    if (d > mid) {
                        cnt_k = 0;
                    } else {
                        cnt_k++;
                        if (cnt_k == k) {
                            cnt_m++;
                            cnt_k = 0;
                        }
                    }
                }

                if (cnt_m < m) {
                    l = mid + 1;
                } else {
                    r = mid;
                }
            }

            return l;
        }
    };
    ```

### 1283. Find the Smallest Divisor Given a Threshold

Solution Binary search

* Notice the specific divisor calculation. under this divisor operation, no matter
  how large the divisor is, the sum is always greater than `nums.size()`, if not,
  the solution is not guaranteed. so the threshold cannot smaller than `nums.size()`.
  This also indicate that the minimum divisor is less than or eaual to `max(nums)`.
* in the bsection predicate, notice this time the condition becomes `if (res > target)`
  essentially, the `if (f(mid) < target)` in the binary search templates is saying
  `mid` and `f(mid)` are positive correlation. here the `mid` and `res` are negative
  correlation.

=== "C++ binary search"

    ```c++  hl_lines="13"
    class Solution {
    public:
        int smallestDivisor(vector<int>& nums, int threshold) {
            int l = 1;
            int r = *max_element(nums.begin(), nums.end());

            while (l < r) {
                int m = l + (r - l) / 2;
                int res = 0;
                for (int num: nums) {
                    res += (num + m - 1) / m;
                }
                if (res > threshold) {
                    l = m + 1;
                } else {
                    r = m;
                }
            }

            return l;
        }
    };
    ```

### 1231. Divide Chocolate

Solution Binary search

1. The key difference between this problem and
   [410. Split Array Largest Sum](../../array/notes/#410-split-array-largest-sum)
   is this problem asks for maximizing the smallest sweetness of the pieces,
   whereas the [410. Split Array Largest Sum](../../array/notes/#410-split-array-largest-sum)
   asks minimizing the largest piece. Support `k` cuts generate `m` outcomes
   $S = \{s_1^{|k|}, s_2^{|k|}, \cdots, s_m^{|k|}\}$, this problem is to find
   the value of $\operatorname*{argmax}_m (\operatorname*{argmax}_k (S))$.
2. Imagine you guessed a value `m`, which is the maximum sweetness you can get
   from the smallest sweetness piece of all cuts. How to test whether the value
   `m` is possible? If possible, we will increase it to maximize it; if not,
    we will still keep it a candidate.

Same problem as [183. Wood cut](./#183-wood-cut-lintcode).

=== "Binary Search"

    ```c++  hl_lines="8 19-23"
    class Solution {
    public:
        int maximizeSweetness(vector<int>& sweetness, int K) {
            int start = *min_element(sweetness.begin(), sweetness.end());
            int end = accumulate(sweetness.begin(), sweetness.end(), 0);

            while (start < end) {
                int mid = (start + end + 1) / 2;
                int sum = 0;
                int cuts = 0;
                for (int s: sweetness) {
                    if ((sum += s) >= mid) {
                        sum = 0;
                        if (++cuts > K)
                            break;
                    }
                }

                if (cuts > K) {
                    // because >= mid above guarentee the "no less than" the guess.
                    // if cuts > K, mid could be the right answer and should be returned.
                    // Remember the binary search invariance requies not miss any
                    // answer in each iteration.
                    start = mid;
                } else {
                    end = mid - 1;
                }
            }

            return start;
        }
    };
    ```

!!! Note
    Compare the binary search solution of problem of [1231. Divide Chocolate](#1231-divide-chocolate)
    and [410. Split Array Largest Sum](../../array/notes/#410-split-array-largest-sum).
    Notice how different in checking the number of cuts. It exceeds the limit `K`,
    for max and min case, it indicate a very trivial difference in meaning.

### [Copy books (lintcode)](../../array/notes/#copy-books-lintcode)

### 183. Wood cut (lintcode)

Description

Given n pieces of wood with length `L[i]` (integer array). Cut them into small
pieces to guarantee you could have equal or more than k pieces with the same
length. What is the longest length you can get from the n pieces of wood? Given
L & k, return the maximum length of the small pieces. You couldn't cut wood into
float length. If you couldn't get >= k pieces, return 0.

Solution 1 Binary search

* It requires getting equal or more than `k` pieces of wood with the same length.
  So you have to cut the wood to fulfill the requirement. However, you need to
  promise that each of the `k` wood is the longest that is possible.
* Imagine that you are given a bunch of wood to cut. How would you do it? You
  probably want to try to make one cut and see whether you can make it or not.
  If not, you may want to make two cuts, and so on. But how could you program
  such a solution? It is very hard.
* Start thinking about the length seems a good option. Suppose you know your
  final maximum length. You would be able to make the cut accordingly. Now given
  a length out of guessing, can you verify whether it going to work or not? Yes,
  you can! That's the core idea of this solution.

```c++
class Solution {
public:
    int woodCut(vector<int> &L, int k) {
        if(L.empty()) return 0;
        int maxlen = *max_element(L.begin(), L.end());
        if(k == 0) return maxlen;
        int start = max(1, maxlen/k), end = maxlen;

        while(start < end) {
            int mid = start + (end - start) / 2;

            int count = 0;
            for(int len : L) {
                count += len / (mid + 1);
            }

            if(count >= k)
                start = mid + 1;
            else
                end = mid;
        }

        int count = 0;
        for(int len : L) count += len/start;
        return count >= k ? start : 0;
    }
};
```

### 774. Minimize Max Distance to Gas Station

Solution 1 Binary search

* It is very similar to the problem [Wood cut](#183-wood-cut-lintcode). You just
  need to take care of the accuracy of the results, namely also the int/double casts.
  It is also the hard part of the problem.
* Notice the `count` variable is int type, you should test your solution
  expecially for the line `count += dist[i] / mid`;

```c++
class Solution {
public:
    double minmaxGasDist(vector<int>& stations, int K) {
        int n = stations.size();
        vector<int> dist(n, 0); // dist[0] = null;
        int d = 0;

        for (int i = 1; i < n; ++i) {
            dist[i] = stations[i] - stations[i - 1];
            d = max(d, dist[i]);
        }

        double low = 0, high = d;
        while (low + 0.000001 < high) {
            double mid = low + (high - low) / 2;

            int count = 0;
            for (int i = 1; i < n; ++i) {
                count += dist[i] / mid;
            }

            if (count > K) { // mid is too small
                low = mid;
            } else {
                high = mid;
            }
        }

        return low;
    }
};
```

### 644. Maximum Average Subarray II

* Notice the great trick you used to test whether there is a subarray of length
  greater than `k` whose average is larger than current `mid`.
* The trick is calculate the `diff[i] = nums[i] - mid`,  and then calculate the
  prefix sum of the `diff` array, and compare to another prefix sum of the same
  array `diff`, the two prefix sum are calculated at two position at least `k`
  distant apart. We actually compare the prefix sum to the smallest prefix sum
  k distant apart.

```c++
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

        /* we keep looking for whether a subarray sum of length >= k in array "sums"
         * is possible to be greater than zero. If such a subarray exist, it means
         * that the target average value is greater than the "mid" value.
         * we look at the front part of sums that at least k element apart from i.
         * If we can find the minimum of the sums[0, 1, ..., i - k] and check if
         * sums[i] - min(sum[0, 1, ..., i - k]) >= 0. If this is the case, it indicate
         * there exist a subarray of length >= k with sum greater than 0 in sums,
         * we can return ture, otherwise, it return false. */
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

### 778. Swim in Rising Water

* In This problem we are trying to find a path, in which the maximum element in
  the path among all paths is minimum. Meaning we look for a target value in
  the grid, such that there exist a path from `grid[0][0]` to `grid[n-1][n-1]`
  which includes this value and it is the maximum value in the path.

```c++
class Solution {
    int x[4] = {0, -1, 0, 1};
    int y[4] = {-1, 0, 1, 0};
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        int begin = grid[0][0], end = n * n - 1;
        // binary search find a path with mini elevation
        while (begin < end) {
            int mid = begin + (end - begin) / 2;
            if (pathExist(grid, mid)) {
                end = mid;
            } else {
                begin = mid + 1;
            }
        }

        return begin;
    }

    bool pathExist(vector<vector<int>> & grid, int mid) {
        int n = grid.size();
        vector<vector<int>> visited(n, vector<int>(n, 0));
        return dfs_helper(grid, visited, n, mid, 0, 0);
    }

    bool dfs_helper(vector<vector<int>> & grid, vector<vector<int>>& visited,
                    int n, int mid, int i, int j) {
        visited[i][j] = 1;
        for (int k = 0; k < 4; ++k) {
            int a = i + x[k];
            int b = j + y[k];
            if (a < 0 || a >= n || b < 0 || b >= n || visited[a][b] == 1 || grid[a][b] > mid) continue;
            if (a == n - 1 && b == n - 1) return true;
            if (dfs_helper(grid, visited, n, mid, a, b)) return true;
        }

        return false;
    }
};
```

### 483 Smallest Good Base

Solution 1 Binary search

1. This problem requires a bit reasoning to achieve the solution.
2. The starting point is realy mean what's asking by the problem. Here it is
   asking a minimum base that represent the given number `n` in a representation
   like binary representation. For example: `13 = 3^0 + 3^1 + 3^2` so `13` can be
   representd as `111`(base 3).
3. First of all, there is a special case that such a base may not exist.
   (precisely, we should seperate the special case when `n = (n-1)^0 + (n-1)^1`;
   With this special case in mind, we can use binary search to iterate through
   each `m` from largest to smallest and check whether the corresponding `k` is a
   good base of the given value `n`. Because when `m` is the largest, `k` is the
   smallest, so if the bianry search find one it must be the smallest `k` we are
   looking for. If binary search found nothing, we simpley return the special
   case `n-1`.

```c++
class Solution {
public:
    string smallestGoodBase(string n) {
        long long num = stoll(n);
        /* for each lenght of the potentional representation,
         * n = 1 + k + ... + k^{i-1} = (k^i-1)/(k-1), lower bound k is 2,
         * we have 2^i-1 = n ==> upper bound i = log2(n+1). */
        for (int i = log2(num + 1); i >= 2; --i) {
            /* upper bound is obtained by n = 1 + k + k^2 ... + k^(i-1) > k^(i-1),
             * n > k^(i-1) ==> k < n^(1/(i-1)); */
            long long left = 2, right = pow(num, 1.0 / (i - 1)) + 1;
            while (left < right) {
                long long mid = left + (right - left) / 2;
                long long sum = 0;

                /* calculate i digits value with base "mid" */
                for (int j = 0; j < i; ++j) {
                    sum = sum * mid + 1;
                }

                /* binary search for the mid (good base) */
                if (sum == num)
                    return to_string(mid);

                if (sum < num)
                    left = mid + 1;
                else
                    right = mid;
            }
        }

        return to_string(num - 1);
    }
};
```

### 658. [Find K Closest Elements](#find-k-closest-elements)

### 378. Kth Smallest Element in a Sorted Matrix

Solution 1 Binary Search

1. The idea of using binary search for this problem my not be straightforward.
   But the method is very important. The idea is very similar to the problem
   Search in a rotated sorted array.
2. Because the matrix is sorted row wise and column wise, there are some ordering
   information we can make use of.
3. Notice we are not try to search using the matrix index, we are searching the
   matrix element value. Compare to the problem 287. Find the Duplicate Number.
4. The comparison `if (count < k)` isn't include mid explicitly. but the `count`
   is some function `f(mid)`, with the current `mid`, the count value is unique
   and can be use to test a condition that decide which `side` we can go to shrink
   the range the target value is possible in.

```c++
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        int start = matrix[0][0], end = matrix[m - 1][n - 1];
        while (start < end) {
            int mid = start + (end - start) / 2;

            int count = 0;
            for (int i = 0; i < m; ++i) {
                count += upper_bound(matrix[i].begin(), matrix[i].end(), mid) - matrix[i].begin();
            }

            if (count < k) { // notice no mid here, but count is a function of mid.
                start = mid + 1;
            } else {
                end = mid;
            }
        }

        return start;
    }
};
```

Solution 2 Priority Queue

1. Notice when the `k <= n^2`, index `j < matrix.size()` will also make it work.

```c++
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        priority_queue<int> pq;
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[0].size(); ++j) {
                pq.push(matrix[i][j]);
                if (pq.size() > k)
                    pq.pop();
            }
        }

        return pq.top();
    }
};
```

### 668. Kth Smallest Number in Multiplication Table

Solution 1 Binary search

1. While this problem looks simple. But it really isn't unless you observed the following.
2. The condition used for binary search is "whether there are k smaller elements
   in the range [start, mid]". You are looking for the smallest number that has
   k elements less than or equal to it. Like in the problem Kth Smallest Element
   in a Sorted Matrix, we will move the number not the index.
3. We move the `start` or `end` appropriately based on this condition, if there
   are more than k, we shrink the range by reduce end: `end = mid`. If there are
   less than k numbers, we increase `begin` hopefully to make `mid` larger so as
   to have close to k numbers in the range of `[1, mid]`.
4. When `being == end`, we've located the kth number desired. In case `k > m*n`,
   we will got `begin == end < k`, which is not a solution.
5. In counting how many element less than mid, you have to be clever a bit by using
   the feature that this matrix is multiplicative table. That is for row `i`, you
   can at most have `x/i` number smaller than `x`, why?
6. Follow up: Does the kth element will be in the range of `[1, m*n]`?

```c++
class Solution {
public:
    int findKthNumber(int m, int n, int k) {
        int begin = 1, end = m * n;

        while (begin < end) {
            int mid = begin + (end - begin) / 2;

            int count = 0;
            for (int i = 1; i <= m; ++i) {
                count += min(mid / i, n);
            }

            if (count < k)
                begin = mid + 1;
            else
                end = mid;
        }

        return begin;
    }
};
```

### 719. Find K-th Smallest Pair Distance

Solution 1 Priority Queue TLE

```c++
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        priority_queue<int> pq;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i + 1; j < nums.size(); ++j) {
                int dist = abs(nums[i] - nums[j]);
                if (pq.size() < k) {
                    pq.push(dist);
                } else if (dist < pq.top()) {
                    pq.push(dist), pq.pop();
                }
            }
        }

        return pq.top();
    }
};
```

Solution 2 Binary search

1. Similar to Problem 668. Kth Smallest Number in Multiplication Table.
2. The problem is complicated at firt glass. A brute force solutoin generates all
   the absolute distances and then sort to find the kth smallest one.
3. We found it is potentially a searchable senario if we sort the elements. We
   have range `[min_distance, max_distance]`. We search a distance in this range
   such that there are exactly k pairs distance including itself. If the count of
   pair distance less than k, we try to increase it buy `start = mid + 1`, vice versa.
4. When the binary search loop stops, if the result exist, `start` point to the
   distance we are searching. Since this problem guarrantee solution exist, we return `start`.

```c++
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {

        sort(nums.begin(), nums.end());

        int start = nums[1] - nums[0];
        for (int i = 2; i < nums.size(); ++i) {
            start = min(start, nums[i] - nums[i - 1]);
        }

        int end = nums.back() - nums[0];

        while (start < end) {
            int mid = start + (end - start) / 2;

            // count how many absolute differences that <= mid;
            int count = 0;
            for (int i = 0; i < nums.size(); ++i) {
                int j = i;
                while (j < nums.size() && nums[j] - nums[i] <= mid) j++;
                count += j - i - 1;
            }

            if (count < k) {
                start = mid + 1;
            } else {
                end = mid;
            }
        }

        return start;
    }
};
```

Solution 3 Using binary search to optimize the counting

1. You can also write your own binary search routine `upper_bound`.

```c++
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {

        sort(nums.begin(), nums.end());

        int start = nums[1] - nums[0];
        for (int i = 2; i < nums.size(); ++i) {
            start = min(start, nums[i] - nums[i - 1]);
        }

        int end = nums.back() - nums[0];

        while (start < end) {
            int mid = start + (end - start) / 2;

            // count how many absolute differences that <= mid;
            int count = 0;
            /*
            for (int i = 0; i < nums.size(); ++i) {
                int j = i;
                while (j < nums.size() && nums[j] - nums[i] <= mid) j++;
                count += j - i - 1;
            }
            */
            // optimize the counting use binary search (nested binary search)
            for (int i = 0; i < nums.size(); ++i) {
                auto iter = upper_bound(nums.begin() + i, nums.end(), nums[i] + mid);
                count += iter - (nums.begin() + i) - 1;
            }

            if (count < k) {
                start = mid + 1;
            } else {
                end = mid;
            }
        }

        return start;
    }
};
```

## Category 4 Binary search as an optimization routine

### 300 Longest Increasing Subsequence

Solution 1 DP

1. The base case is single char. `f[i]` is the length of LIS from the begining.

```c++
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (n == nums.size()) return 0;

        int f[n] = {0};

        int res = 0;
        for (int j = 0; j < n; j++) {
            f[j] = 1;
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

Solution 2 Using binary search

1. The DP solution is `O(n^2)`. Using binary search could reduce to `O(nlogn)`.
2. Binary search solution analysis. For each `i`, we are looking for the largest
   `f` value that has smallest `A` value. For example, `A[0] = 5` could be ignored
   because of its `f` value is same as `A[1] = 1`, which is smaller. In searching
   for the LIS, we prefer a small ending value when the length is the same.
3. The following solution using a vector `b` to record the minimum `A` value for
   each length of LIS (`f` value), we use binary search to find the last value in
   `b` that smaller that current value `A[i]`. If we found such a value in `b`,
   we use `A[i]` to replace the value next to the found value in `b`).

    ```text
    i  0 1 2 3 4 5 6  7
    A  5 1 3 7 6 4 2 10
    f  1 1 2 3 3 3 2  4
    f[1] = 1, a[1] = 1
    f[6] = 2, a[6] = 2
    f[5] = 3, a[5] = 4
    f[7] = 4, a[7] = 10
    ```

```c++
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> b;

        for (int i = 0; i < nums.size(); ++i) {
            int l = 0, r = b.size();
            while (l < r) {
                int m = l + (r - l) / 2;
                if (b[m] < nums[i]) { // nums[i] is the target
                    l = m + 1;
                } else {
                    r = m;
                }
            }

            if (l == b.size()) // nums[i] greater than all element in b
                b.push_back(nums[i]);
            else // begin point to next element no less than the target nums[i].
                b[l] = nums[i];
        }

        return b.size();
    }
};
```

Alternatively, we could use `lower_bound`.

```c++
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> b;

        for (int i = 0; i < nums.size(); ++i) {
            int l = lower_bound(b.begin(), b.end(), nums[i]) - b.begin();

            if (l == b.size()) // nums[i] greater than all element in b
                b.push_back(nums[i]);
            else // begin point to next element no less than the target nums[i].
                b[l] = nums[i];
        }

        return b.size();
    }
};
```

### 354. Russian Doll Envelopes

### 174. Dungeon Game
