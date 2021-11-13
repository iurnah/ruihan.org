# Sorting Algorithms

## Insertion sort

### Search Insert Position

### Count of Smaller Numbers After Self

## Merge sort

Beside sorting an array, the application of merge sort includes counting of elements
that fulfill a property (i.e. Count Inversion and Count of Range Sum) and find the
maximum subarray/submatrix sum under some constraints (i.e. Max Sum of Rectangle No Larger Than K).
Using the idea of merge sort doesn't come naturally as one solving those hard problems.
Identifying the characteristics of those problems and connecting to the characteristics
of merge sort should be helpful in future. The merge sort would look like the following.

```
Parameters: the passed in parameter have start and end.

1. Merge sort base case.
2. Split and recurse on the left part and on the right part.
3. Do the counting/maxing use two pointers.
   For each element (first pointer) in the right part and each element (second pointer) in the right part, we can check their relative values and do the counting/maxing.
4. Do the merge. The merge could be done in parallel with the counting/maxing or use the C++ STL inplace_merge() before return.
```

### Count Inversion

### Count of Range Sum

### Reverse Pairs

### Count of Smaller Numbers After Self

### Max Sum of Rectangle No Larger Than K

### Sort List

## Quick sort

## Sort List

## Heap sort

## Bubble sort

## Counting sort

## Radix sort

## Maximum Gap

## Three way partition

## Order statistics

### Quickselect algorithm

The idea is similar to the Quicksort, it selects a pivot to partition the original array, and continue to recurse on the left part or the right part based on the pivot position and the order statistics.

### Kth Largest Element in an Array

### Third Maximum Number (if else trick)

### Median of Two Sorted Arrays
