# Backtracking

## introduction

Backtracking algorithm can be used to generate all the subsets of a given set,
all the permutation of a given sequence, and all the combinations of k elements
from a given set with n elements. The algorithms are very similar but differ in
some unique property of each problem.

## Subnets

* How many subsets there are for a set with n elements? ($2^n$)
* How to generate all the subsets if there are NO duplicates in the set?
  (See problem [Subsets](./#subsets))
* How to generate all the UNIQUE subsets if there are duplicates in the set?
  (See problem [Subsets II](./#subsets-ii))
* What are the application of subset?

## Permutation

* How many permutations (number of ordering) of a sequence with n elements? ($n!$)
* How to generate all the permutations if there are NO duplicates in the set?
  (See problem [Permutations](./#permutations))
* How to generate all the UNIQUE permutations if there are duplicates in the set?
  (See problem [Permutations II](./#permutations-ii))
* What are the applications of permutation? (upper bound of sorting algorithms)

## Combination

* How many combinations of k elements from a set of n elements? ($\binom{n}{k}$)
* How to derive the formula $\binom{n}{k} = \frac{n!}{k! (n - k)!}$?
    1. Choose k elements from the set one after another without putting back:
       $n (n - 1) \dots (n - k + 1) = \frac{n!}{(n - k)!}$
    2. Calculate the number of ways to choose k elements as $\binom{n}{k}$, then
       order the k elements: $\binom{n}{k} \times k!$
    3. Equalize the two give us: $\binom{n}{k} \times k!$
* What’s the relation between the subset and combination?
* For `k` from `0` to `n`, get all the combinations, all those combinations will
  form the powerset of the original set of n elements. We have $\sum_{k = 0}^{n}\binom{n}{k} = \binom{n}{0} + \binom{n}{1}, \dots, \binom{n}{n} = 2^n\text{(number of all set)}$
* How to generate all the combination if there are NO duplicates in the set?
  (See problem [Combinations](./#combinations))
* How to generate all the UNIQUE combinations if there are duplicates in the set?
  (See problem [Combination Sum](./#combination-sum), [Combination Sum II](./#combination-sum-ii),
  [Combination Sum III](./#combination-sum-iii), [Backpack VI](./#backpack-vi))
* What are the applications of combination? (calculate the upper bound of sorting algorithms)

## Partition

* How many ways can we partition a set of `n` elements into `r` groups, with the
  `i`-th group have $n_i$ elements. ($\frac{n!}{n_1! n_2!, \dots, n_r!}$)
    1. We will look at the problem this way: note the numbfer of different ways
       of partitioning as $C$ , each partition have $n_1, n_2, \dots, n_r$ elements.
       We align $n_1, n_2, \dots, n_r$ together to form a `n` elements sequence
       and there are $n!$ of such sequences. Remeber for each partition, there
       are $n_1!$ different sequences. So that we have so we have $C \cdot n_1! \cdot n_2! \dots n_r! = n!$,
       and $\frac{n!}{n_1! n_2!, \dots, n_r!}$
* When the `r = 2`, the partition problem essentially becomes a combination problem.
* What are the applications of partition? (calculate the upper bound of sorting algorithms)

## Summary

**Subset**, **permutation**, and **combination** problems can be solved using a
single code template. Other problems that are involving multiple steps, and asking
for multiple eligible results that fulfill certain criteria, could also use this
template. The subtlety arise when deal with replicates element in the problems.

There are some general principles that I summarized for solving it.

1. The recursive helper function prototype:
```C++
helper(vector<int> nums, int n, [int cur], vector<int> &subset, vector<vector<int>> &result);
```

2. The "*select action*" could be done through update the index `int cur` for
   each level of recursive call.
3. Each `for` iteration (invoke helper function) is to "*select*" an element in
   this level, it recursively adding more elements to the temporary result.
4. To remove duplicate result if duplicate element presented, we first need to
   be clear about where the duplicate results come from. e.g. In generating the
   power subsets, the duplicated results due to repeatedly selecting the same
   element from different original position. (consecutive positions, since it is
   sorted first). Therefore, the idea to avoid duplicate results is to ***not***
   select the duplicate elements for a second time to form the same pattern, we
   use the following template check statement to achieve this.
```C++
if (i != cur && nums[i] == nums[i - 1]){
        continue;  
}
```

!!! note "4 elements of backtracking"
    1. What is the iteration? (the for loop, the same level of node in the tree)
    2. What is the recursion? (what index will be advanced?)
    3. What is counted in the result? (what should be pushed back?)
    4. When is the result returned? (the cutting condition)

!!! note "traceability of backtracking"
    By using recursion and iteration together in this backtrack technique, we
    can imagine the problem as growing a recursive tree.

    * To grow the node at the same level, an index in a for loop can do the job.
    * To grow a child of a node, a recursive function should be called.
      More specifically, we can advance the index by passing it into the recursive
      function. This index increment let the tree grow vertically.
