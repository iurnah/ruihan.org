# Graph

## Representations

### Edge lists

A edge is represented using an array of two vertex numbers or objects contain
the vertex numbers. Weights can be added as the third number or another member
of the objects.

* Example: `[[0,1], [0,6], [0,8], [1,4], [1,6], [1,9], [2,4], [2,6], [3,4], [3,5]]`
* Space: $\Theta(E)$
* Search edge: $O(E)$

### Adjacency matrix

Use a `matrix[i][j]` to represent the edges. `1` is connected, `0` is not connected.

* Example:

    ```text
    [ [0, 1, 0, 0, 0, 0, 1, 0, 1, 0],
    [1, 0, 0, 0, 1, 0, 1, 0, 0, 1],
    [0, 0, 0, 0, 1, 0, 1, 0, 0, 0],
    [0, 0, 0, 0, 1, 1, 0, 0, 1, 0],
    [0, 1, 1, 1, 0, 1, 0, 0, 0, 1],
    [0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
    [1, 1, 1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 0, 0, 1, 0, 0, 0, 1, 0, 0],
    [0, 1, 0, 0, 1, 0, 0, 1, 0, 0] ]
    ```

* Check edge `(i, j)` exists: $O(1)$
* Space: $\Theta(V^2)$
* Search djacent vertex: $O(V)$
* For undirected graph, adjacency matrix is symmetric. For a directed graph, the
  adjacency matrix need not be symmetric.

### Adjacency list

Representing a graph with adjacency lists combines adjacency matrices with edge lists. For each vertex `i`, store an array of the vertices adjacent to it. Each vertex has a list.

![Adjacency list](fig/adjacency-list.png)

* Space: $\Theta(2E)$ for undirected graph. $\Theta(E)$ for directed graph.
* Check edge `(i, j)` exists: $O(d)$, `d` is the degree of the vertex `i`. $0 \leq d \leq V - 1$.
* If the graph is weighted, then each item in each adjacency list is either a two-item array or an object, giving the vertex number and the edge weight.

[Representing graphs](https://www.khanacademy.org/computing/computer-science/algorithms/graph-representation/a/representing-graphs)

## Algorithms

### Bipartise

### Directed graph strongly connected components

### Kosaraju's Two-pass algorithm

1. Reverse each arc of the original graph.
2. First run of the  `DFS-Loop` to computer the "finish" time of each node in
   the reversed graph. Book keeping:
       - The "source node of a node". A source node is the first node in the DFS tree.
       - The "finish time of a node".
3. Second run of the `DFS-Loop` to computer the strongly connected components (SCC).
   The SCC is represented by the source ndoe.

### Minimum spanning tree

## Problems

### Critical Connections in a Network

### [Critical Routers](https://leetcode.com/discuss/interview-question/436073/)