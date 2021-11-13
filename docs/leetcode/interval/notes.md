# Interval problems

## Interval problem solving techniques

1. sweep line technique
2. iterval tree using a STL map
3. binary search to locate an interval in the list

## Special cases in disjoint interval set problems

1. Special case that the interval may have duplicates. i.e. Problem
   [Number of Airplanes in the Sky](#number-of-airplanes-in-the-sky)
2. Notice the search result in `::begin()` and `::end()` in interval query.

## Data structure of disjoint interval set

1. Two containers `vector` and `map` can store disjoint intervals.

## Disjoint interval operations

* Pay attention to the interval representation, whether it is open end or close
  end. The code is slitly different.

[Range Module](#range-module) is a design problem asking you to implement the
following all three operations.

### insertInterval (addInterval, mergeInterval)

=== "C++ vector"

    ```C++
    void addRange(int left, int right) {
        vector<pair<int, int>> tmp;
        int pos = 0;
        for (int i = 0; i < intervals.size(); i++) {
            if (left > intervals[i].second) {
                tmp.push_back(intervals[i]);
                pos++;
            } else if (right < intervals[i].first) {
                tmp.push_back(intervals[i]);
            } else {
                left = min(intervals[i].first, left);
                right = max(intervals[i].second, right);
            }
        }

        tmp.insert(tmp.begin() + pos, {left, right});
        swap(intervals, tmp);
    }
    ```

=== "C++ map"

    ```C++
    void addRange(int left, int right) {
        auto l = m.upper_bound(left);
        auto r = m.upper_bound(right);
        // rule out the case that l overlap with previous
        if (l != m.begin()) {
            if((--l)->second < left) {
                ++l;
            }
        }

        // sure left is non-overlap, check the right
        if (l != r) {
            int lmin = min(l->first, left);
            int rmax = max(right, (--r)->second);
            m.erase(l, ++r); // remove iterator
            m[lmin] = rmax;
        } else { // both no overlap
            m[left] = right;
        }
    }
    ```

### queryInterval

* Since we can use a sorted vector list or a map to store the interval. We could
  have two different implementation.
* The key to do the query is how to search the position, we use `upper_bound`.

=== "C++ vector"

    ```C++
    bool queryRange(int left, int right) {
        int n = invals.size(), l = 0, r = n;
        while (l < r) {
            int m = l + (r - l) / 2;
            if (invals[m].first <= left) {
                l = m + 1;
            } else {
                r = m;
            }
        }

        if (l == 0 || invals[--l].second < right) {
            return false;
        }

        return true;
    }
    ```

=== "C++ map"

    ```C++
    bool queryRange(int left, int right) {
        auto l = m.upper_bound(left);

        if (l == m.begin() || (--l)->second < right) {
            return false;
        }

        return true;
    }
    ```

### deleteInterval

=== "C++ vector"

    ```C++
    void removeRange(int left, int right) {
        int n = invals.size();
        vector<pair<int, int>> tmp;
        for (int i = 0; i < n; i++) {
            if (invals[i].second <= left || invals[i].first >= right)
                tmp.push_back(invals[i]);
            else {
                if (invals[i].first < left)  tmp.push_back({invals[i].first, left});
                if (invals[i].second > right) tmp.push_back({right, invals[i].second});
            }
        }

        swap(invals, tmp);
    }
    ```

=== "C++ map"

    ```C++
    void removeRange(int left, int right) {
        auto l = m.upper_bound(left);
        auto r = m.upper_bound(right);

        if (l != m.begin()) {
            --l;
            if(l->second < left) {
                ++l;
            }
        }

        // nothing need to be removed
        if (l == r) {
            return;
        }

        int l1 = min(left, l->first);
        int r1 = max(right, (--r)->second);

        m.erase(l, ++r);

        if (l1 != left) {
            m[l1] = left;
        }

        if (r1 != right) {
            m[right] = r1;
        }
    }
    ```

## Problems

### 56.Merge intervals

* Merge interval need first sort the interval then put the first interval to the
  result vector, iteratively compare the end of the vector to `interval[i]`,
  either update the `end` of the `res.back().end` or directly push the
  `interval[i]` to the end of the vector.

!!! Note
    You cannot use index to refer to the neighboring elements. because the vector is being modified.

```C++
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        if (intervals.empty()) return vector<Interval>();
        sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b){
            return a.start < b.start;
        });

        vector<Interval> res;
        res.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); ++i) {
            if (res.back().end >= intervals[i].start) {
                res.back().end = max(res.back().end, intervals[i].end);
            } else {
                res.push_back(intervals[i]);
            }
        }

        return res;
    }
};
```

### 57. Insert Interval

* Given the new interval `[a, b]`, consider each interval in the input `[x, y]`.
    1. Completely disjoint to the left `(b < x)`.
    2. Completely disjoint to the right `(y < a)`.
    3. Overlap with each other. (uniformly handled by `min`, `max`)
* The key to write code is
    1. Tracking the inserting position.
    2. Insert the new interval to the vector.
    3. Finally check the special cases: insert at the begin, insert at the end.

=== "One pass I"

    ```C++
    class Solution {
    public:
        vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
            int n = intervals.size();
            vector<Interval> res;
            int insertPos = 0;

            for (int i = 0; i < n; ++i) {
                if (newInterval.start > intervals[i].end) { // start no overlap
                    res.push_back(intervals[i]);
                    insertPos++;
                } else if (newInterval.end < intervals[i].start) { // end no overlap.
                    res.push_back(intervals[i]);
                } else { // overlap happens.
                    newInterval.start = min(newInterval.start, intervals[i].start);
                    newInterval.end = max(newInterval.end, intervals[i].end);
                }
            }

            res.insert(res.begin() + insertPos, newInterval);

            return res;
        }
    };
    ```

=== "One pass II"

    ```C++
    class Solution {
    public:
        vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
            int n = intervals.size();
            vector<Interval> res;

            for (int i = 0; i <= n; ++i) {
                if (i == n || newInterval.end < intervals[i].start) { // end no overlap.
                    res.push_back(newInterval);
                    while (i < n) res.push_back(intervals[i++]); // i will be after n
                } else if (newInterval.start > intervals[i].end) { // start no overlap
                    res.push_back(intervals[i]);
                } else { // overlap happens.
                    newInterval.start = min(newInterval.start, intervals[i].start);
                    newInterval.end = max(newInterval.end, intervals[i].end);
                }
            }

            return res;
        }
    };
    ```

### Number of Airplanes in the Sky

1. We can use a map to record each coordinate, add all coordinates up for each interval

```C++
class Solution {
public:
    int countOfAirplanes(vector<Interval> &airplanes) {
        unordered_map<int, int> mp;
        int res = 0;
        for (auto a : airplanes) {
            for (int i = a.start; i < a.end; ++i) {
                mp[i]++;
                res = max(res, mp[i]);
            }
        }

        return res;
    }
};
```

Solution 2 Sweep line

1. Sweep line technique usually seperate the intervals into two vectors. In this
   problem we seperate them and mark each point as start or end.
2. Notice you cannot use container like `set` or `map` because it may have
   duplicate intervals. You need to use vectors and then sort them.

```C++
class Solution {
public:
    int countOfAirplanes(vector<Interval> &airplanes) {
        vector<pair<int, int>> points;
        for (auto& a : airplanes) {
            points.push_back({a.start, 1});
            points.push_back({a.end, -1});
        }

        sort(points.begin(), points.end());

        int res = 0, count = 0;
        for (auto& p : points) {
            count += p.second;
            res = max(res, count);
        }

        return res;
    }
};
```

### 435. Non-overlapping Intervals

Solution 1 Greedy algorithm

=== "C++, sort by start"

    ```C++
    class Solution {
    public:
        int eraseOverlapIntervals(vector<vector<int>>& intervals) {
            int n = intervals.size();
            int res = 0;

            sort(intervals.begin(), intervals.end(),
                [](const vector<int>& a, const vector<int>& b){
                    return a[0] < b[0];
                });

            int left = 0;
            for (int i = 1; i < n; i++) {
                if (intervals[left][1] > intervals[i][0]) {
                    if (intervals[left][1] > intervals[i][1]) {
                        left = i;
                    }
                    res++;
                } else {
                    left = i;
                }
            }

            return res;
        }
    };
    ```

=== "C++, sort by end"

    ```C++
    class Solution {
    public:
        int eraseOverlapIntervals(vector<vector<int>>& intervals) {
            int n = intervals.size();
            int res = 0;

            sort(intervals.begin(), intervals.end(),
                [](const vector<int>& a, const vector<int>& b){
                    return a[1] < b[1];
                });

            int left = 0;
            int end = intervals[0][1];
            for (int i = 1; i < n; i++) {
                if (interval[i][0] < end) {
                    res++;
                } else {
                    end = intervals[i][1]; // no need to remove, update the new end
                }
            }

            return res
        }
    }
    ```

### 452. Minimum Number of Arrows to Burst Balloons

Solution 1 Sweep line, using map.

```C++
```

### 252. Meeting Rooms

```C++
```

### 253. Meeting Rooms II

Solution 1 Sweep line, split the start and end, put them into a vector.

1. same as the solution to the Number of Airplanes in the Sky
2. Notice the when a start and an end overlap, the sort will sort base on the
   pair::second. This make the dup end value goes before the start value so that
   we will not got over count the rooms.

```C++
/** special case:
2___5 <---- this 5 come first than the start 5
    5___9
*/
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        vector<pair<int, int>> points;

        for (auto& i : intervals) {
            points.push_back(make_pair(i.start, 1));
            points.push_back(make_pair(i.end, -1)); // this -1 < 1 is important
        }

        sort(points.begin(), points.end());

        int rooms = 0, count = 0;
        for (auto& p : points) {
            count += p.second;
            rooms = max(rooms, count);
        }

        return rooms;
    }
};
```

=== "lamda sorting"

    ```C++
    class Solution {
    public:
        int minMeetingRooms(vector<vector<int>>& intervals) {
            int n = intervals.size();
            if (n == 0) return 0;

            vector<vector<int>> points;
            for (auto &interval: intervals) {
                points.push_back({interval[0], 1});
                points.push_back({interval[1], -1});
            }
            sort(points.begin(), points.end(), [](vector<int> &a, vector<int> &b){
                if (a[0] == b[0]) {
                    return a[1] < b[1];
                } else {
                    return a[0] < b[0];
                }
            });

            int res = 0;
            int rooms = 0;
            for (auto &p : points) {
                rooms += p[1];
                res = max(res, rooms);
            }

            return res;
        }
    };
    ```

Solution 2

```C++
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        int n = intervals.size();
        vector<int> s(n, 0);
        vector<int> e(n, 0);
        for (int i = 0; i < n; i++) {
            s[i] = intervals[i].start;
            e[i] = intervals[i].end;
        }

        sort(s.begin(), s.end());
        sort(e.begin(), e.end());

        int rooms = 0;
        int j = 0;
        for (int i = 0; i < n; i++) {
            if (s[i] < e[j]) { /* meeting i start before the lastest end meeting j */
                rooms++;
            } else { /* s[i] >= e[j] indicate new meeting i will start after meeting j end, reuse the room */
                j++; /* increament to the next end */
            }
        }

        return rooms;
    }
};
```

### 646. Maximum Length of Pair Chain

* sort and use the idea similar to merge intervals, here we also use greedy.
* The key is sort by the end. The greedy is that we always prefer to choose
  `pairA` against `pairB` because `pairA[1] < pairB[1]`.

=== "Python interval"

    ```python
    class Solution:
        def findLongestChain(self, pairs: List[List[int]]) -> int:
            n = len(pairs)
            pairs.sort(key=lambda x: x[1]) # sort by end
            prev = pairs[0][1]

            count = 1
            for p in pairs:
                if p[0] > prev:
                    count += 1
                    prev = p[1]

            return count
    ```

### 729. My Calendar I

Solution 1 Check intervals (vector) overlapping

1. Consider the 3 different overlapping cases. Can be summarized using `max(start1, start2) < min(end1, end2)`.

```C++
class MyCalendar {
    vector<pair<int, int>> calendar;
public:
    MyCalendar() {

    }

    bool book(int start, int end) {
        for (auto book : calendar) {
            if (max(book.first, start) < min(book.second, end))
                return false;
            calendar.push_back({start, end});
        }

        return true;
    }
};
```

Solution 2 Using map to optimize to O(logn)

```C++
class MyCalendar {
    map<int, int> calendar;
public:
    MyCalendar() {

    }

    bool book(int start, int end) {
        auto it = calendar.lower_bound(start);
        if (it != calendar.end() && it->first < end) return false;
        if (it != calendar.begin() && prev(it)->second > start) return false;

        calendar[start] = end;

        return true;
    }
};
```

### 731. My Calendar II

Solution 1 reuse object and keep a copy of the intervals in it

* Notice we can reuse the class `MyCalendar` from the problem My Calendar I.
* Each interval booked also keeped in another `MyCalendar` objects. For each new
  booking, we first see whether the new booking overlaps with existing one, if
  so we get the overlap as a new interval and try to book the `MyCalendar`
  project which keep the same intervals.

```C++
class MyCalendar {
    vector<pair<int, int>> calendar;
public:
    MyCalendar() {

    }

    bool book(int start, int end) {
        for (auto book : calendar) {
            if (max(book.first, start) < min(book.second, end))
                return false;
        }

        calendar.push_back({start, end});

        return true;
    }
};

class MyCalendarTwo {
    vector<pair<int, int>> calendar;
public:
    MyCalendarTwo() {

    }

    bool book(int start, int end) {
        MyCalendar overlaps;
        for (auto book : calendar) {
            if (max(book.first, start) < min(book.second, end)) {
                pair<int, int> overlap = getOverlap(book.first, book.second, start, end);
                if (!overlaps.book(overlap.first, overlap.second)) return false;
            }
        }
        calendar.push_back({start, end});

        return true;
    }

    pair<int, int> getOverlap(int a0, int a1, int b0, int b1) {
        return {max(a0, b0), min(a1, b1)};
    }
};
```

Solution 2 Sweep line like Number of Air Plans in the Sky

* Notice this in this problem we can use `map` or `set` because they will not
  affect the count. If there is duplicates intervals, the map::second will increase accordingly.
* Compare this to the Number of Air Plans in the Sky problem to see why that one
  cannot use `map` or `set`.
* Notice the schedule in this calendar object is lost, we can only use it to
  check whether there are triple booking.

```C++
class MyCalendarTwo {
    map<int, int> calendar;
public:
    MyCalendarTwo() {

    }

    bool book(int start, int end) {
        calendar[start]++;
        calendar[end]--;

        int count = 0;
        for (auto& c : calendar) {
            count += c.second;
            if (count == 3) {
                calendar[start]--;
                calendar[end]++;
                return false;
            }
        }

        return true;
    }
};
```

### 732. My Calendar III

Solution 1 Sweep line solution

* Remember to consider whether duplicates schedule exists or not and how it
  affect the solution.

```C++
class MyCalendarThree {
    map<int, int> calendar;
public:
    MyCalendarThree() {

    }

    int book(int start, int end) {
        calendar[start]++;
        calendar[end]--;

        int k = 0, count = 0;
        for (const auto& m : calendar) {
            k = max(k, count += m.second);
        }

        return k;
    }
};
```

Solution 2 Fancy iterator

* We use the `map::emplace` return a pair of `<iterator, bool>` to indicate it
  inserted successfully or not

```C++
class MyCalendarThree {
    map<int, int> calendar;
    int k;
public:
    MyCalendarThree(): k(0) {

    }

    map<int, int>::iterator insert(int t) {
        pair<map<int, int>::iterator, bool> iter_bool = calendar.emplace(t, 0);
        if (iter_bool.second && iter_bool.first != calendar.begin())
            iter_bool.first->second = prev(iter_bool.first)->second;

        return iter_bool.first;
    }

    int book(int start, int end) {

        for (auto i = insert(start), j = insert(end); i != j; ++i) {
            k = max(k, ++(i->second));
        }

        return k;
    }
};
```

### 715. Range Module

Solution 1 Using vector to stroe the disjoint intervals

```C++
class RangeModule {
public:
    RangeModule() {

    }

    void addRange(int left, int right) {
        vector<pair<int, int>> tmp;
        int pos = 0;
        for (int i = 0; i < intervals.size(); i++) {
            if (left > intervals[i].second) {
                tmp.push_back(intervals[i]);
                pos++;
            } else if (right < intervals[i].first) {
                tmp.push_back(intervals[i]);
            } else {
                left = min(intervals[i].first, left);
                right = max(intervals[i].second, right);
            }
        }

        tmp.insert(tmp.begin() + pos, {left, right});
        swap(intervals, tmp);
    }

    bool queryRange(int left, int right) {
        int n = intervals.size();
        int l = 0;
        int r = n;

        while (l < r) {
            int m = l + (r - l) / 2;
            if (intervals[m].first <= left) {
                l = m + 1;
            } else {
                r = m;
            }
        }

        if (l == 0 || intervals[--l].second < right) {
            return false;
        }

        return true;
    }

    void removeRange(int left, int right) {
        int n = intervals.size();
        vector<pair<int, int>> tmp;
        for (int i = 0; i < n; i++) {
            if (intervals[i].second <= left || intervals[i].first >= right) {
                tmp.push_back(intervals[i]);
            } else {
                if (intervals[i].first < left) {
                    tmp.push_back({intervals[i].first, left});
                }

                if (intervals[i].second > right) {
                    tmp.push_back({right, intervals[i].second});
                }
            }
        }

        swap(intervals, tmp);
    }
private:
    vector<pair<int, int>> intervals;
};

/**
 * Your RangeModule object will be instantiated and called as such:
 * RangeModule* obj = new RangeModule();
 * obj->addRange(left,right);
 * bool param_2 = obj->queryRange(left,right);
 * obj->removeRange(left,right);
 */
```

Solution 2 Using map to stroe the disjoint intervals

```C++
class RangeModule {
public:
    RangeModule() {}
/*
1_3, 5___8, 12___15
*/
    void addRange(int left, int right) {
        auto l = m.upper_bound(left);
        auto r = m.upper_bound(right);
        // rule out the case that l overlap with previous
        if (l != m.begin()) {
            if((--l)->second < left) {
                ++l;
            }
        }

        // sure left is non-overlap, check the right
        if (l != r) {
            int lmin = min(l->first, left);
            int rmax = max(right, (--r)->second);
            m.erase(l, ++r); // remove iterator
            m[lmin] = rmax;
        } else { // both no overlap
            m[left] = right;
        }
    }

    bool queryRange(int left, int right) {
        auto l = m.upper_bound(left);

        if (l == m.begin() || (--l)->second < right) {
            return false;
        }

        return true;
    }
    /*
        l     r
    1_3, 5__8, 12_______20, 22____25
        4_6
        4____9
        4___________15 <- impossible
    2__4
    */
    void removeRange(int left, int right) {
        auto l = m.upper_bound(left);
        auto r = m.upper_bound(right);

        if (l != m.begin()) {
            --l;
            if(l->second < left) {
                ++l;
            }
        }

        // nothing need to be removed
        if (l == r) {
            return;
        }

        int l1 = min(left, l->first);
        int r1 = max(right, (--r)->second);

        m.erase(l, ++r);

        if (l1 != left) {
            m[l1] = left;
        }

        if (r1 != right) {
            m[right] = r1;
        }
    }

private:
    map<int, int> m;
};
```

### 436. Find Right Interval

Solution 1 using map

```C++
class Solution {
public:
    vector<int> findRightInterval(vector<Interval>& intervals) {
        int n = intervals.size();
        map<int, int> m;
        for (int i = 0; i < n; ++i) {
            m[intervals[i].start] = i;
        }

        vector<int> res;
        for (auto interval : intervals) {
            auto idx = m.lower_bound(interval.end);
            if (idx != m.end()) {
                res.push_back(idx->second);
            } else {
                res.push_back(-1);
            }
        }

        return res;
    }
};
```

### 435. Non-overlapping Intervals

Solution 1 Greedy

1. Iterate through the intervals, remove the one with larger end.

```C++
class Solution {
public:
    int eraseOverlapIntervals(vector<Interval>& intervals) {
        int n = intervals.size();
        int res = 0;
        sort(intervals.begin(), intervals.end(),
             [](const Interval& a, const Interval& b){
                return a.start < b.start;
             });

        int prev = 0;
        for (int i = 1; i < n; ++i) {
            if (intervals[prev].end > intervals[i].start) {
                if (intervals[prev].end > intervals[i].end) {
                    prev = i;
                }
                res++;
            } else { // end <= start no overlap
                prev = i;
            }
        }

        return res;
    }
};
```

Solution 2

```C++
class Solution {
public:
    int eraseOverlapIntervals(vector<Interval>& intervals) {
        int n = intervals.size();
        if (n == 0) return 0;

        sort(intervals.begin(), intervals.end(),
             [](const Interval& a, const Interval& b){
                 return a.end < b.end;
             });

        int res = 0;
        int end = INT_MIN;
        for (int i = 0; i < n; ++i) {
            if (intervals[i].start < end) {
                res++;
            } else {
                end = intervals[i].end;
            }
        }

        return res;
    }
};
```

### 352. Data Stream as Disjoint Intervals

Solution 1 Use vector as the container

* We take the stream and store them in a vector in increasing order based on the
  start. Eeach new number will find its insertion position using `lower_bound`.
* With the iterator, we look back to see whether the number can merge with an
  interval whose `start` is smaller. If so, the iterator walk back on step.

```C++
class SummaryRanges {
    vector<Interval> intervals;
public:
    /** Initialize your data structure here. */
    SummaryRanges() {

    }

    void addNum(int val) {
        Interval curr(val, val);
        int start = val, end = val;
        auto iter = lower_bound(intervals.begin(), intervals.end(), curr,
                              [](const Interval& a, const Interval& b){
                                  return a.start < b.start;
                              });

        if (iter != intervals.begin() && (iter - 1)->end + 1 >= val) --iter;
        while (iter != intervals.end() && iter->end + 1 >= val && val + 1 >= iter->start) {
            start = min(start, iter->start);
            end = max(end, iter->end);
            iter = intervals.erase(iter); // invalided previous iterator, return next iter
        }

        intervals.insert(iter, Interval(start, end));
    }

    vector<Interval> getIntervals() {
        return intervals;
    }
};
```

Solution 2 Use map or set as the container

```C++
class SummaryRanges {
    struct cmp {
        bool operator()(const Interval& a, const Interval& b){
            return a.start < b.start;
        }
    };

    set<Interval, cmp> intervals;
public:
    /** Initialize your data structure here. */
    SummaryRanges() {

    }

    void addNum(int val) {
        Interval curr(val, val);
        int start = val, end = val;
        auto iter = intervals.lower_bound(curr);

        if (iter != intervals.begin() && (--iter)->end + 1 < val) ++iter;
        while (iter != intervals.end() && iter->end + 1 >= val && val + 1 >= iter->start) {
            start = min(start, iter->start);
            end = max(end, iter->end);
            iter = intervals.erase(iter); // invalided previous iterator, return next iter
        }

        intervals.insert(iter, Interval(start, end));
    }

    vector<Interval> getIntervals() {
        vector<Interval> res;
        for (auto& interval : intervals)
            res.push_back(interval);
        return res;
    }
};
```

Solution 3 Use the insert interval subroutine.

```C++
```

### 915. Partition Array into Disjoint Intervals

### 163. Missing Ranges

Solution 1

* This problem show us the importance of defining the loop invariant. If we use
  The one ahead of the first variable `prev` to keep the loop invariant, it saved
  many code to handle the coner cases.
* Another trick is factor out the logic to generate the final result in to a funciton.

=== "C++ succient solution"

    ```C++
    class Solution {
    public:
        vector<string> findMissingRanges(vector<int>& nums, int lower, int upper) {
            int n = nums.size();
            vector<string> res;
            long prev = 0;
            long curr = 0;

            prev = (long)lower - 1;
            for (int i = 0; i < n; i++) {
                curr = nums[i];
                //curr = i == n ? (long)upper + 1 : nums[i];
                if (curr - prev > 1) {
                    res.push_back(getRange(prev + 1, curr - 1));
                }

                prev = curr;
            }
            /* the case can be included in the above case by changing line 10 and 11 */
            if (upper > prev) {
                res.push_back(getRange(prev + 1, upper));
            }

            return res;
        }

        string getRange(int a, int b) {
            return a == b ? to_string(a) : to_string(a) + "->" + to_string(b);
        }
    };
    ```

=== "C++ lengthy solution"

    ```c++
    class Solution {
    public:
        vector<string> findMissingRanges(vector<int>& nums, int lower, int upper) {
            vector<string> res;

            if (nums.size() == 0) {
                if (lower == upper)
                    res.push_back(to_string(lower));
                else
                    res.push_back(to_string(lower) + "->" + to_string(upper));

                return res;
            }

            long s = nums[0];
            if (lower < s - 1) {
                res.push_back(to_string(lower) + "->" + to_string(s - 1));
            } else if (lower < s) {
                res.push_back(to_string(lower));
            }

            for (int i = 0; i < nums.size() - 1; i++) {
                long a = nums[i];
                long b = nums[i + 1];
                if (a == b - 1) {
                    continue;
                } else if (a == b - 2) {
                    res.push_back(to_string(a + 1));
                } else if (a < b - 2) {
                    res.push_back(to_string(a + 1) + "->" + to_string(b - 1));
                }
            }

            long e = nums[nums.size() - 1];
            if (e < upper - 1) {
                res.push_back(to_string(e + 1) + "->" + to_string(upper));
            } else if (e < upper) {
                res.push_back(to_string(upper));
            }

            return res;
        }
    };
    ```

### 228. Summary Ranges

* Pay attention to the integer overflow when you test whether one number is
  greater that another. `[-2147483648,-2147483647,2147483647]`
* Another thing is that `to_string()` can also change long and float number to string.

```C++
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        int n = nums.size();
        vector<string> res;
        if (n == 0) {
            return res;
        }

        long start = nums[0];
        long prev = nums[0];
        for (int i = 1; i < n; i++) {
            if ((long)nums[i] - prev > 1) {
                string s = start < prev ? to_string((int)start) + "->" + to_string((int)prev) : to_string((int)start);
                res.push_back(s);
                start = nums[i];
            }

            prev = nums[i];
        }

        res.push_back(start < prev ? to_string((int)start) + "->" + to_string((int)prev) : to_string((int)start));

        return res;
    }
};
```

### 938. Range Sum of BST

### 370. Range Addition

### 597. Range Addition II
