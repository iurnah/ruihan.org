# Topological Sort

## Algorithm

* __Motivation:__ sequence tasks while respecing all precedence constraints.
* If graph G __exists__ a directed cycle, no topological ordering.
* Every directed acyclic graph must have a __sink__ vertex.

=== "The psudo code"

```text
DFS-Loop (graph G)
-- mark all nodes unexplored
-- current_label = n [to keep track of ordering]
-- for each vertex
    -- if v not yet explored [in previous DFS call ]
    -- DFS(G,v)

DFS(graph G, start vertex s)
-- for every edge (s,v)
    -- if v not yet explored
    -- mark v explored
    -- DFS(G,v)
-- set f(s) = current_label
-- current_label = current_label - 1
```

One possible improvement is to use three flags to distinguish the `non-visited`,
`visiting`, and `visited` nodes. It detects circle when run into a vertex marked
as `visiting` as the current vertex is in `visiting` state. Because in DAG, the
property of DFS states that __a descendant have to be visited before its
predecessor finished__. For example, DFS start from $s$, and just discovered
vertex $v$, recursive call on vertex $v$ discovred the $s$ in "visiting" state,
this indicate there exists a cycle.

## BFS Topological sorting

Apply BFS in DAG topological sorting requires to start with the vertices that
have `0` indegree, namely the source vertices. We need a __queue__ and a `indegree`
vector for solving topological sorting problems. The BFS queue may start with
more than one source vertices because topological sorted graph may start with
two or more vertices (indegree == 0). When BFS visit an adjacent vertex of the
queue-front vertex, the indegree of this visited vertex should be decreased by 1,
as we "visited" this arc. If the indegree of the visited vertex is reduced to 0,
we push it to the queue. The idea of using BFS in topological sorting is to explore
frontier of the vertices that currently have `0` non-visited arcs leading to the
vertices (indegree is 0).

* What if the given graph have a single vertex?
* How to check whether the topological sorting is unique or not?
* How to output all the unique topological sorted sequence of vertices?

### Course Schedule

* Solution 1 DFS
* Solution 2 BFS

=== "C++ DFS"

    ```c++
    class Solution {
    public:
        bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) {
            vector<vector<int>> graph(numCourses, vector<int>(0));
            vector<int> visit(numCourses, 0);

            /* construct the graph in adjacency list */
            for (auto a : prerequisites) {
                graph[a.second].push_back(a.first);
            }

            for (int i = 0; i < numCourses; ++i) {
                if (!canFinishDFS(graph, visit, i))
                    return false;
            }
            return true;
        }

        bool canFinishDFS(vector<vector<int>>& graph, vector<int>& visit, int i) {
            if (visit[i] == -1) return false; // visiting
            if (visit[i] == 1) return true;

            visit[i] = -1;
            for (auto a : graph[i]) {
                if (!canFinishDFS(graph, visit, a))
                    return false;
            }
            visit[i] = 1;
            return true;
        }
    };
    ```

=== "C++ BFS"

    ```c++
    class Solution {
    public:
        bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) {
            vector<vector<int>> graph(numCourses, vector<int>(0));
            vector<int> in(numCourses, 0);
            for (auto a : prerequisites) {
                graph[a.second].push_back(a.first);
                ++in[a.first]; /* indegree of nodes */
            }

            queue<int> q;
            /* locate the "start" of the directed acyclic graph */
            for (int i = 0; i < numCourses; ++i) {
                if (in[i] == 0) q.push(i);
            }

            while (!q.empty()) {
                int t = q.front(); q.pop();
                for (auto a : graph[t]) {
                    --in[a]; /* visit the edge t->a */
                    if (in[a] == 0) q.push(a);
                }
            }

            /* if there are cycle, (with node indegree > 0) */
            for (int i = 0; i < numCourses; ++i) {
                if(in[i] != 0) return false;
            }

            return true;
        }
    };
    ```

### Course Schedule II

* Solution 1 DFS
* Solution 2 BFS

=== "C++ DFS"

    ```c++
    class Solution {
    public:
        vector<int> findOrder(int numCourses, vector<pair<int, int>>& prerequisites) {
            int n = prerequisites.size();
            vector<vector<int>> graph(numCourses, vector<int>(0));
            vector<int> visit(numCourses, 0); /* 0: not visited, 1: visiting, -1: visited */
            vector<int> res;

            for (auto a : prerequisites) {
                graph[a.second].push_back(a.first);
            }

            for (int i = 0; i < numCourses; ++i) {
                if (!visit[i] && !findOrderDFS(graph, visit, i, res)) {
                    return {};
                }
            }

            reverse(res.begin(), res.end());

            return res;
        }

        bool findOrderDFS(vector<vector<int>>& graph, vector<int>& visit, int i, vector<int>& res) {
            if (visit[i] == 1) return false; /* visiting */
            if (visit[i] == -1) return true;

            visit[i] = 1;
            for (auto a : graph[i]) {
                if (!findOrderDFS(graph, visit, a, res)) {
                    return false;
                }
            }

            visit[i] = -1;
            res.push_back(i);
            return true;
        }
    };
    ```

