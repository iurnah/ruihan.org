# Reservoir Sampling

## Problem statement

Randomly select 1 item (or k items) from a stream of items of unknown length.
Each item should be selected with equal probability.

## Proof

Starting with the case that only 1 element need to be chosen randomly with equal
probability, that is an element $a_i$ is chosen with probability $1/n$. How
should we implement the "choose" action? In other words, in obtaining a element
$a_i$, how could we programmatically decide whether to keep it or not keep it?
If we know the probability to keep $a_i$ in timestamp $i$, and the probability
to **not** replace it after $a_i$, we can calcualte the probability of $a_i$
beening selected ultimately. But how?

Define $P(a_i)$ the probability that $a_i$ is the final element selected. We
know from the requirement that each element should be selected with equal
probability $P(a_1) = \dots = P(a_i) = P(a_{i+1}) = \dots = P(a_n) = 1/n$. For
$a_i$, we have the following:

$P(a_i) = P(a_i\text{ is selected at timestamp } i) \cdot P(a_i \text{ have not been replaced at timestamp: } i + 1, i + 2, \dots, n)$

If at timestamp $i$, the data stream terminates. If we select the element $a_i$
with probability $\frac{1}{i}$, it fulfilled the requirement of the problem
statement. If there are more element followed, we have to ensure those will not
be selected (not to replace $a_i$), otherwise, it will contradict to the
assumption that $a_i$ is the final element being selected. For element $a_{i+1}$,
it will be selected with probability $\frac{1}{i+1}$, and not being seleted with probability
$1-\frac{1}{i+1}$. thus the above probability becomes:

$\begin{eqnarray*}
P(a_i) & = & P(a_i\text{ is selected at timestamp } i) \cdot P(a_i \text{ have not been replaced at timestamp: } i + 1, i + 2, \dots, n)\\
       & = & \frac{1}{i} \cdot (1-\frac{1}{i+1}) \cdot (1-\frac{1}{i+2}) \dots \cdot (1-\frac{1}{n}) \\
       & = & \frac{1}{i} \cdot (\frac{i}{i+1}) \cdot (\frac{i+1}{i+2}) \dots (\frac{n-2}{n-1}) \cdot (\frac{n-1}{n}) \\
       & = & \frac{1}{n}
\end{eqnarray*}$

Next, we will take a look at the case that select $k$ element, with each
outcome with an equal probability.

## Reference

1. https://www.youtube.com/watch?v=Ybra0uGEkpM
2. https://gregable.com/2007/10/reservoir-sampling.html

## Problem

### 382. Linked List Random Node

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
    ListNode *head;
public:
    /** @param head The linked list's head.
        Note that the head is guaranteed to be not null, so it contains at least one node. */
    Solution(ListNode* head) {
        this->head = head;
    }

    /** Returns a random node's value. */
    int getRandom() {
        int res = this->head->val;
        int c = 2;
        ListNode *curr = this->head->next;

        while (curr) {
            int j = rand() % c;
            if (j == 0) {
                res = curr->val;
            }

            c++;
            curr = curr->next;
        }

        return res;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(head);
 * int param_1 = obj->getRandom();
 */
```

### 398. Random Pick Index

```c++
class Solution {
    vector<int> vec;

public:
    Solution(vector<int> nums) {
        vec = nums;
    }

    int pick(int target) {
        int cnt = 0, res = -1;
        for (int i = 0; i < vec.size(); ++i) {
            if (vec[i] != target) continue;
            ++cnt;
            if (rand() % cnt == 0) res = i;
        }

        return res;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution obj = new Solution(nums);
 * int param_1 = obj.pick(target);
 */
```
