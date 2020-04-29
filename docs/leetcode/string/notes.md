# String Problems

!!! Warning "Understand the problem by listing all the test cases first"
    One of the most important principle to solve a string proble during an
    interview is to first make sure you have all the possible test cases laid out.

## Loop invariance

To keep loop invariance, you have to be really clear about the different type of
chars in the string that may affect the complexity of the loop. The following
problems can be tricky to implement when you keep the wrong loop invariance or
have identified wrong if condition or while conditions.

### [Decode String](https://leetcode.com/problems/decode-string/)

* Iterative solution v.s. Recursive solution
* Make sure you have all the test cases first
* Consider the special cases. i.e. nested brackets `3[a]2[a3[bc]]`, and unbracketed
  chars `3[a]abcd` and `3[a]2[a3[bc]aa]`
* This solution requires a lot of attention to details. You should draw the
  variable and exercise it using example. Use loop invariance and only process one char at a time.
* We can also use the divide and conquer idea and using recursive helper function
  to solve the problem. The key to implement this solution is to keep the loop invariance in mind.

```C++ tab="Iterative solution"
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

```C++ tab="Recursive solution"
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

### Longest Absolute File Path

### [Maximum Score After Splitting a String](https://leetcode.com/problems/maximum-score-after-splitting-a-string/)

* The steps to solve this type of problem is
    1. enumerate all the possible test cases
    2. write a naive solution to cover all test cases
    3. refactor to improve the complexity

```C++ tab="Two pass"
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

```C++ tab="One pass"
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
