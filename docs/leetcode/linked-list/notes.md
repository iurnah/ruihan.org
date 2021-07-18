# Linked List

## Tricks to solve linked list

### Edge case need to watch in linked list

### When to use Dummy node

### Iterate tricks

* trick 1: If the iteration is for two linked lists with different length. You
  can use `while (l1 || l2)`, inside the while loop, you can maintain the loop
  invariance by checking the `nullptr` and considering all edge cases.

### Recursive in linked list

## Catergory 1 Iterate linked lists

### Add Two Numbers

* Think about how to make the iteration elegant without dealing many `if else` conditions

```C++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(INT_MIN);
        ListNode *prev = &dummy;

        int sum = 0;
        while (l1 || l2 || sum) {
            sum += (l1 ? l1->val : 0) + (l2 ? l2->val : 0);
            ListNode *node = new ListNode(sum % 10);
            prev->next = node;
            prev = node;
            l1 = l1 ? l1->next : l1;
            l2 = l2 ? l2->next : l2;
            sum /= 10;
        }

        return dummy.next;
    }
};
```

### Add Two Numbers II

* It seems we need a stack. We can use a `stack` variable or use recursion.

=== "C++ stack"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode(int x) : val(x), next(NULL) {}
    * };
    */
    class Solution {
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            stack<int> s1, s2;
            while(l1) {
                s1.push(l1->val);
                l1 = l1->next;
            }
            while(l2) {
                s2.push(l2->val);
                l2 = l2->next;
            }

            int sum = 0;
            ListNode *p = nullptr, *head = new ListNode(0);
            while (!s1.empty() || !s2.empty()) {
                if (!s1.empty()) { sum += s1.top(); s1.pop(); }
                if (!s2.empty()) { sum += s2.top(); s2.pop(); }
                head->val = sum % 10;
                p = new ListNode(sum / 10);
                p->next = head;
                head = p;
                sum /= 10;
            }

            return head->val == 0 ? head->next : head;
        }
    };
    ```

### Plus One Linked List

* We need a stack. Using recursion can help us handle the case well

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* plusOne(ListNode* head) {
        int carry = 0;

        helper(head, carry);
        if (carry > 0) {
            ListNode* node = new ListNode(carry);
            node->next = head;
            head = node;
        }
        return head;
    }

    // carry can also be passed by return value
    void helper(ListNode* head, int& carry) {
        // base case
        if (head->next == nullptr) {
            int tmp = head->val;
            head->val = (head->val + 1 + carry) % 10;
            carry = (tmp + 1 + carry) / 10;
            return;
        }

        helper(head->next, carry);
        if (carry > 0) {
            int tmp = head->val;
            head->val = (head->val + carry) % 10;
            carry = (tmp + carry) / 10;
        }
    }
};
```

## Catergory 2 Remove nodes from linked lists

### Remove Nth Node From End of List

* use slow and fast node to "measure" the nth node and remove it.

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(INT_MIN);
        dummy.next = head;
        ListNode *slow = &dummy;
        ListNode *fast = &dummy;

        while (n > 0) {
            fast = fast->next;
            n--;
        }

        while (fast->next) {
            slow = slow->next;
            fast = fast->next;
        }
        slow->next = slow->next->next;

        return dummy.next;
    }
};
```

### Remove Linked List Elements

* This is an easy problem, but can you use two * programing to solve it?

=== "C++ solution"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode(int x) : val(x), next(NULL) {}
    * };
    */
    class Solution {
    public:
        ListNode* removeElements(ListNode* head, int val) {
            ListNode dummy(0);
            dummy.next = head;
            ListNode *curr = &dummy;

            while (curr->next) {
                if (curr->next->val == val) {
                    ListNode *next = curr->next;
                    curr->next = next->next;
                } else {
                    curr = curr->next;
                }
            }

            return dummy.next;
        }
    };
    ```

=== "Two * solution"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode(int x) : val(x), next(NULL) {}
    * };
    */
    class Solution {
    public:
        ListNode* removeElements(ListNode* head, int val) {
            // if (head == nullptr) return nullptr;
            ListNode **headptr = &head;

            ListNode **curr = headptr;
            while (*curr != NULL) {
                ListNode *entry = *curr;
                if (entry->val == val) {
                    *curr = entry->next;
                } else {
                    curr = &(entry->next);
                }
            }

            return *headptr;
        }
    };
    ```

### Remove Duplicates from Sorted List

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (head == nullptr || head->next == nullptr)
            return head;
        ListNode* prev = head;
        ListNode* curr = head->next;
        while (curr) {
            if (curr->val == prev->val) {
                curr = curr->next;
                prev->next = curr;
            } else {
                curr = curr->next;
                prev = prev->next;
            }
        }

        return head;
    }
};
```

