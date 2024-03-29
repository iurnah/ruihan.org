# String Problems

!!! Warning "Understand the problem by listing all the test cases first"
    One of the most important principle to solve a string problem during an
    interview is to first make sure you have all the possible test cases laid out.

## Loop invariance

To keep loop invariance, you have to be really clear about the different type of
chars in the string that may affect the complexity of the loop. The following
problems can be tricky to implement when you keep the wrong loop invariance or
have identified the wrong `if` condition or `while` condition.

### 157. Read N Characters Given Read4

Solution 1

=== "C++ loop invariance"

    ```c++
    /**
    * The read4 API is defined in the parent class Reader4.
    *     int read4(char *buf4);
    */

    class Solution {
    public:
        /**
        * @param buf Destination buffer
        * @param n   Number of characters to read
        * @return    The number of actual characters read
        */
        int read(char *buf, int n) {
            int total = 0;
            int len = 0;
            while (total < n) {
                len = read4(buf + total);
                if (len == 0) {
                    break;
                }

                total += len;

                if (total > n) {
                    total = n;
                }
            }

            return total;
        }
    };
    ```

### 158. Read N Characters Given Read4 II - Call multiple times

=== "C++ one byte by one byte"

```c++
/**
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf);
 */

class Solution {
    char buf4[4];
    int pos = 0, len = 0;
public:
    /**
     * @param buf Destination buffer
     * @param n   Number of characters to read
     * @return    The number of actual characters read
     */
    int read(char *buf, int n) {
        for (int i = 0; i < n; i++) {
            if (pos == len) {
                len = read4(buf4);
                pos = 0;
            }

            if (len == 0) {
                return i;
            }

            if (pos < len) {
                buf[i] = buf4[pos++];
            }
        }

        return n;
    }
};
```

### 394. Decode String

* Iterative solution v.s. Recursive solution
* Make sure you have all the test cases first
* Consider the special cases. i.e. nested brackets `3[a]2[a3[bc]]`, and unbracketed
  chars `3[a]abcd` and `3[a]2[a3[bc]aa]`
* This solution requires a lot of attention to details. You should draw the
  variable and exercise it using example. Use loop invariance and only process one char at a time.
* We can also use the divide and conquer idea and using recursive helper function
  to solve the problem. The key to implement this solution is to keep the loop invariance in mind.

=== "C++ Iterative solution"

    ```c++
    class Solution {
    public:
        string decodeString(string s) {
            int n = s.size();
            stack<int> stk_cnt;
            stack<string> stk_str;
            int cnt = 0;
            string tmp_str = "";

            for (int i = 0; i < n; i++) {
                if (isdigit(s[i])) {
                    cnt = cnt * 10 + s[i] - '0';
                } else if (s[i] == '[') {
                    stk_cnt.push(cnt);
                    stk_str.push(tmp_str);
                    cnt = 0;
                    tmp_str.clear();
                } else if (s[i] == ']') {
                    // output, or modify stacks
                    int c = stk_cnt.top();
                    stk_cnt.pop();
                    for (int i = 0; i < c; i++) {
                        stk_str.top() += tmp_str;
                    }
                    tmp_str = stk_str.top();
                    stk_str.pop();
                } else {
                    tmp_str += s[i];
                }
            }

            return stk_str.empty() ? tmp_str : stk_str.top();
        }
    };
    ```

=== "C++ Recursive solution"

    ```c++
    class Solution {

    public:
        string decodeString(string s) {
            int i = 0;
            return decode_helper(s, i);
        }

        string decode_helper(string s, int& i) {
            int n = s.size();
            string res = "";

            while (i < n && s[i] != ']') {
                if (s[i] < '0' || s[i] > '9') {
                //if (isalpha(s[i])) {
                    res += s[i++];
                } else {
                    int cnt = 0;
                    //while (i < n && isdigit(s[i])) {
                    while (i < n && s[i] >= '0' && s[i] <= '9') {
                        cnt = cnt * 10 + s[i++] - '0';
                    }
                    ++i; // '['
                    string t = decode_helper(s, i);
                    ++i;  //  ']'
                    while (cnt-- > 0) {
                        res += t;
                    }
                }
            }

            return res;
        }
    };
    ```

### 388. Longest Absolute File Path

* Use level to count the depth of the path. `res` variable to store the global
  longest path.
* Use special char `'\n'` to start (init level = 0) the count of the level and
  char `'\t'` to count the "level" of the directory.
* Use hash to record the lenght of each level of parent directory, once we reach
  that level, we can look up the directory path length from the hash.

