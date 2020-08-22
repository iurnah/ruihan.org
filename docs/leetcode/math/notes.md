# Math and Geometry

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