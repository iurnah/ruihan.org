# Hash Table

### Isomorphic Strings

=== "Python Two Hash Solution"

    ```python
    class Solution:
        def isIsomorphic(self, s: str, t: str) -> bool:
            if len(s) != len(t): return False;
            sdict = {}
            tdict = {}
            for a, b in zip(s, t):
                if a not in sdict:
                    if b in tdict: return False
                    else:
                        sdict[a] = b
                        tdict[b] = a
                elif sdict[a] != b:
                    return False

            return True
    ```

=== "Python Two Hash Solution"

    ```C++
    class Solution {
    public:
        bool isIsomorphic(string s, string t) {
            if (s.length() != t.length())
                return false;
            vector<int> m1(256, -1);
            vector<int> m2(256, -1);
            for (int i = 0; i < s.length(); i++) {
                if (m1[s[i]] != m2[t[i]])
                    return false;
                m1[s[i]] = i;
                m2[t[i]] = i;
            }

            return true;
        }
    };
    ```