=== "C++ BFS"

    ```c++
    class Solution {
    public:
        vector<int> findOrder(int numCourses, vector<pair<int, int>>& prerequisites) {
            vector<vector<int>> graph(numCourses, vector<int>(0));
            vector<int> in(numCourses, 0);
            vector<int> res;
            // identify the sink edge by indegree
            for (auto a : prerequisites) {
                graph[a.second].push_back(a.first);
                ++in[a.first];
            }

            queue<int> q;
            for (int i = 0; i < numCourses; ++i) { //push the "sink" vetex to the queue
                if (in[i] == 0) q.push(i);
            }

            while (!q.empty()) {
                int t = q.front(); q.pop();
                res.push_back(t);
                for (auto a : graph[t]) {
                    --in[a];
                    if (in[a] == 0) q.push(a); // add a new sink vertex
                }
            }
            // circle detection
            for (int i = 0; i < numCourses; ++i) {
                if (in[i] != 0) return {};
            }

            return res;
        }
    };
    ```

### Alien Dictionary

* Solution 1 Topological Sort
  * use DFS to get the topological order
  * use BFS to traverse to get the result

=== "C++ Topological sort"

    ```c++
    class Solution {
    public:
        string alienOrder(vector<string>& words) {
            set<pair<char, char>> g; // adj list graph, second -> first,
            unordered_set<char> set; // all unique characters set
            vector<int> in(256, 0);  // indegree for each chars

            string res = "";

            for (auto a : words) // store all unique char of the dict words
                set.insert(a.begin(), a.end());

            for (int i = 0; i < words.size() - 1; i++) {
                int min_len = min(words[i].size(), words[i + 1].size()), j;
                for (j = 0; j < min_len; j++) {
                    if (words[i][j] != words[i + 1][j]) { // build graph from the dictionary
                        g.insert({words[i][j], words[i + 1][j]}); // word[0][0] is the sink vertex
                        break; // take only one
                    }
                }
            }

            // calculate indegree of nodes
            for (auto c : g)
                ++in[c.second];

            // push sink node to the queue
            queue<int> q;
            for (char c : set) {
                if (in[c] == 0) {
                    q.push(c);
                    res += c; // sink vertex added to result
                }
            }

            while (!q.empty()) { // BFS to output the sorted order
                char a = q.front(); q.pop();
                for (auto c : g) {
                    if (c.first == a) { // for each incomming edge of sink node a.
                        --in[c.second]; // here we treat the topological order c.second -> c.first
                        if (in[c.second] == 0) {
                            q.push(c.second);
                            res += c.second;
                        }
                    }
                }
            }

            return res.size() == set.size() ? res : "";
        }
    };
    ```

### Sequence Reconstruction

* Solution 1 Topological Sort
* How to check whether the topological sorting is unique or not?
* How to output all the unique topological sorted sequence of vertices?

=== "C++ Topological sort"

    ```c++
    class Solution {
    public:
        bool sequenceReconstruction(vector<int>& org, vector<vector<int>>& seqs) {
            int n = org.size();
            int m = seqs.size();

            vector<vector<int>> graph(n + 1, vector<int>(0));
            vector<int> in(n + 1, 0);
            bool empty = true;
            for (auto seq : seqs) {
                if (seq.empty()) continue;
                if (seq.size() < 2 && (seq[0] < 1 || seq[0] > n)) return false;
                empty = false;
                for (int i = 0; i < seq.size() - 1; ++i) {
                    int u = seq[i];
                    int v = seq[i + 1];
                    if (u < 1 || u > n || v < 1 || v > n) return false;
                    graph[u].push_back(v); // build the graph
                    in[v]++; // compute the indegree
                }
            }

            if (empty) return false;

            queue<int> q;
            for (int i = 1; i <= n; ++i) {
                if (in[i] == 0) {
                    q.push(i);
                }
            }

            int k = 0;
            while (!q.empty()) { // BFS to compute the topological order
                // inorder to get a unique sequence, the q.size() should always be 1,
                // this means that the topological order is unique, consider the first example
                if (q.size() > 1) return false;
                int t = q.front(); q.pop();
                if (t != org[k++]) return false;
                for (auto a : graph[t]) {
                    in[a]--;
                    if (in[a] == 0) q.push(a);
                }
            }

            return k == n;
        }
    };
    ```
