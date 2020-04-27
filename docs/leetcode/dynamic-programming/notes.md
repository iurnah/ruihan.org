# Dynamic Programming

## 难点

1. 如何在分析最后一步时选择正确角度并考虑所有可能情况，进而写出状态转化方程.
   i.e. Buy and Shell stock, Edit distance, Longest Increasing Subsequence.

### Race Car

waymo

## 坐标型

### Triangle

### [Unique Paths](../../../courses/9chap-dynamic-prog/notes/#unique-paths)

### [Unique Paths II](../../../courses/9chap-dynamic-prog/notes/#unique-paths-ii)

### [Minimum Path Sum](../../../courses/9chap-dynamic-prog/notes/#minimum-path-sum)

### [Bomb Enemy](../../../courses/9chap-dynamic-prog/notes/#bomb-enemy)

### Dungeon Game

## 序列型

### Perfect Squares

### [Longest Increasing Subsequence](#longest-increasing-subsequence)

### Number of Longest Increasing Subsequence

### [Longest Increasing Continuous Subsequence](#longest-increasing-continuous-subsequence)

## 序列型 + 状态

### [Paint House](../../../courses/9chap-dynamic-prog/notes/#paint-house)

### [Paint House II](../../../courses/9chap-dynamic-prog/notes/#paint-house-ii)

### [House Robber](../../../courses/9chap-dynamic-prog/notes/#house-robber)

### [House Robber II](../../../courses/9chap-dynamic-prog/notes/#house-robber-ii)

### [Best Time to Buy and Sell Stock](../../../courses/9chap-dynamic-prog/notes/#best-time-to-buy-and-sell-stock)

### [Best Time to Buy and Sell Stock II](../../../courses/9chap-dynamic-prog/notes/#best-time-to-buy-and-sell-stock-ii)

### [Best Time to Buy and Sell Stock III](../../../courses/9chap-dynamic-prog/notes/#best-time-to-buy-and-sell-stock-iii)

### [Best Time to Buy and Sell Stock IV](../../../courses/9chap-dynamic-prog/notes/#best-time-to-buy-and-sell-stock-iv)

## 划分型

### Word Break

Solution 1 use set dictionary O(n^2), iterate forwards

```C++
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int m = s.length();
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());

        bool f[m + 1] = {0};
        f[0] = true;

        for (int i = 1; i <= m; i++) {
            for (int j = 0; j <= i; j++) {
                if (f[j] && wordSet.count(s.substr(j, i - j)) != 0) {
                    f[i] = true;
                    break;
                }
            }
        }

        return f[m];
    }
};
```

Solution 2 use set dictionary O(n^2), iterate backwards

```C++
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int n = s.length();
        unordered_set<string> dict(wordDict.begin(), wordDict.end());

        int f[n + 1] = {0};
        f[0] = 1;

        for (int i = 0; i <= n; ++i) {
            for (int j = n - 1; j >= 0; --j) {
                if (f[j] && dict.count(s.substr(j, i - j))) {
                    f[i] = 1;
                    break;
                }
            }
        }

        return f[n];
    }
};
```

Solution 3 (best solution) use vector, iterate through string

- Do not use word set to check exist or not, use each word as the last step.

```C++
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int n = s.length();

        int f[n + 1] = {0};
        f[0] = 1;

        for (int i = 0; i <= n; ++i) {
            for (string word : wordDict) {
                if (word.length() <= i && f[i - word.length()]) {
                    if (s.substr(i - word.length(), word.length()) == word) {
                        f[i] = 1;
                        break;
                    }
                }
            }
        }

        return f[n];
    }
};
```

### Maximum Vacation Days

Solution 1

```c++
class Solution {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size();
        int k = days[0].size();

        vector<vector<int>> f(n, vector<int>(k, -1));
        f[0][0] = days[0][0];

        for (int i = 1; i < n; ++i) {
            if (flights[0][i]) {
                f[i][0] = days[i][0];
            }
        }

        for (int d = 1; d < k; ++d) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if ((flights[j][i] || i == j) && f[j][d - 1] != 1) {
                        f[i][d] = max(f[i][d], f[j][d - 1] + days[i][d]);
                    }
                }
            }
        }

        int result = 0;
        for (int i = 0; i < n; ++i) {
            result = max(result, f[i][k - 1]);
        }

        return result;
    }
};
```

Solution 2 DFS with Cache