### Remove Duplicates from Sorted List II

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (head == nullptr) return nullptr;
        ListNode dummy(INT_MIN);
        dummy.next = head;
        ListNode *prev = &dummy;
        ListNode *curr = head;

        while (curr) {
            if (curr->next && curr->val == curr->next->val) {
                int tmp = curr->val;
                while (curr && curr->val == tmp) {
                    curr = curr->next;
                }
                prev->next = curr;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }

        return dummy.next;
    }
};
```

### Remove Zero Sum Consecutive Nodes from Linked List

## Catergory 3 Reverse linked lists

### Reverse Linked List

=== "C++ iterative with dummy"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode(int x) : val(x), next(NULL) {}
    * };
    */
    class Solution {
    public:
        ListNode *reverse(ListNode *head) {
            ListNode *new_head = nullptr;

            while(head != nullptr) {
                ListNode *tmp = head->next;
                head->next = new_head;
                new_head = head;
                head = tmp;
            }

            return new_head;
        }
    };
    ```

=== "C++ iterative without dummy"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode(int x) : val(x), next(NULL) {}
    * };
    */
    class Solution {
    public:
        ListNode* reverseList(ListNode* head) {
            if (head == nullptr) return nullptr;

            ListNode *curr = head;
            while(curr->next) {
                ListNode *tmp = curr->next;
                curr->next = tmp->next;
                tmp->next = head;
                head = tmp;
            }

            return head;
        }
    };
    ```

=== "C++ recursive"

    ```c++
    /**
    * Definition for singly-linked list.
    * struct ListNode {
    *     int val;
    *     ListNode *next;
    *     ListNode(int x) : val(x), next(NULL) {}
    * };
    */
    class Solution {
    public:
        ListNode* reverseList(ListNode* head) {
            if (head == nullptr || head->next == nullptr)
                return head;

            ListNode *new_head = reverseList(head->next);
            head->next->next = head;
            head->next = nullptr;

            return new_head;
        }
    };
    ```

### Reverse Linked List II

### Palindrome Linked List

### Reverse Nodes in k-Group

## Catergory 4 Merge linked lists

### Merge Two Sorted Lists

### Merge k Sorted Lists

### Reorder List

### Sort List

## Catergory 5 Cycle in linked lists

### 708. Insert into a Sorted Circular Linked List

* What the loop invariant?
    * It is easy to handle the common case `prev <= insertVal <= curr`.
    * For the special case when "wrap around" the circle, it could be `insertVal`
      is the max or min, but in both cases, the insertion operation is the same.

=== "Python loop invariant"

    ``` python
    """
    # Definition for a Node.
    class Node:
        def __init__(self, val=None, next=None):
            self.val = val
            self.next = next
    """

    class Solution:
        def insert(self, head: 'Node', insertVal: int) -> 'Node':
            node = Node(insertVal)
            node.next = node
            if not head:
                return node

            prev, curr = head, head.next
            while not prev.val <= insertVal <= curr.val and not prev.val > curr.val > insertVal and not insertVal > prev.val > curr.val:
                prev, curr = prev.next, curr.next
                if prev == head:
                    break

            prev.next = node
            node.next = curr

            return head
    ```

=== "C++ loop invariant"

    ```c++
    /*
    // Definition for a Node.
    class Node {
    public:
        int val = NULL;
        Node* next = NULL;

        Node() {}

        Node(int _val, Node* _next) {
            val = _val;
            next = _next;
        }
    };
    */
    class Solution {
    public:
        Node* insert(Node* head, int insertVal) {
            if (head == nullptr) {
                head = new Node(insertVal, nullptr);
                head->next = head;
                return head;
            }

            Node* prev = head;
            Node* next = head->next;
            while (!(prev->val <= insertVal && insertVal <= next->val)
                && !(prev->val > next->val && insertVal < next->val)
                && !(prev->val > next->val && insertVal > prev->val)) {
                prev = prev->next;
                next = next->next;
                if (prev == head)
                    break;
            }

            prev->next = new Node(insertVal, next);

            return head;
        }
    };
    ```

### Linked List Cycle

### Linked List Cycle II

### Intersection of Two Linked Lists

### Rotate List

## Catergory 6 Linked List property
