# Math and Geometry

## Integer overflow and numeric limits

* On the fly detection: When parsing a string to integer, we check the current
  passed result res against the value INT_MAX / 10. ([String to Integer (atoi)]() and [Reverse Integer]())

  ```c++
  if (res > INT_MAX / 10 || (res == INT_MAX / 10 && str[i] - '0' > 7)) {
    /* process overflow cases */
  }
  // or
  if ((res > INT_MAX / 10 && x != 0) || (res < INT_MIN / 10 && x != 0)) {
    /* process overflow cases */
  }
  ```

## Solve by "median"

### [Minimum Moves to Equal Array Elements]()
### [Minimum Moves to Equal Array Elements II]()
### [Best Meeting Point]()

## Prime, GCD, LCM, Euclidean algorithm

### [Count Primes]()
### [Ugly Number]()
### [Ugly Number II]()
### [Super Ugly Number]()

## Cartesian plane

## Problems

### 478. Generate Random Point in a Circle

### 497. Random Point in Non-overlapping Rectangles

Solution 1 Trick to locate the coordinate in a matrix

```C++ tab="C++"
class Solution {
    int n = 0;
    vector<vector<int>> rectangles;
    vector<int> psum;

public:
    Solution(vector<vector<int>>& rects) {

        rectangles = rects;
        for (auto& r : rectangles) {
            n += (r[2] - r[0] + 1) * (r[3] - r[1] + 1);
            psum.push_back(n);
        }
    }

    vector<int> pick() {
        vector<int> res(2);

        // which rect to select?
        int target = rand() % n;

        int left = 0;
        int right = psum.size();

        // binary search: find the first greater element
        while (left != right) {
            int mid = left + (right - left) / 2;
            if (psum[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        vector<int> r = rectangles[left];

        // which point to select inside the selected rectangle?
        int width = r[2] - r[0] + 1;
        int height = r[3] - r[1] + 1;
        int base = psum[left] - width * height;

        return {r[0] + (target - base) % width, r[1] + (target - base) / width};
    }
};
```