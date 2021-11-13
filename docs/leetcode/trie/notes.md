# Trie

* To implement a trie, you should first have the mental model of a trie node.
  Especially how a char is stored in the node. It is not explicitly stored in
  a node, but use the index of the pointer array to represent char (by pointing
  the pointer to a node).
* For build and query the trie, you can either implement recursive or iterative
  solution.
* Notice the `deleteKey` might be the hardest API to implement and track the
  correctness.

## Trie APIs

=== "C++ Trie APIs"

    ```C++
    class Node {
    public:
        bool isEnd;
        int val;
        vector<Node*> next;

        Node () {
            isEnd = false;
            val = INT_MIN;
            next = vector<Node*>(256, nullptr);
        }
    };

    class Trie {
    public:
        Node* root;
        const int NUM_CHARS = 256;

        // recursive helper function to get all the keys with a common prefix
        void collectByPrefix(Node* node, string curr, vector<string>& keys);

        // recursive helper function to get all the keys that match a pattern
        void collectByPattern(Node* node, string curr, string pattern, vector<string>& keys);

        // recursive helper function to search the longest word in the trie that match target
        int search(Node* node, string target, int d, int length);

        // recursive helper function to detele a key and return the trie root
        Node* deleteKey(Node* node, string key, int d);

        Trie () {
            root = new Node();
        }

        ~Trie () {
            destroy(root);
        }

        Node* getRoot() { return root; }

        // delete and destroy a trie
        void destroy(Node* root);

        // insert a word iteratively
        void insert(string word, int val);

        // insert a word recursively
        Node* insert(Node* node, string word, int val, int d);

        // find the symbol value interatively, can return node by change the signature
        int search(string word);

        // find the symbol value recursively, can return node by change the signature
        int search(Node* node, string word, int d);

        // exists a key start with a prefix
        bool existsKeyStartsWith(string prefix);

        // get all the keys have a common prefix
        vector<string> keyStartWith(string prefix);

        // get all the keys that match a pattern (support wildcard '.')
        vector<string> keysThatMatch(string pattern);

        // get the longest key that matches the prefix of the target
        string keyWithLongestMatch(string target);

        // delete a key
        void deleteKey(string key);
    };
    ```

!!! Note
    The implementation can be found at [here](./src/trie.cc)

## Problems

### 208. Implement Trie (Prefix Tree)

```C++
class Node {
public:
    bool isEnd;
    vector<Node*> next;
    Node () {
        next = vector<Node*>(26, nullptr);
        isEnd = false;
    }
};


class Trie {
    Node* root;
public:
    /** Initialize your data structure here. */
    Trie() {
        root = new Node();
    }

    ~Trie() {
        destroy(root);
    }

    void destroy(Node* root) {
        for (auto node : root->next) {
            if (node) {
                destroy(node);
            }
        }

        delete root;
    }

    /** Inserts a word into the trie. */
    void insert(string word) {
        Node* p = root;
        for (char c : word) {
            if (p->next[c - 'a'] == nullptr) {
                p->next[c - 'a'] = new Node();
            }
            p = p->next[c - 'a'];
        }

        p->isEnd = true;
    }

    /** Returns if the word is in the trie. */
    bool search(string word) {
        Node* p = root;
        for (char c : word) {
            if (p != nullptr) {
                p = p->next[c - 'a'];
            } else {
                return false;
            }
        }

        return p != nullptr && p->isEnd;
    }

    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        Node* p = root;
        for (char c : prefix) {
            if (p != nullptr) {
                p = p->next[c - 'a'];
            }
        }

        return p != nullptr;
    }
};
/**
 * Your Trie object will be instantiated and called as such:
 * Trie obj = new Trie();
 * obj.insert(word);
 * bool param_2 = obj.search(word);
 * bool param_3 = obj.startsWith(prefix);
 */
```

### 212. Word Search II

Solution 1 search the word square against the trie

* mark visited to avoid revisit.
* Use a set to remove duplicate word.
* need destructor for the trie.