```c++
class Solution {
public:
    int lengthLongestPath(string input) {
        int n = input.length();
        unordered_map<int, int> map;
        int res = 0;
        int level = 0;

        map[0] = 0;/* 1. should initialize hash map. */
        for (int i = 0; i < n; ++i) {
            int start = i;

            /* Skipping alphabets chars */
            while (i < n && input[i] != '\n' && input[i] != '\t')
                ++i;

            /* Directory or file name */
            if (i == n || input[i] == '\n') {
                string name = input.substr(start, i - start);
                if (name.find('.') == string::npos) {
                    level++;
                    map[level] = map[level - 1] + name.length() + 1;
                } else {
                    res = max(res, map[level] + (int)name.length());
                }
                /* level start from zero every time we start a new line */
                level = 0;
            } else { /* when we have '\t', level increase */
                level++;
            }
        }

        return res;
    }
};
```

### 1233. Remove Sub-Folders from the Filesystem

```c++
class Solution {
public:
    vector<string> removeSubfolders(vector<string>& folder) {
        int n = folder.size();
        vector<string> res;
        if (n == 0) {
            return res;
        }

        // sort
        sort(folder.begin(), folder.end(), less<string>());

        // remove
        string root = folder[0];
        res.push_back(folder[0]);
        for (int i = 1; i < n; i++) {
            // need to consider both "/a/b", "/a/bc"
            if (folder[i].substr(0, root.length()) != root ||
                folder[i][root.length()] != '/') {
                res.push_back(folder[i]);
                root = folder[i];
            }
        }

        return res;
    }
};;
```

### 1422. Maximum Score After Splitting a String

* The steps to solve this type of problem is
    1. enumerate all the possible test cases
    2. write a naive solution to cover all test cases
    3. refactor to improve the complexity

=== "C++ Two pass"

    ```c++
    class Solution {
    public:
        int maxScore(string s) {
            int len = s.size();
            if (len == 0) {
                return 0;
            }

            int ones = 0;
            int zeros = 0;
            int max_score = INT_MIN;
            for (int i = 0; i < len; i++) {
                if (s[i] - '0' == 1) ones++;
            }

            for (int i = 0; i < len - 1; i++) {
                if (s[i] - '0' == 0) zeros++;
                else ones--;
                max_score = max(max_score, zeros + ones);
            }

            return max_score;
        }
    };
    ```

=== "C++ One Pass"

    ```c++
    class Solution {
    public:
        int maxScore(string s) {
            int len = s.size();
            if (len == 0) {
                return 0;
            }

            int ones = 0;
            int zeros = 0;
            int max_score = INT_MIN;

            for (int i = 0; i < len; i++) {
                if (s[i] - '0' == 0) zeros++;
                else ones++;
                if (i < len - 1)
                    max_score = max(max_score, zeros - ones);
            }

            return max_score + ones;
        }
    };
    ```

## Substring search and substring window problem

This type of problems asking for a substring that fulfill certain properties.
The idea of a general solution is to keep a substring window by moving two
"pointers" and keep a loop invariance. This algorithm is linear. Remember the
following principles while solving the problem.

1. Using `while` loop is better.
2. Identify when to move the `left` pointer. Be clear about how the map is modified meanwhile.
3. Maintain the invariance: the map keeps the info about chars in `[left, i)`.

### 3. Longest Substring Without Repeating Characters

Solution 1 Loop invariance

* This solution is very neat that it uniformly take care of two cases: 1) first
  time discovered a char. 2) discovered a preated char, by cleverly set the
  initial value of the `dict` value to `-1` and `start = -1`;
* The idea can be described as follows: set the start points to current position
  if you found a duplicate(notice whe a char is first discovered, because we set
  `start = -1`, start will also update). Otherwise just record the current
  position in the dictionary and update the length.
* The `start = dict[s[i]]` maintained the invariance so that `i - start` will never be wrong.

=== "C++ One pass iteration"

    ```c++
    class Solution {
    public:
        int lengthOfLongestSubstring(string s) {
            vector<int> dict(256, -1);
            int maxLen = 0, start = -1;

            for (int i = 0; i < s.length(); ++i) {
                if (dict[s[i]] > start)
                    start = dict[s[i]];

                dict[s[i]] = i;
                maxLen = max(maxLen, i - start);
            }

            return maxLen;
        }
    };
    ```

=== "C++ Two pointer loop invariant"

    ```c++
    class Solution {
    public:
        int lengthOfLongestSubstring(string s) {
            unordered_map<char, int> mp;

            int left = 0;
            int count = 0;
            int res = 0;
            for (int i = 0; i < s.length(); i++) {
                m[s[i]]++;
                if (m[s[i]] > 1) count++;

                while (count > 0) {
                    m[s[left]]--;
                    if (m[s[left]] == 1) {
                        count--;
                    }
                    left++;
                }

                res = max(res, i - left + 1);
            }

            return res;
        }
    }
    ```