```C++
class Solution {
    vector<vector<int>> memo;
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        vector<vector<int>> memo(flights.size(), vector<int>(days[0].size(), INT_MIN));

        return dfs(flights, days, 0, 0, memo);
    }
    int dfs(vector<vector<int>>& flights, vector<vector<int>>& days, int start, int day, vector<vector<int>>& memo) {
        if (day == days[0].size()) {
            return 0;
        }
        if (memo[start][day] != INT_MIN) {
            return memo[start][day];
        }

        int maxVal = 0;
        for (int i = 0; i < flights.size(); ++i) {
            if (flights[start][i] || start == i) {
                maxVal = max(maxVal, days[i][day] + dfs(flights, days, i, day + 1, memo));
            }
        }
        memo[start][day] = maxVal;
        return maxVal;
    }
};
```

Solution 3 (greedy, wrong)

```C++
class Solution {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size();
        int k = days[0].size();
        int start = 0;
        int result = 0;
        set<pair<int, int>, std::greater<pair<int, int>>> s;

        for (int col = 0; col < k; ++col) {
            for (int row = 0; row < n; ++row) {
                s.insert({days[row][col], row});
            }
            int stay = 1;
            for (auto p : s) {
                if (flights[start][p.second]) {
                    result += p.first;
                    cout << p.first << " " << p.second << endl;
                    start = p.second;
                    stay = 0;
                    break;
                }
            }
            if (stay) {
                result += days[start][col];
            }
            s.clear();
        }

        return result;
    }
};
```

### [Decode Ways](../../../courses/9chap-dynamic-prog/notes/#decode-ways)

### Decode Ways II

Solution 1

```C++
class Solution {
public:
    int numDecodings(string s) {
        const int MOD = 1000000007;
        int n = s.length();
        long long f[n + 1] = {0};
        if (n == 0) return 0;

        f[0] = 1;
        for (int i = 1; i <= n; ++i) {
            if (s[i - 1] == '*') { // not include '0' according to the problem statement
                f[i] = (f[i] + f[i - 1] * 9) % MOD; // number of ways only decode one digit
                if (i > 1) { // number of ways decode two digits
                    if (s[i - 2] == '*') {
                        f[i] = (f[i] + f[i - 2] * 15) % MOD; // 11 - 26
                    } else if (s[i - 2] == '1') {
                        f[i] = (f[i] + f[i - 2] * 9) % MOD; // 11 - 19
                    } else if (s[i - 2] == '2') {
                        f[i] = (f[i] + f[i - 2] * 6) % MOD; // 21 - 26
                    }
                }
            } else { // s[i - 1] != '*', have to consider the case '0'
                if (s[i - 1] != '0') { // number of ways only decode one digit
                    f[i] = (f[i] + f[i - 1]) % MOD;
                }
                if (i > 1) { // number of ways decode two digits
                    if (s[i - 2] == '*') {
                        if (s[i - 1] <= '6') { // '*' can represent 1, 2
                            f[i] = (f[i] + f[i - 2] * 2) % MOD;
                        } else { // '*' can only represent 1
                            f[i] = (f[i] + f[i - 2]) % MOD;
                        }
                    } else { // no '*' case
                        int t = (s[i - 2] - '0') * 10 + s[i - 1] - '0';
                        if (t >= 10 && t <= 26) {
                            f[i] = (f[i] + f[i - 2]) % MOD;
                        }
                    }
                }
            }
        }

        return f[n] % MOD;
    }
};
```

## 双序列型

### Longest Common Subsequence

Solution 1

- consider the last step, the case is NOT `A[n-1] == B[l-1]` and `A[n-1] != B[m-1]`
  but, three cases: `A[n-1]` is included, `B[m-1]` is included, both `A[n-1]` and
  `B[m-1]` are included, `(A[n-1] == B[m-1])`.

```C++

```

### Interleaving String

Solution 1

```C++
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        // f[i][j] = OR(f[i][j - 1]|B[j - 1] in C, f[i - 1][j]|A[i - 1] in C);
        int m = s1.length();
        int n = s2.length();
        int l = s3.length();
        if (m + n != l)
            return false;

        int f[m + 1][n + 1] = {0};
        f[0][0] = 1;
        for (int i = 0; i <= m; ++i) {
            for (int j = 0; j <= n; ++j) {
                if (i > 0 && s1[i - 1] == s3[i + j - 1]) {
                    f[i][j] |= f[i - 1][j];
                }
                if (j > 0 && s2[j - 1] == s3[i + j - 1]) {
                    f[i][j] |= f[i][j - 1];
                }
            }
        }

        return f[m][n];
    }
};
```

