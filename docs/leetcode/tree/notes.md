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

### 951. Flip Equivalent Binary Trees

* Solution 1: recursive

```python
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def flipEquiv(self, root1: Optional[TreeNode], root2: Optional[TreeNode]) -> bool:
        if root1 == root2 == None:
            return True

        if root1 and root2 and root1.val == root2.val:
            return (self.flipEquiv(root1.left, root2.left) and self.flipEquiv(root1.right, root2.right)) \
                or (self.flipEquiv(root1.left, root2.right) and self.flipEquiv(root1.right, root2.left))
        else:
            return False
```

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

### 1339. Maximum Product of Splitted Binary Tree

* use recursion to calculate the subtree sum and then calculate the results.

=== "Python solution 1"

    ```Python
    class Solution:
        def __init__(self):
            self.res = 0
            self.total = 0

        def maxProduct(self, root: Optional[TreeNode]) -> int:
            def dfs(node):
                if not node: return 0
                left, right = dfs(node.left), dfs(node.right)
                self.res = max(self.res, left * (self.total - left), right * (self.total - right))
                return left + right + node.val

            self.total = dfs(root)
            dfs(root)
            return self.res % (10**9 + 7)
    ```

=== "Python solution 2"

    ```Python
    class Solution:
        def maxProduct(self, root: Optional[TreeNode]) -> int:
            vals=[]

            def dfs(node):
                if not node: return 0
                left, right = dfs(node.left), dfs(node.right)
                vals.append(left + right + node.val)
                return left + right + node.val

            total = dfs(root)

            return max(val * (total - val) for val in vals) % (10**9 + 7)
    ```

## Preorder traversal

### Morris traversal (preorder)

Preorder Morris traversal use constant space (tow local variables `pred`, `curr`)
to walk through the binary tree from the left most node to the right most node.
It mainly relies on modifying the `pred->right` pointer to point to the `curr`.
This is the key to allow `curr` to traverse the right after finish the left subtree.

The implementation idea as following: (current node start from root)

1. If the left subtree doesn't exist, output curr node, and update the `curr`: `curr = curr->right`.
2. If the left subtree exists, we find the maximum node in left subtree (the predecessor of `curr`),
   based on the right value (NULL or possibly being modified to point to ancestor root) of the predecessor we do following:
      1. If the right value of predecessor is `NULL`, connect this right value: `pred->right = curr;`, and update the `curr`: `curr = curr->left;`
      2. If the right value of predecessor is `curr`, output `curr`, recover right value to `NULL`, and update the `curr`: `curr = curr->right;`
3. repeat the 1, 2 steps until `curr == NULL`

```c++
void inorderMorrisTraversal(TreeNode *root) {
    TreeNode *curr = root, *pred = NULL;
    while (curr != NULL) {
        if (curr->left == NULL) {        /* 1. */
            printf("%d ", curr->val);
            curr = curr->right;
        } else {
            /* find predecessor */
            pred = curr->left;
            while (pred->right != NULL && pred->right != curr)
                pred = pred->right;

            if (pred->right == NULL) {   /* 2.1 */
                pred->right = curr;
                printf("%d ", curr->val);  // only difference from inorder traversal
                curr = curr->left;
            } else {                     /* 2.2 */
                pred->right = NULL;
                curr = curr->right;
            }
        }
    }
}
```

### Binary Tree Preorder Traversal

=== "Morris traversal (preorder)"

    ```C++  hl_lines="30"
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

=== "Iterative using stack"

    ```C++
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

=== "Recursive"

    ```C++
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

### Morris traversal (inorder)

Inorder Morris traversal use constant space (tow local variables `pred`, `curr`) to
walk through the binary tree from the left most node to the right most node.
It mainly relies on modifying the `pred->right` pointer to point to the `curr`.
This is the key to allow `curr` to traverse the root after finish the left subtree.

The implementation idea as following: (current node start from root):

1. If the left subtree doesn't exist, output curr node, and update `curr`: `curr = curr->right;`
2. If the left subtree exists, we find the maximum node in left subtree (the predecessor of `curr`),
   based on the right value (`NULL` or possibly being modified to point to ancestor root) of the predecessor we do following:
   1. If the right value of predecessor is `NULL`, connect this right value: `pred->right = curr;`, and update `curr`: `curr = curr->left;`
   2. If the right value of predecessor is `curr`, output `curr`, recover right value to `NULL`, and update `curr`: `curr = curr->right;`
3. repeat the 1, 2 steps until `curr == NULL`

```c++
void inorderMorrisTraversal(TreeNode *root) {
    TreeNode *curr = root, *pred = NULL;
    while (curr != NULL) {
        if (curr->left == NULL) {        /* 1. */
            printf("%d ", curr->val);
            curr = curr->right;
        } else {
            /* find predecessor */
            pred = curr->left;
            while (pred->right != NULL && pred->right != curr)
                pred = pred->right;

            if (pred->right == NULL) {   /* 2.1 */
                pred->right = curr;
                curr = curr->left;
            } else {                     /* 2.2 */
                pred->right = NULL;
                printf("%d ", curr->val);
                curr = curr->right;
            }
        }
    }
}
```

### Binary Tree Inorder Traversal

=== "Morris traversal inorder"

    ```C++
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

=== "Iterative using a stack"

    ```C++
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

=== "Recursive"

    ```C++
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

Morris traversal for preorder and postorder [cnblog post](http://www.cnblogs.com/AnnieKim/archive/2013/06/15/MorrisTraversal.html) reference.

### Binary Tree Postorder Traversal

=== "iterative using 2 stacks"

    ```c++
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

=== "Recursive solution"

    ```C++
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

=== "Morris like traversal ussing a stack"

    ```C++
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

### Binary Tree Level Order Traversal

### Binary Tree Zigzag Level Order Traversal

### Invert Binary Tree

### Binary Tree Right Side View

### Serialize and Deserialize Binary Tree

### Populating Next Right Pointers in Each Node

### Populating Next Right Pointers in Each Node II

## Binary tree vertical order traversal

## Tree serialize and deserialize

### Serialize and Deserialize BST

### Serialize and Deserialize Binary Tree

### Construct String from Binary Tree

### Construct Binary Tree from String

### Construct Binary Tree from Preorder and Inorder Traversal

### Construct Binary Tree from Inorder and Postorder Traversal

### Recover Binary Search Tree

### Flatten Binary Tree to Linked List

### Convert Sorted Array to Binary Search Tree

### Convert Sorted List to Binary Search Tree

!!! note
    __Serialization__
    Tree serialization requires two ingredients: 1) tree traversal. 2) delimiter.
    All of four order traversal methods can be used to serialize a tree. For BST,
    the delimiter isn't needed. However, the deserialization should use the BST
    property to verify whether a node is a valid child or should be a sibling node.

!!! note
    __Deserialization__
    Deserialization prefer recursion. The key to implement it is to include the
    index to the serialized data as a recursive parameter and advance it after
    deserialized the current value inside the body. For BST, the recursive function
    should also include the minVal and maxVal in the parameter list.

## Tree node manipulate problems

### Insert a Node in Binary Search Tree*

### Remove Node in Binary Search Tree*

### Delete Node in a BST