```C++
class TrieNode {
public:
    bool isWord;
    vector<TrieNode *> next;
    TrieNode () {
        isWord = false;
        next = vector<TrieNode*>(26, nullptr);
    }
};

class Trie {
private:
    TrieNode *root;
public:
    Trie () {
        root = new TrieNode();
    }

    ~Trie() {
        destroy(root);
    }

    void destroy(TrieNode* root) {
        for (auto node : root->next) {
            if (node) {
                destroy(node);
            }
        }

        delete root;
    }

    TrieNode *getRoot() {
        return root;
    }

    void insert(string s) {
        TrieNode *p = root;
        for (int i = 0; i < s.length(); ++i) {
            if (p->next[s[i] - 'a'] == nullptr) {
                p->next[s[i] - 'a'] = new TrieNode();
            }
            p = p->next[s[i] - 'a'];
        }

        p->isWord = true;
    }
};

class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        Trie wordTrie;
        TrieNode *root = wordTrie.getRoot();

        for (string word : words) {
            wordTrie.insert(word);
        }

        unordered_set<string> res;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[0].size(); ++j) {
                string word;
                helper(board, i, j, root, word, res);
            }
        }

        return vector<string>(res.begin(), res.end());
    }

    void helper(vector<vector<char>>& board, int i, int j,
                TrieNode *node, string word, unordered_set<string>& res) {

        if (i < 0 || i >= board.size()
            || j < 0 || j >= board[0].size()
            || board[i][j] == '#') {

            return;
        }

        if (node->next[board[i][j] - 'a']) {
            word.push_back(board[i][j]);
            node = node->next[board[i][j] - 'a'];

            if (node->isWord) {
                res.insert(word);
            }

            char tmp = board[i][j];
            board[i][j] = '#';
            helper(board, i + 1, j, node, word, res);
            helper(board, i - 1, j, node, word, res);
            helper(board, i, j + 1, node, word, res);
            helper(board, i, j - 1, node, word, res);
            board[i][j] = tmp;
        }
    }
};
```

### 425. Word Squares

* search the trie row by row, first row include the start of the second row in
  WS. So we can build a prefix for next row from previous row.
* Use the prefix for current row, we can search the trie for a complete word for
  current row.
* The key to solve this problem is in a node to record all the index of the word
  in the given words that share a prefix. So that given the prefix we build for
  current row, we can retrive words with this prefix fast.
* The trie node definition doesn't have `isEnd`, because in this problem, we
  know the length of the words are fixed. if the char we found from the trie is
  exist they must be a valid word.
* The "backtracking" is necessary because the word we get from its prefix might
   not form a WS, we have to "backtrack" and try a different word.

```C++
struct TrieNode {
    bool isEnd;
    vector<int> indexes;
    vector<TrieNode*> next;
    TrieNode() {
        next = vector<TrieNode*>(26, NULL);
    }
};

class Trie {
    TrieNode *root;
public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        destroy(root);
    }

    void destroy(TrieNode *root) {
        for (auto node : root->next) {
            if (node) {
                destroy(node);
            }
        }

        delete root;
    }

    TrieNode *getRoot() {
        return root;
    }

    void insert(string s, int idx) {
        TrieNode *p = root;
        for (int i = 0; i < s.length(); ++i) {
            if (p->next[s[i] - 'a'] == NULL) {
                p->next[s[i] - 'a'] = new TrieNode();
            }
            p = p->next[s[i] - 'a'];
            p->indexes.push_back(idx);
        }
    }
};

class Solution {
public:
    vector<vector<string>> wordSquares(vector<string>& words) {
        int m = words.size();
        int n = m ? words[0].size() : 0;
        vector<vector<string>> res;

        Trie wordsTrie;
        for (int i = 0; i < m; ++i) {
            wordsTrie.insert(words[i], i);
        }

        TrieNode *root = wordsTrie.getRoot();

        for (int i = 0; i < m; ++i) {
            vector<string> wordsSquare(n, "");
            wordsSquare[0] = words[i];
            helper(words, 1, root, wordsSquare, res);
        }

        return res;
    }

    void helper(vector<string>& words, int count, TrieNode *root,
                vector<string>& wordsSquare, vector<vector<string>>& res) {
        if (count == words[0].size()) {
            res.push_back(wordsSquare);
            return;
        }

        string prefix = "";
        for (int i = 0; i < count; ++i) {
            prefix += wordsSquare[i][count];
        }

        TrieNode *p = root;
        for (char c : prefix) {
            if (p) {
                p = p->next[c - 'a'];
            }
        }

        if (p == nullptr)
            return;

        for (int idx : p->indexes) {
            wordsSquare[count] = words[idx];
            helper(words, count + 1, root, wordsSquare, res);
        }
    }
};
```