### Edit Distance

Solution 1

- state: `f[i][j]` is min edit distance to make the first i chars of A the same
  as first `j` chars of B, as we consider the last operation (insert, delete,
  replace). One key idea of this problem is after the operation, the length of
  the A and B are not necessarily `i` and `j`. The focus here is the "min edit
  distance", not the length of A or B. For example, if the last step is insert,
  A's length will become `i + 1`, B's length will maintain `j`. You should also
  note that the A, B concept is imaginary, B is actually changed from A by
  editing once. As a result, we know `i + 1 == j` after the insertion operation.
  Don't iterpret the state `f[i][j]` as min edit distance of first `i` chars in
  A and first `j` chars in B and after the editing, A's length is `i` and B's
  length is `j`. You should seperate the two intepretations.

```C++
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length();
        int n = word2.length();

        int f[m + 1][n + 1];
        int i, j;

        for (i = 0; i <= m; i++) {
            for (j = 0; j <=n; j++) {
                if (i == 0) {
                    f[i][j] = j;
                    continue;
                }
                if (j == 0) {
                    f[i][j] = i;
                    continue;
                }

                           // delete           insert
                f[i][j] = min(f[i - 1][j] + 1, f[i][j - 1] + 1);
                                        // replace
                f[i][j] = min(f[i][j], f[i - 1][j - 1] + 1);

                if (word1[i - 1] == word2[j - 1]) {
                    f[i][j] = min(f[i][j], f[i - 1][j - 1]);
                }
            }
        }

        return f[m][n];

    }
};
```

Solution 2 Space optimized

```C++
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length();
        int n = word2.length();

        int f[2][n + 1];
        int prev = 0, curr = 0;
        for (int i = 0; i <= m; i++) {
            prev = curr;
            curr = 1 - curr;
            for (int j = 0; j <=n; j++) {
                if (i == 0) {
                    f[curr][j] = j;
                    continue;
                }
                if (j == 0) {
                    f[curr][j] = i;
                    continue;
                }
                          // delete               insert
                f[curr][j] = min(f[prev][j] + 1, f[curr][j - 1] + 1);
                                             // replace
                f[curr][j] = min(f[curr][j], f[prev][j - 1] + 1);

                if (word1[i - 1] == word2[j - 1]) {
                    f[curr][j] = min(f[curr][j], f[prev][j - 1]);
                }
            }
        }

        return f[curr][n];
    }
};
```

## Memoization

### Sentence Screen Fitting

Solution 1

- You can view the sentence as a space seperated English sentence and then use
  each row to "frame" the sentence. If the end of the frame overlap a space, we
  continue move to the next frame if available. If the end of the frame overlap
  a character, we should move the right the frame to the end of the prev word
  and move on to the next frame.

```C++
class Solution {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        int n = sentence.size();
        string s;
        for (string w : sentence) {
            s += string(w + " ");
        }

        int start = 0, len = s.length(); //s with an extra trailing space.
        for (int i = 0; i < rows; i++) {
            start += cols;
            if (s[start % len] == ' ') {
                ++start;
            } else {
                while (start > 0 && s[(start - 1) % len] != ' ') {
                    --start;
                }
            }
        }

        return start / len;
    }
};
```

Solution 2

- One insight of this solution is that every word in the sentence is possible to
  start a new row, but not necessary. For example, a long row can contain
  multiple of the setence, but the last word cannot fit in to this sigle row, it
  start a new row. In this case only the first word in the sentence and the last
  word can start a new row. We use this insight in the following way, for each
  word, we assume it could start a new row, then greedyly use the following
  words in the sentence to fix the row, we record the total number of words can
  fit this row in dp[i], i is the index of the starting word in the sentence.
  after the calculation, we could count sum(dp[i]), i is starting word's index
  
```C++
class Solution {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        int n = sentence.size();
        int f[n] = {0};

        for (int i = 0; i < n; ++i) {
            int len = 0, count = 0, idx = i;
            while (len + sentence[idx % n].length() <= cols) {
                len += (sentence[idx % n].length() + 1);
                idx++;
                count++;
            }
            f[i] = count;
        }

        int total = 0;
        for (int i = 0, idx = 0; i < rows; ++i) {
            total += f[idx];
            idx = (f[idx] + idx) % n;
        }


        return total / n;
    }
};
```

TODO:
This solution may be further improved because for some of the count, we never
used. Can we calculate the final result by only one loop, or in a nested loop.
