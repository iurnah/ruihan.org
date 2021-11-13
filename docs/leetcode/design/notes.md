# Data Structure Design

## static data structure V.S. Dynamic data structure

* Static data structure such as array, vector, or set is prefered in the case that
  the only need for the data operation is some static query operations such as
  search, select, rank, etc..
* While dynamic data structure such as hash table or headp is better to support
  operations that modify the data structure dynamically, such as insert, delete,
  etc.. In solving a design problem, following these steps:

1. Analyze the data and operations needed, identify whether the static data structure
   is sufficient to support all the operations. If a static data structure like vector
   or set can achieve over 70% percent of the operations required, use the static data
   structure and introduce some efficient algorithm to build customized routines to
   achieve the lacking operations. (i.e. Optimize it over a linear search using binary
   search) If this isn't possible or simply too complicated to implement, we should
   turn to a dynamic data structure.
2. evaluate each dynamic data structure to check which comes as the most natural option
   for the given data. In selecting the data structure you should focus on the operation
   supported and the efficiency of those operations.
3. Implement the static operations needed. Those operations such as search, select, min, max, etc..
4. Implement the dynamic operations needed. Those operations such as Insert, delete, etc..

## Problems

###