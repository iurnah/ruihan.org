#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <unordered_map>
#include <memory>

using namespace std;

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

void Trie::destroy(Node* node) {
    for (auto p : node->next) {
        if (p) {
            destroy(p);
        }
    }

    delete node;
}

// insert iteratively
void Trie::insert(string word, int val) {
    Node *p = this->root;
    for (char s: word) {
        if (p->next[s - 'A'] == nullptr) {
            p->next[s - 'A'] = new Node();
        }
        p = p->next[s - 'A'];
    }

    p->val = val;
    p->isEnd = true;
}

// insert recursively, param node initially points to the root
Node* Trie::insert(Node* node, string word, int val, int d) {
    if (node == nullptr)
        node = new Node();

    if (word.size() == d) {
        node->isEnd = true;
        node->val = val;
        return node;
    }

    node->next[word[d] - 'A'] = insert(node->next[word[d] - 'A'], word, val, d + 1);

    return node;
}

// search interatively, can return node by change the signature
int Trie::search(string word) {
    Node* p = this->root;
    for (char s: word) {
        if (p) {
            p = p->next[s - 'A'];
        } else {
            break;
        }
    }

    if (p != nullptr && p->isEnd) {
        return p->val;
    }

    return INT_MIN;
}

// search recursively, can return node by change the signature
int Trie::search(Node* node, string word, int d) {
    if (node == nullptr)
        return INT_MIN;

    if (node->isEnd && d == word.length()) {
        return node->val;
    }

    return search(node->next[word[d]], word, d + 1);
}

// check whether exists a key start with a prefix
bool Trie::existsKeyStartsWith(string prefix) {
    Node* p = root;
    for (char c : prefix) {
        if (p) {
            p = p->next[c - 'A'];
        }
    }

    return p != nullptr;
}

// recursive helper function to get all the keys with a common prefix
void Trie::collectByPrefix(Node* node, string curr, vector<string>& keys) {
    if (node == nullptr)
        return;

    if (node->isEnd) {
        keys.push_back(curr);
        //return;
    }
    for (int i = 0; i < NUM_CHARS; i++) {
        curr.push_back(i + 'A');
        collectByPrefix(node->next[i], curr, keys);
        curr.pop_back();
    }
}

// get all the keys given a prefix, ominicomplex functionality
vector<string> Trie::keyStartWith(string prefix) {
    Node* p = root;
    for (char c : prefix) {
        if (p) {
            p = p->next[c - 'A'];
        }
    }

    vector<string> keys;
    if (p == nullptr) {
        return keys;
    }

    collectByPrefix(p, prefix, keys);

    return keys;
}

// recursive helper function to get all the keys that match a pattern
void Trie::collectByPattern(Node* node, string curr, string pattern, vector<string> &keys) {
    if (node == nullptr)
        return;

    int d = curr.length();
    if (pattern.length() == d && node->isEnd) {
        keys.push_back(curr);
        return;
    }

    char c = pattern[d];
    for (int i = 0; i < NUM_CHARS; i++) {
        if (c == '.' || c == (char)(i + 'A')) {
            curr.push_back((char)(i + 'A'));
            collectByPattern(node->next[i], curr, pattern, keys);
            curr.pop_back();
        }
    }

    return;
}

// get all the keys that match a pattern that support '.' wildcard
vector<string> Trie::keysThatMatch(string pattern) {
    vector<string> keys;

    collectByPattern(this->root, "", pattern, keys);

    return keys;
}

// search the longest word in the trie that match target
int Trie::search (Node* node, string target, int d, int length) {
    if (node == nullptr) return length;

    if (node->isEnd) length = d;

    if (d == target.length()) return length;

    char c = target[d];
    return search(node->next[c - 'A'], target, d + 1, length);
}

// get the longest key that matches the prefix of the target
string Trie::keyWithLongestMatch(string target) {
    int length = search(root, target, 0, 0);
    
    return target.substr(0, length);
}

// recursive helper function to detele a key and return the trie root
Node* Trie::deleteKey(Node* node, string key, int d) {
    if (node == nullptr)
        return nullptr;

    if (d == key.length()) {
        node->isEnd = false;
        node->val = INT_MIN;
    } else {
        char c = key[d];
        node->next[c] = deleteKey(node->next[c], key, d + 1);
    }

    // whether current node is a word end.
    if (node->isEnd) return node;

    // current node is not a word end, check whether it has children
    for (int i = 0; i < NUM_CHARS; ++i) {
        if (node->next[i]) return node;
    }

    // not word end, not children, delete it
    return nullptr;
}

// delete a key
void Trie::deleteKey(string key) {
    root = deleteKey(root, key, 0);
}

int main() {
    /*  test_keys1 in the trie      
              root  
        A      a      d     k                
        |    /  \    / \    |
        n   p   w   a   o   e
            |   |   |   |   | 
            p   a   y   c   e 
            |   |       |   |
            l   y       t   p
            |           |   |
            e           o   s
                        |
                        r 
    */
    vector<string> test_keys1 = {"An", "apple", "a", "day", "keeps", "doctors", "away"};
    vector<int> test_vals1 = {2, 5, 1, 3, 4, 7, 4};
    vector<string> test_keys2 = {"an", "aqqle", "A", "Day", "kep", "doctor", "way"};
    vector<int> test_vals2 = {-2, -5, -1, -3, -4, -7, -4};
    vector<string> test_keys3 = {"she", "sells", "sea", "shells", "by", "the", "sea", "shore", "is"};
    vector<int> test_vals3 = {22, 55, 11, 33, 44, 77, 14, 10, 1};

    Trie* test_trie = new Trie();

    cout << "Iterative method" << endl;

    cout << "adding...";
    for (int i = 0; i < test_keys1.size(); ++i) {
        cout << "(" << test_keys1[i] << ", " << test_vals1[i] << ") ";
        test_trie->insert(test_keys1[i], test_vals1[i]);
    }
    cout << endl;

    cout << "adding...";
    for (int i = 0; i < test_keys3.size(); ++i) {
        cout << "(" << test_keys3[i] << ", " << test_vals3[i] << ") ";
        test_trie->insert(test_keys3[i], test_vals3[i]);
    }
    cout << endl;

    cout << "searching original keys...";	
    for (int i = 0; i < test_keys1.size(); ++i) {
        cout << "(" << test_keys1[i] << ", " << test_trie->search(test_keys1[i]) << ") ";
    }
    cout << endl;

    cout << "searching testing keys...";
    for (int i = 0; i < test_keys2.size(); ++i) {
        cout << "(" << test_keys2[i] << ", " << test_trie->search(test_keys2[i]) << ") ";
    }
    cout << endl;

    cout << "searching with wildcard...";
    vector<string> res = test_trie->keysThatMatch("a.ple");
    for (string s : res) {
        cout << s << " ";
    }
    cout << endl;
    
    cout << "searching with prefix...";
    res = test_trie->keyStartWith("a");
    for (string s : res) {
        cout << s << " ";
    }
    cout << endl;

    cout << "searching key with longest match...";
    string res_str = test_trie->keyWithLongestMatch("shellsort");
    cout << res_str << endl;

    cout << "Deconstruct the trie...";
    // test_trie.destroy(test_trie->getRoot());
    // test_trie->~Trie();

    cout << "searching original keys after delete...";	
    for (int i = 0; i < test_keys1.size(); ++i) {
        cout << "(" << test_keys1[i] << ", " << test_trie->search(test_keys1[i]) << ") ";
    }
    cout << endl;

    return 0;
}