=== "C++ Two pointer loop invariant II"

    ```c++
    class Solution {
    public:
        int lengthOfLongestSubstring(string s) {
            if (s.length() == 0) return 0;

            int map[256] = {0};
            int res = INT_MIN;
            int j = 0;

            for (int i = 0; i < s.length(); i++) {
                while (j < s.length() && map[s[j]] == 0) {
                    map[s[j]]++;
                    res = max(res, j - i + 1);
                    j++;
                }

                map[s[i]]--;
            }

            return res;
        }
    };
    ```

### 159. Longest Substring with At Most Two Distinct Characters

* We can use a __hash table__ to record the chars we have seen, the key is the
  char, the value is the count of the char seen so far.
* Once we have more than two hash entries, we should stop looking further and
  think about removing the third char. So that we can count the desired length.
* We use a pointer `left` to keep the left boundary of the interested chars.

=== "C++ loop invariant with map"

    ```c++
    class Solution {
    public:
        int lengthOfLongestSubstringTwoDistinct(string s) {
            int max_len = 0;
            int left = 0;
            unordered_map<char, int> m;

            for (int i = 0; i < s.length(); i++) }{
                m[s[i]]++;
                while (m.size() > 2) {
                    if (--m[s[left]] == 0) m.erase(s[left]);
                    left++;
                }

                max_len = max(max_len, i - left + 1);
            }

            return max_len;
        }
    };
    ```

=== "C++ using char array as map"

    ```c++
    class Solution {
    public:
        int lengthOfLongestSubstringTwoDistinct(string s) {
            int max_len = 0;
            int left = 0;
            int cnt = 0;
            int m[128] = {0};

            for (int i = 0; i < s.length(); i++) {
                if (m[s[i]]++ == 0) cnt++;
                while (cnt > 2) {
                    if (--m[s[left++]] == 0) cnt--;
                }
                max_len = max(max_len, i - left + 1);
            }

            return max_len;
        }
    };
    ```

=== "C++ Two pointers"

    ```c++
    class Solution {
    public:
        int lengthOfLongestSubstringTwoDistinct(string s) {
            int max_len = 0;
            int cnt = 0;
            int m[128] = {0};

            for (int i = 0, j = 0; i < s.length(); i++) {
                while (j < s.length()) {
                    if (m[s[j]] == 0) {
                        cnt++;
                    }
                    m[s[j]]++;
                    /* loop invariance: maintain the hash only have 2 distinct chars */
                    while (cnt > 2) {
                        if (--m[s[i++]] == 0)
                            cnt--;
                    }
                    max_len = max(max_len, j - i + 1);
                    j++;
                }
            }

            return max_len;
        }
    };
    ```

### 340. Longest Substring with At Most K Distinct Characters

* This problem is essentially equivalent to the
  [Longest Substring with At Most Two Distinct Characters](#longest-substring-with-at-most-two-distinct-characters) problem.

```c++
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        int max_len = 0;
        int left = 0;
        int cnt = 0;
        int map[128] = {0};

        for (int i = 0; i < s.length(); i++) {
            if (map[s[i]] == 0)
                cnt++;
            map[s[i]]++;
            while (cnt > k) {
                if (--map[s[left++]] == 0)
                    cnt--;
            }
            max_len = max(max_len, i - left + 1);
        }

        return max_len;
    }
};
```

### Minimum Window Substring

### Substring with Concatenation of All Words

### Max Consecutive Ones II

### Longest Substring with At Least K Repeating Characters

### Longest Repeating Character Replacement

### Find All Anagrams in a String

## Word Abbreviation

### Unique Word Abbreviation

### Encode String with Shortest Length

### Generalized Abbreviation

### Valid Word Abbreviation

### Minimum Unique Word Abbreviation

### Word Abbreviation

### Encode and Decode Strings

## Palindrome

## Manacher's Algorithm

## Valid parenthesis

### Valid Parentheses

### Generate Parentheses

### Longest Valid Parentheses

### Different Ways to Add Parentheses

### Remove Invalid Parentheses

### Valid Parenthesis String

### Ternary Expression Parser

### Construct Binary Tree from String

### Construct String from Binary Tree

## Parse/generate recursive data

Also see [Nested list iteration] in stack category.

### Mini Parser

### Ternary Expression Parser

### Flatten Nested List Iterator

### Construct Binary Tree from String

### Construct String from Binary Tree

### Remove Comments

### Integer to English Words

### Scramble String

The recursive method is always preferred than the iterative method using a stack in solve those problems.

## KMP

## Rabin–Karp algorithm