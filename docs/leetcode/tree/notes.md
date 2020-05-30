# Tree

## Summary

These binary tree problems could be divided into the following type of questions:

* Path sum problem
* Traversal problem
* Binary tree property problem
* Ancestor and successor problem
* Build binary tree problem
* Binary tree manipulation problem

Each question type could be solved using the similar thought process, and
problems in each individual type share the similar traps and common pitfalls.
I will write some of my consolidated notes for them.

## Binary tree property problems

### Kth Smallest Element in a BST

### Find Mode in Binary Search Tree

### Count Univalue Subtrees

### Closest Binary Search Tree Value

### Closest Binary Search Tree Value II

### Binary Tree Tilt

## Path sum and longest path problem

### Path Sum

### Path Sum II

### Path Sum III* (how the linear solution using hash works?)

### Path Sum IV

### Binary Tree Maximum Path Sum*

### Sum Root to Leaf Numbers

### Binary Tree Paths

### Sum of Left Leaves

### Binary Tree Longest Consecutive Sequence

### Binary Tree Longest Consecutive Sequence II

### Longest Univalue Path

### Most Frequent Subtree Sum

### Diameter of Binary Tree

## Preorder traversal

### Binary Tree Preorder Traversal

```C++ tab="Morris traversal (preorder)" hl_lines="30"
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        vector<int> res;

        while (curr != nullptr) {
            if (curr->left == nullptr) {
                res.push_back(curr->val);
                curr = curr->right;
            } else {
                prev = curr->left;
                while (prev->right != nullptr && prev->right != curr)
                    prev = prev->right;

                if (prev->right == nullptr) {

                    res.push_back(curr->val);

                    prev->right = curr;
                    curr = curr->left;
                } else {
                    prev->right = nullptr;
                    curr = curr->right;
                }
            }
        }

        return res;
    }
};
```

```C++ tab="Iterative using stack"
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        if (root == nullptr) return res;

        stack<TreeNode*> s;
        s.push(root);

        while (!s.empty()) {
            TreeNode* node = s.top(); s.pop();
            res.push_back(node->val);

            if (node->right) // push right first
                s.push(node->right);

            if (node->left)
                s.push(node->left);
        }

        return res;
    }
};
```

```C++ tab="Recursive"
class Solution {
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == NULL) return res;

        helper(root, res);
        return res;
    }

    void helper(TreeNode *root, vector<int> &res){
        if (root == NULL) return;

        res.push_back(root->val);
        helper(root->left, res);
        helper(root->right, res);

    }
};
```

### Verify Preorder Sequence in Binary Search Tree

### Verify Preorder Serialization of a Binary Tree

## Inorder traversal

### Binary Tree Inorder Traversal

```C++ tab="Morris traversal inorder"
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        vector<int> res;

        while (curr != nullptr) {
            if (curr->left == nullptr) {
                res.push_back(curr->val);
                curr = curr->right;
            } else {
                prev = curr->left;
                while (prev->right != nullptr && prev->right != curr)
                    prev = prev->right;

                if (prev->right == nullptr) {
                    prev->right = curr;
                    curr = curr->left;
                } else {
                    prev->right = nullptr;
                    res.push_back(curr->val);
                    curr = curr->right;
                }
            }
        }

        return res;
    }
};
```

```C++ tab="Iterative using a stack"
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        if (root == nullptr)
            return res;
        stack<TreeNode *> s;
        TreeNode* curr = root;

        while (!s.empty() || curr) {
            if (curr) {
                s.push_back(curr);
                curr = curr->left;
            } else {
                curr = s.top();
                s.pop();
                res.push_back(curr->val);
                curr = curr->right;
            }
        }

        return res;
    }
}
```

```C++ tab="Recursive"
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        // write your code here
        vector<int> res;
        if (root == NULL) return res;

        helper(root, res);

        return res;
    }

    void helper(TreeNode *root, vector<int>& res) {
        if (root == NULL) {
            return;
        }

        helper(root->left, res);
        res.push_back(root->val);
        helper(root->right, res);
    }
};
```

### Recover Binary Search Tree

### [Kth Smallest Element in a BST](./kth-smallest-element-in-a-bst)

### [Closest Binary Search Tree Value](./closest-binary-search-tree-value)

### Closest Binary Search Tree Value II*

### Minimum Absolute Difference in BST

## Postorder traversal

### Binary Tree Postorder Traversal

```C++ tab="iterative using 2 stacks"
/**
 * Definition of TreeNode:
 * class TreeNode {
 * public:
 *     int val;
 *     TreeNode *left, *right;
 *     TreeNode(int val) {
 *         this->val = val;
 *         this->left = this->right = NULL;
 *     }
 * }
 */
class Solution {
    /**
     * @param root: The root of binary tree.
     * @return: Postorder in vector which contains node values.
     */
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;
        if (root == NULL) return res;
        stack<TreeNode *> s;
        stack<TreeNode *> output;

        s.push(root);
        while (!s.empty()) {
            TreeNode *cur = s.top();
            output.push(cur);
            s.pop();

            /* push left first */
            if (cur->left) {
                s.push(cur->left);
            }

            /* push right second */
            if (cur->right) {
                s.push(cur->right);
            }
        }

        /* populate the result */
        while (!output.empty()) {
            res.push_back(output.top()->val);
            output.pop();
        }

        return res;
    }
};
```

```C++ tab="Recursive solution"
class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        // write your code here
        vector<int> res;
        if (root == NULL) return res;

        helper(root, res);

        return res;
    }

    void helper(TreeNode *root, vector<int>& res) {
        if (root == NULL) {
            return;
        }

        helper(root->left, res);
        helper(root->right, res);
        res.push_back(root->val);
    }
};
```

```C++ tab="Morris like traversal ussing a stack"
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> res;

        if (root == nullptr) return res;

        stack<TreeNode *> s;
        TreeNode* prev = nullptr;

        s.push(root);
        while (!s.empty()) {
            TreeNode* curr = s.top();

            if (!prev || prev->right == curr || prev->left == curr) {
                if (curr->left) {
                    s.push(curr->left);
                } else if (curr->right) {
                    s.push(curr->right);
                } else {
                    res.push_back(curr->val);
                    s.pop();
                }
            } else if (curr->left == prev) {
                if (curr->right) {
                    s.push(curr->right);
                } else {
                    res.push_back(curr->val);
                    s.pop();
                }
            } else if (curr->right == prev) {
                res.push_back(curr->val);
                s.pop();
            }

            prev = curr;
        }
    }
};
```

### Find Duplicate Subtrees

## Level order traversal

## Binary tree vertical order traversal

## Tree serialize and deserialize

## Tree node manipulate problems