# Breadth-First Search

## Algorithm

BFS search a graph by first explore all the vertices that are adjacent to the
source vertex `s`, then explore the adjacent vertices of those already examined
in layer-by-layer fashion until all the vertices are visited.

```text tab="The psudo code"
BFS (graph G, start vertex s)
[ all nodes initially unexplored ]
-- mark s as explored
-- let Q = queue data structure, initialized with s
-- while Q not empty:
    -- remove the first node of Q, call it v
    -- for each edge(v,w) :
        -- if w unexplored
            -- mark w as explored
            -- add w to Q (at the end)
```

## BFS properties

1. Runtime complexity $\Theta(|V| + |E|)$.
2. It calculate __shortest path__ from the source $s$ to the vertex $v$.
3. It build a breadth-first tree, CLRS called predecessor subgraph.
4. If edge $(u, v)$ in the BFS tree, the simple path from source vertext $s$ to
   $u$ is equal to the simple path from $s$ to $v$, or differ by 1.
5. Simple path from a source vertex $s$ to a destination vertex $v$ is equal to
   the later’s __level label__.
6. If $v_1, v_2, \cdots, v_n$ is the vertex enqueued in a certain point, $v_1$
   is the head and $v_n$ is the back, then the level label of $v_1$ is either
   equal to level label of $v_n$ or less by 1. Also the level label of all the
   vertices in the queue is non-decreasing from head to tail.
   __In other words, at a certain time the enqueued vertices at most comes from two different levels.__

## Search

1. unweighted edges
2. Bi-directional search, (Speed up Dijkastra of 6.006)
3. At all time, the level of nodes in queue at most differ by 1
4. The relationship between BFS and DFS (The Maze II)

!!! Note "BFS tips for board and grid problems"
    1. When push a pair of coordinate to a BFS queue, one way of doing it is using
       `pair<int, int>`. Another way is to convert to a number by `p = i * n + j`,
        and retrieve the coordinate by `i = p / n` and `j = p % n`.
    2. The BFS starting point and "target" are important, give a second thought
       about where should the search start and how it terminate.
    3. `visitted[m][n]` is not necessary sometimes. See whether you can modify
       the original borad/matrix/grid/maze to reflect whether the element has
       already been accessed or not.
    4. In a borad or maze like input, searching neighbor cells requires you to
       manipulate indexes for accessing its neighbors. We do this by defining two
       "offset" coordinates arrays. `int x[4] = {-1, 0, 1, 0}; int y[4] = {0, 1, 0, -1};`
    5. The BFS queue doesn't necessarily start with one element pushed. It could
       be __"multi-end" BFS__. Like in problme [Pacific Atlantic Water Flow](#pacific-atlantic-water-flow)
       and [Longest Increasing Path in a Matrix](#longest-increasing-path-in-a-matrix),
       one could init the queue with all possible starting node then in use a while
       to visit all the node towards finding the optimal path.
    6. The "multi-end" BFS not only can make the BFS level by level (each round
       of for loop visited one level of nodes), but also can make use some optimal
       feature of BFS, like the BFS solution of Walls and Gates.
    7. The BFS while loop could include another while loop to interate throught
       total `q.size()` nodes. This is different from the "oracle" BFS solution
       from CRLS, but sometime could be useful. For example: [Longest Increasing Path in a Matrix](#longest-increasing-path-in-a-matrix).

### The Maze

```C++ tab="DFS"
class Solution {
public:
    bool hasPath(vector<vector<int>>&maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size();
        int n = m ? maze[0].size() : 0;
        vector<vector<bool>> visited(m, vector<bool>(n, false));

        return dfs_helper(maze, start, destination, visited);
    }

    void dfs_helper(vector<vector<int>>& maze, vector<int>& start,
                    vector<int>& destination, vector<vector<bool>>& visited) {

        if (start[0] == destination[0] && start[1] == destination[1]) {
            return true;
        }

        int x[4] = {0, -1, 0, 1};
        int y[4] = {-1, 0, 1, 0};

        bool res = false;
        for (int k = 0; k < 4; k++) {
            int a = start[0] + x[k];
            int b = start[1] + y[k];

            // walk untill hit the wall
            while (a >= 0 && b >= 0 && a < maze.size() && b < maze[0].size90 && maze[a][b] == 0) {
                a += x[k];
                b += y[k];
            }

            if (!visited[a - x[k]][b - y[k]]) {
                visited[a - x[k]][b - y[k]] = true;
                vector<int> new_start({a - x[k], b - y[k]});
                res != dfs_helper(maze, new_start, destination, visited);
            }
        }

        return res;
    }
};
```

```C++ tab="BFS"
class Solution {
public:
    bool hasPath(vector<vector<int>>&maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size();
        int n = m ? maze[0].size() : 0;

        vector<vector<bool>> visited(m, vector<bool>(n, false));
        queue<vector<int>> q;
        q.push(start);

        int x[4] = {0, -1, 0, 1};
        int y[4] = {-1, 0, 1, 0};

        while (!q.empty()) {
            vector<int> s = q.front(); q.pop();

            if (s[0] == destination[0] && s[1] == destination[1]) {
                return true;
            }

            for (int k = 0; k < 4; k++) {
                int a = s[0] + x[k];
                int b = s[1] + y[k];

                while (a >= 0 && b >= 0 && a < m && b < n && maze[a][b] == 0) {
                    a += x[k];
                    b += y[k];
                }

                if (!visited[a - x[k]][b - y[k]]) {
                    q.push(vector<int>({a - x[k], b - y[k]}));
                    visited[a - x[k]][b - y[k]] = true;
                }
            }
        }

        return false;
    }
};
```

### The Maze II

* BFS can computer shortest path, we need additional memory to keep the optimal values.
* we update the corresponding destination cell in `distance` vector when we found
  a smaller distane can reach it from any direction. the final result will be in
  `distance[destination[0]][destination[1]]`.
* If we don't want to use so much memory, we can use a flag to mark when the BFS
  switch levels.
* This solution is essentially to use BFS to solve the weighted graph problem as
  Victor mentioned in 6006 R15.
* Compare this solution to the Dijkastra solution. The update distance matrix is
  essentially the relaxiation step.

```C++ tab=""
class Solution {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size();
        int n = m ? maze[0].size() : 0;
        if (m == 0) return 0;

        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        queue<vector<int>> q;
        q.push(start);
        distance[start[0]][start[1]] = 0;

        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};

        while (!q.empty()) {
            vector<int> s = q.front(); q.pop();

            for (int k = 0; k < 4; k++) {
                int a = s[0] + x[k];
                int b = s[1] + y[k];
                int count = 0;
                while (a >= 0 && b >= 0 && a < m && b < n && maze[a][b] == 0) {
                    a += x[k];
                    b += y[k];
                    count++;
                }

                // notice here is a optimize
                if (distance[s[0]][s[1]] + count < distance[a - x[k]][b - y[k]]) {
                    distance[a - x[k]][b - y[k]] = distance[s[0]][s[1]] + count;
                    q.push(vector<int>({a - x[k], b - y[k]})); /* could use emplace({a - x[k], b - y[k]}) */
                }
            }
        }

        return distance[destination[0]][destination[1]] == INT_MAX ? -1 : distance[destination[0]][destination[1]];
    }
};
```

!!! Note
    - Based on the BFS solution, we can use a priority queue. Then the problem is very similar to the Dijkastra algorithm.
    - The discussion about the improvement in leetcode forum is very interesting.

### The Maze III

* This problem is aksing for output the shortest path to reach the destination.
* The idea is to use a `distance[i][i]` and `result[i][j]`. we should keep update
  the `distance[i][j]` and `result[i][j]` with smaller distance and lexicographical
  order. The hardest part is that there is two constrains. While using BFS, How
  could we ensure the two constrains are met, and also we are not missing any
  possible path. (i.e. missed to push a path to the BFS queue.)
* A trick we can make use of is we searching in the lexicographically order. such
  as `down`, `left`, `right`, `up`. However, we face a problem: the __smallest
  lexicon order result may not be the shortest distance__. Think this through.
* We need to used additional memory to record that piece of information beside
  the memory used to keep the minimum distance.

```C++ tab="BFS" hl_lines="54"
class Solution {
public:
    string findShortestWay(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size();
        int n = m ? maze[0].size() : 0;

        if (m == 0) return 0;

        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        vector<vector<string>> result(m, vector<string>(n, ""));
        queue<vector<int>> q;
        q.push(start);
        distance[start[0]][start[1]] = 0;

        char path[4] = {'d', 'l', 'r', 'u'};
        int x[4] = {1, 0, 0, -1};
        int y[4] = {0, -1, 1, 0};

        while (!q.empty()) {
            vector<int> s = q.front(); q.pop();

            for (int k = 0; k < 4; k++) {
                int a = s[0] + x[k];
                int b = s[1] + y[k];
                int count = 0;
                /* continue if no wall */
                while (a >= 0 && b >= 0 && a < m && b < n && maze[a][b] == 0) {
                    /* run over the destination */
                    if (a == destination[0] && b == destination[1]){
                        if (distance[s[0]][s[1]] + count < distance[a][b]) {
                            distance[a][b] = distance[s[0]][s[1]] + count;
                            result[a][b] = result[s[0]][s[1]] + path[k];
                        } else if (distance[s[0]][s[1]] + count == distance[a][b]) {
                            string tmp = result[s[0]][s[1]] + path[k];
                            /* same distance, smaller lexicon */
                            if (tmp.compare(result[a][b]) < 0) {
                                result[a][b] = tmp;
                            }
                        }
                    }

                    a += x[k];
                    b += y[k];
                    count++;
                }

                /* hit a wall, have to retreat "0", current a, b is in wall or off maze */
                if (distance[s[0]][s[1]] + count < distance[a - x[k]][b - y[k]]) {
                    distance[a - x[k]][b - y[k]] = distance[s[0]][s[1]] + count;
                    result[a - x[k]][b - y[k]] = result[s[0]][s[1]] + path[k];
                    q.push(vector<int>({a - x[k], b - y[k]}));
                } else if (distance[s[0]][s[1]] + count == distance[a - x[k]][b - y[k]]) {
                    string tmp = result[s[0]][s[1]] + path[k];
                    /* same distance, smaller lexicon */
                    if (tmp.compare(result[a - x[k]][b - y[k]]) < 0) {
                        result[a - x[k]][b - y[k]] = tmp;
                        /* possible path, need to search it again */
                        q.push(vector<int>({a - x[k], b - y[k]}));
                    }
                }
            }
        }

        return distance[destination[0]][destination[1]] == INT_MAX ? "impossible" : result[destination[0]][destination[1]];
    }
};
```

```C++ tab="DFS"
class Solution {
public:
    string findShortestWay(vector<vector<int>>& maze, vector<int>& ball, vector<int>& hole) {
        return roll(maze, ball[0], ball[1], hole, 0, 0, 0, "", pair<string, int>() = {"impossible", INT_MAX});
    }

    string roll(vector<vector<int>>& maze, int rowBall, int colBall, const vector<int>& hole,
                int distRow, int distCol, int steps, const string& path, pair<string, int>& res)
    {
        if (steps < res.second) {
            if (distRow != 0 || distCol != 0) {
                while ((rowBall + distRow) >= 0 && (rowBall + distRow) < maze.size() &&
                       (colBall + distCol) >= 0 && (colBall + distCol) < maze[0].size() &&
                       maze[rowBall + distRow][colBall + distCol] != 1) {

                    rowBall += distRow;
                    colBall += distCol;
                    ++steps;
                    if (rowBall == hole[0] && colBall == hole[1] && steps < res.second) {
                        res = {path, steps};
                    }
                }
            }

            if (maze[rowBall][colBall] == 0 || steps + 2 < maze[rowBall][colBall]) {
                maze[rowBall][colBall] = steps + 2;
                if (distRow == 0) roll(maze, rowBall, colBall, hole, 1, 0, steps, path + "d", res);
                if (distCol == 0) roll(maze, rowBall, colBall, hole, 0, -1, steps, path + "l", res);
                if (distCol == 0) roll(maze, rowBall, colBall, hole, 0, 1, steps, path + "r", res);
                if (distRow == 0) roll(maze, rowBall, colBall, hole, -1, 0, steps, path + "u", res);
            }
        }

        return res.first;
    }
};
```

```C++ tab="Dijkstra algoirthm"
class Vertex
{
public:
    int x, y, dist;
    std::string path;
    Vertex(int x, int y, int dist, std::string path)
    {
        this->x = x;
        this->y = y;
        this->dist = dist;
        this->path = path;
    }
    Vertex(int x, int y)
    {
        this->x = x;
        this->y = y;
        this->dist = INT_MAX;
        this->path = "";
    }
    bool equals(Vertex v)
    {
        return this->x == v.x && this->y == v.y;
    }
    bool equals(int x, int y)
    {
        return this->x == x && this->y == y;
    }
};
class Solution {
private:
    // coordinates and path strings for the four directions.
    std::vector<int> directions = {-1, 0, 1, 0, -1};
    std::vector<std::string> paths = {"u", "r", "d", "l"};
    // can't roll if it's boundry, wall or hole.
    bool canRoll(vector<vector<int>> &maze, Vertex hole, int x, int y) {
        int m = maze.size(), n = maze[0].size();
        if (x >= 0 && x < m && y >= 0 && y < n)
            return maze[x][y] != 1 && !hole.equals(x, y);
        else
            return false;
    }

    Vertex roll(vector<vector<int>> &maze, Vertex curr, Vertex hole, int dx,
                int dy, std::string path) {
        int x = curr.x, y = curr.y, steps = curr.dist;
        while (canRoll(maze, hole, x + dx, y + dy))
        {
            x += dx;
            y += dy;
            steps++;
        }
        // if couldn't roll due to hole, then return vertex corresponding to the hole.
        if (hole.equals(x + dx, y + dy))
        {
            x += dx;
            y += dy;
        }
        return Vertex(x, y, steps, curr.path + path);
    }
public:
    /*
     * to form a pq based on the increasing order of distances, and if same
     * distance then sorting based on lexicographic order.
     */
    struct Compare
    {
        bool operator()(const Vertex &a, const Vertex &b)
        {
            return a.dist > b.dist || a.dist == b.dist && a.path > b.path;
        }
    };
    /*
     * Applying Dijkstra's algorithm here.
     * We treat hole as a gate here. The trick is look for holes while rolling
     * the back in a particular direction. So, the pq here contains the vertices
     * before gates as well as the holes. So, using dijkstra, when we pop a hole
     * from pq, we know its that.
     */
    string findShortestWay(vector<vector<int>>& maze, vector<int>& ball, vector<int>& hole) {
        int m = maze.size(), n = maze[0].size();
        std::priority_queue<Vertex, std::vector<Vertex>, Compare> pq;
        Vertex startVertex(ball[0], ball[1], 0, ""), holeVertex(hole[0], hole[1]);
        std::unordered_set<int> visited;
        pq.push(startVertex);
        while (!pq.empty())
        {
            Vertex curr = pq.top();
            visited.insert(curr.x * n + curr.y);
            pq.pop();
            if (curr.equals(holeVertex))
            {
                return curr.path;
            }
            for (int i = 0; i < directions.size() - 1; i++)
            {
                Vertex newStart = roll(maze, curr, holeVertex, directions[i], directions[i + 1], paths[i]);
                if (visited.find(newStart.x * n + newStart.y) == visited.end())
                    pq.push(newStart);
            }
        }
        return "impossible";
    }
};
```

### Pacific Atlantic Water Flow

* Using DFS and two matrices to record the states and then to loop through the
  result check for both possible flow cases.
* The key is how to start the DFS? should we initiate the DFS in each `(i, j)`?
  Here we can see it is more efficient if that we start from the edges. Notice
  we mark `visited[i][j] = true` immediately enter the helper function.

```C++
class Solution {
public:
    vector<pair<int, int>> pacificAtlantic(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        vector<vector<int>> p_visited(m, vector<int>(n, 0));
        vector<vector<int>> a_visited(m, vector<int>(n, 0));
        vector<pair<int, int>> res;

        for (int i = 0; i < m; i++) {
            helper(matrix, i, 0, p_visited);
            helper(matrix, i, n - 1, a_visited);
        }

        for (int j = 0; j < n; j++) {
            helper(matrix, 0, j, p_visited);
            helper(matrix, m - 1, j, a_visited);
        }
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (p_visited[i][j] && a_visited[i][j]) {
                    //res.push_back(pair<int, int>(i, j));
                    res.push_back({i, j});
                }
            }
        }

        return res;
    }

    void helper (vector<vector<int>>& mat, int i, int j, vector<vector<int>>& visited) {
        int m = mat.size();
        int n = m ? mat[0].size() : 0;
        int x[] = {0, -1, 0, 1};
        int y[] = {-1, 0, 1, 0};

        visited[i][j] = true; /* visite mat[i][j] first */
        /* and then explore its neighbors */
        for (int k = 0; k < 4; k++) {
            int p = i + x[k];
            int q = j + y[k];
            if (p >= 0 && p < m && q >= 0 && q < n && !visited[p][q] && mat[i][j] <= mat[p][q]) {
                helper(mat, p, q, visited);
            }
        }
    }
};
```

```C++ tab="DFS solution"
class Solution {
public:
    vector<pair<int, int>> pacificAtlantic(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        vector<vector<bool>> vPacific(m, vector<bool>(n, false));
        vector<vector<bool>> vAtlantic(m, vector<bool>(n, false));
        queue<pair<int, int>> qPacific;
        queue<pair<int, int>> qAtlantic;

        for (int i = 0; i < m; ++i) {
            qPacific.push({i, 0});
            vPacific[i][0] = true;
            qAtlantic.push({i, n - 1});
            vAtlantic[i][n - 1] = true;
        }

        for (int i = 0; i < n; ++i) {
            qPacific.push({0, i});
            vPacific[0][i] = true;
            qAtlantic.push({m - 1, i});
            vAtlantic[m - 1][i] = true;
        }

        bfs_helper(matrix, qPacific, vPacific);
        bfs_helper(matrix, qAtlantic, vAtlantic);

        vector<pair<int, int>> res;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (vPacific[i][j] && vAtlantic[i][j]) {
                    res.push_back({i, j});
                }
            }
        }

        return res;
    }

    void bfs_helper(vector<vector<int>>& matrix, queue<pair<int, int>>& q, vector<vector<bool>>& visited) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;
        int x[4] = {0, -1, 0, 1};
        int y[4] = {-1, 0, 1, 0};

        while (!q.empty()) {
            int a = q.front().first;
            int b = q.front().second;
            q.pop();
            for (int k = 0; k < 4; ++k) {
                int u = a + x[k];
                int v = b + y[k];

                if (u < 0 || u >= m || v < 0 || v >= n || visited[u][v] || matrix[a][b] > matrix[u][v]) continue;
                visited[u][v] = true;
                q.push({u, v});
            }
        }
    }
};
```

!!! warning
    Notice your terminate condition `matrix[a][b] > matrix[u][v]` in the DFS
    solution indicates your search direction cannot not go from high to low. The
    search direction is from low to high (or equal). This is to optimize the
    search since we don't have to revisite a node later.

### Longest Increasing Path in a Matrix

BFS solution

* Each cell should possibly be the starting cell of the longest increasing path.
  So we have to search starting from each one of the cells. It seems the running
  time is quadratic. How to optimize?
* use markers matrix to mark whether the `matrix[i][j]` have greater neighbors,
  `left`, `top`, `right`, `bottom` -> 1, 2, 4, 8.
* The do BFS only looking for the directions that have greater neighbors. The
  idea is very similar to the "branch cutting" in DFS.

```C++
class Solution {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        vector<vector<int>> markers(m, vector<int>(n, 0));

        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};
        int res = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < 4; k++) {
                    int p = i + x[k];
                    int q = j + y[k];
                    if (p < 0 || p >= m || q < 0 || q >= m) continue;
                    if (matrix[p][q] > matrix[i][j])
                        markers[i][j] ^= (1 << k);
                }
            }
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int level = 0;
                if (markers[i][j] > 0) {
                    level = 0;
                    queue<int> q;
                    q.push(i * n + j);
                    while (!q.empty()) {
                        int a = q.front() / n;
                        int b = q.front() % n;
                        q.pop();
                        bool flag = true;
                        for (int k = 0; k < 4; k++) {
                            int u = a + x[k];
                            int v = b + y[k];
                            if (u < 0 || u >= m || v < 0 || v >= m) continue;
                            if (has_greater_neighbor(markers[a][b], k)) {
                                q.push(u * n + v);
                                if (flag) {
                                    level++;
                                    flag = false;
                                }
                            }
                        }
                    }
                }
                res = max(res, level);
            }
        }

        return res;
    }

    bool has_greater_neighbor(int flags, int k) {
        return (flags >> k) & 1;
    }
};
```

BFS solution II

* BFS can find the shortest path. How can you use BFS to find the longest path?
  In this problem, we are not use BFS's shortest path properpy, we make use of
  BFS's property of finding the connected components. We can imagine each
  increasing path is a connected component, we need to find out the largest one.
  Notice how the solution construct the search structure (for loop inside the while loop).
* By construct the search structure in this way, the len is actually the level
  of nodes from all the peaks identified in the first nested for loops.

```Java tab="Java solution"
class Solution {
    public int longestIncreasingPath(int[][] matrix) {
        private final int[][] dirs = {{1, 0}, {-1, 0},{0, 1}, {0, -1}};
        private boolean ispeak(int[][] matrix, boolean[][] marked, int i, int j) {
            if (i > 0 && !marked[i-1][j] && matrix[i-1][j] > matrix[i][j]) return false;
            if (i < matrix.length-1 && !marked[i+1][j] && matrix[i+1][j] > matrix[i][j]) return false;
            if (j > 0 && !marked[i][j-1] && matrix[i][j-1] > matrix[i][j]) return false;
            if (j < matrix[0].length-1 && !marked[i][j+1] && matrix[i][j+1] > matrix[i][j]) return false;
            return true;
        }
        public int longestIncreasingPath(int[][] matrix) {
            if (matrix.length == 0 || matrix[0].length == 0) return 0;
            int len = 0;
            LinkedList<int[]> queue = new LinkedList<>();
            boolean[][] marked = new boolean[matrix.length][matrix[0].length];
            for (int i = 0; i < matrix.length; i++) {
                for (int j = 0; j < matrix[0].length; j++) {
                    if (ispeak(matrix, marked, i, j)) queue.add(new int[]{i, j});
                }
            }
            while (!queue.isEmpty()) {
                len++;
                int size = queue.size();
                for (int i = 0; i < size; i++) {
                    int[] p = queue.poll();
                    marked[p[0]][p[1]] = true;
                    for (int j = 0; j < 4; j++) {
                        int r = p[0]+dirs[j][0], c = p[1]+dirs[j][1];
                        if (r >= 0 && r < matrix.length && c >= 0 && c < matrix[0].length && !marked[r][c] && ispeak(matrix, marked, r, c)) {
                            if (matrix[r][c] != matrix[p[0]][p[1]]) queue.add(new int[]{r, c});
                        }
                    }
                }
            }
            return len;
        }
    }
}
```

!!! Note
    This BFS is not the same as the classic BFS routine from CLRS. It is a
    generalized BFS approach, which can be transformed to a problem to find the
    connected components.

DFS solution

* we can also use DFS for this problem, it turns out DFS is the simpliest solution.

```C++ tab="DFS solution"
class Solution {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = m ? matrix[0].size() : 0;

        if (n == 0) return 0;
        vector<vector<int>> cache(m, vector<int>(n, 0));
        int res = 1;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int len = dfs_helper(matrix, i, j, m, n, cache);
                res = max(res, len);
            }
        }

        return res;
    }

    int dfs_helper(vector<vector<int>>& matrix, int i, int j, int m, int n, vector<vector<int>>& cache) {
        if (cache[i][j]) return cache[i][j];
        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};
        int res = 1;
        for (int k = 0; k < 4; k++) {
            int p = i + x[k];
            int q = j + y[k];
            if (p < 0 || p >= m || q < 0 || q >= n || matrix[p][q] <= matrix[i][j]) continue;
            int len = 1 + dfs_helper(matrix, p, q, m, n, cache);
            res = max(res, len);
        }
        cache[i][j] = res;
        return res;
    }
};
```

### Walls and Gates

BFS solution

* We push all the gate to the queue at once, then BFS to update the room. This
  makes use of the optimal character of BFS and bring us some optimization.
* Proof correctness: If init the queue with all gates, BFS alternate between
  gates, once BFS explore one level of rooms for a particular gate, it switches
  to explore the same level or next level rooms for another gate. The level is
  the distance from room to gate. Suppose room R1 is visited at the first time
  from G1, then if R1 is visited again via G2, the distance from G2 to R1 is
  possible to be the same as G1 to R1, or greater than G1 to R1 by 1. __The
  assigned nearest distance from room to gate never be able to reduce further in
  future.__ Then the first assigned distance value for a room will be the nearest one.

```C++ tab=""
class Solution {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int m = rooms.size();
        int n = m ? rooms[0].size() : 0;

        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};

        queue<pair<int, int>> q;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (rooms[i][j] == 0) { // start from each gate 
                    q.push({i, j});
                }
            }
        }

        while (!q.empty()) {
            int a = q.front().first;
            int b = q.front().second;
            q.pop();
            for (int k = 0; k < 4; k++) {
                int u = a + x[k];
                int v = b + y[k];
                if (u < 0 || u >= m || v < 0 || v >= n || rooms[u][v] != INT_MAX) {
                    continue;
                }

                rooms[u][v] = rooms[a][b] + 1;
                q.push({u, v});
            }
        }
    }
};
```

DFS solution

* I initially start with each `INF` cell and search for 0. the issue is we have
  to pass the original `i`, `j`, in case we reach the base case to update the
  `INF` cell.
* Another trick is how to handle the repeated searching the same empty room.
  Extra space like `visited[m][n]` will work. but a better way to handle it is
  what we did in the below solution. We check if `d > ma[i][j]`, if `d > 0`, the
  `d > mat[i][j]` also make sure we update `mat[i][j]` with a smaller value
  because the initial value of `INF` cell is `INT_MAX`. This is different from
  DFS because DFS can ensure the shortest path.
* The time complexity is $O(m^2n^2)$. The space complexity is from the stack. It
  is definitely not constant.

```C++
class Solution {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int m = rooms.size();
        int n = m ? rooms[0].size() : 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (rooms[i][j] == 0) { // start from each gate, not room
                    helper(rooms, i, j, 0);
                }
            }
        }
    }
    /* i, j are the starting point. */
    void helper(vector<vector<int>>& mat, int i, int j, int d) {
        int m = mat.size();
        int n = m ? mat[0].size() : 0;

        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};
  
        if (i < 0 || i >= m || j < 0 || j >= n || mat[i][j] < d) {
            return;
        }
        // have already made sure d < mat[i][j]
        mat[i][j] = d;
        for (int k = 0; k < 4; k++) {
            int a = i + x[k];
            int b = j + y[k];
            helper(mat, a, b, d + 1);
        }
    }
};
```

!!! Note "multi-end BFS"
    Notice this implementation of BFS is different from triditional BFS. It
    start with a queue that have more than one element. Some people call this
    [multi-end BFS](https://leetcode.com/problems/walls-and-gates/discuss/72748/benchmarks-of-dfs-and-bfs).
    It is perform better than the original BFS, The time complexity is $O(mn)$

### 01 Matrix

### Surrounded Regions

* We first mark the boundary 'O', then start BFS from each 'O'. If it cannot
  reach the special marks, we capture the cell.
* This solution is starting from each of inner 'O' and search the mark at the
  boundary. The solution is TLE due to more inner 'O's than boundary 'O's.

```C++
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        int m = board.size();
        int n = m ? board[0].size() : 0;
        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 || j == 0 || i == m - 1 || j == n - 1) {
                    board[i][j] = board[i][j] == 'O' ? '-' : 'X';
                }
            }
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'O') {
                    bool flag = true;
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    while (!q.empty()) {
                        int a = q.front().first, b = q.front().second;
                        q.pop();
                        for (int k = 0; k < 4; k++) {
                            int u = a + x[k];
                            int v = b + y[k];
                            if (u < 0 || u >= m || v < 0 || v >= n || board[u][v] == 'X') {
                                continue;
                            }

                            if (board[u][v] == 'O') {
                                q.push({u, v});
                            }

                            if (board[u][v] == '-') {
                                flag = false;
                                break;
                            }
                        }
                        if (!flag)
                            break;
                    }
                    if (flag)
                        board[i][j] = 'X';
                }
            }
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 || j == 0 || i == m - 1 || j == n - 1) {
                    board[i][j] = board[i][j] == '-' ? 'O' : 'X';
                }
            }
        }
    }
};
```

* Instead searching whether a 'O' have an outlet. We can discover all 'O's that
  lead to edge 'O's and mark them, then all the rest 'O's are captured.
* Specifically, starting from every 'O' at the edge, using BFS to search all the
  'O's outside the 'X's, all these 'O's that cannot be captured. Compare to
  solution 1, this ideas is thinking “out of the box”, instead of thinking inside
  'O's, we take care outside 'O's first.
* Similar problems: [Walls and Gates](#walls-and-gates), [Shortest Distance from All Buildings](#shortest-distance-from-all-buildings).

```C++
class Solution {
public:
    void solve(vector<vector<char>>& board) {
        int m = board.size();
        int n = m ? board[0].size() : 0;

        // bfs search from boundary 'O's
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 || j == 0 || i == m - 1 || j == n - 1) {
                    if (board[i][j] == 'O')
                        bfs_helper(board, i, j);
                }
            }
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'O') {
                    board[i][j] = 'X'; // capture all inner 'O's
                } else if (board[i][j] == '-') {
                    board[i][j] = 'O'; // flip back the marks
                }
            }
        }
    }

    // helper function to mark "outside" 'O' as '-'
    void bfs_helper(vector<vector<char>>& board, int i, int j) {
        int m = board.size();
        int n = m ? board[0].size() : 0;
        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};

        board[i][j] = '-';
        queue<pair<int, int>> q;
        q.push({i, j});
        while (!q.empty()) {
            int a = q.front().first, b = q.front().second;
            q.pop();
            for (int k = 0; k < 4; k++) {
                int u = a + x[k];
                int v = b + y[k];
                if (u >= 0 && u < m && v >= 0 && v < n && board[u][v] == 'O') {
                    q.push({u, v});
                    board[u][v] = '-';
                }
            }
        }
    }
};
```

!!! Notes "Tips
    以上两个 solution 足以说明BFS的搜索方向和搜索目标非常重要。巧妙转化搜索目标可以使得code很好写，更简洁。

### Best Meeting Point

* At the first attempt, I tried to use BFS to do search level by level, which
  can exhaust all the possible distances from 1 to 0. Unfortunately, it is TLE in the OJ.
* Following from the idea in problem Minimum Moves to [Equal Array Elements II](#equal-array-elements-ii),
  we can use math method to calculate the median of the x and y coordinates.
* The worst case time complexity is $O(mn\log mn)$, since there are at most $m\times n$ "1" in the grid.

```C++ "Median solution"
class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = m ? grid[0].size() : 0;

        vector<int> rows, cols;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    rows.push_back(i);
                    cols.push_back(j);
                }
            }
        }

        return minDistance(rows) + minDistance(cols);
    }

    int minDistance(vector<int> vec) {
        int n = vec.size();
        sort(vec.begin(), vec.end());
        int l = 0, r = n - 1;
        int res = 0;
        while (l < r) res += vec[r--] - vec[l++];

        return res;
    }
};
```

```C++ "Median solution II"
class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = m ? grid[0].size() : 0;

        vector<int> rows, cols;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    rows.push_back(i);
                    cols.push_back(j);
                }
            }
        }

        sort(cols.begin(), cols.end());
        int l = 0, r = rows.size() - 1;
        int res = 0;
        while (l < r)
            res += rows[r] - rows[l] + cols[r--] - cols[l++];

        return res;
    }
};
```

```C++ tab="BFS"
class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = m ? grid[0].size() : 0;
        int res = INT_MAX;

        vector<vector<int>> dist(m, vector<int>(n, 0));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    bfs_helper(grid, i, j, dist);
                }
            }
        }
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (res > dist[i][j]) {
                    res = dist[i][j];
                }
            }
        }

        return res;
    }

    void bfs_helper(vector<vector<int>>& grid, int i, int j, vector<vector<int>> &dist) {
        int m = grid.size();
        int n = m ? grid[0].size() : 0;

        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};

        vector<vector<bool>> visited(m, vector<bool>(n, false));

        int level = 1;
        queue<pair<int, int>> q;
        q.push({i, j});
        visited[i][j] = true;
        while (!q.empty()) {
            int len = q.size();
            for (int l = 0; l < len; l++) {
                int a = q.front().first;
                int b = q.front().second;
                q.pop();
                for (int k = 0; k < 4; k++) {
                    int u = a + x[k];
                    int v = b + y[k];
                    if (u >= 0 && u < m && v >= 0 && v < n && !visited[u][v]) {
                        dist[u][v] += level;
                        visited[u][v] = true;
                        q.push({u, v});
                    }
                }
            }
            level++;
        }
    }
};
```

### Number of Islands

### Battleships in a Board

### Bus Routes

### Word Ladder

### Minimum Genetic Mutation

### Word Ladder II

### Open the Lock

### Evaluate Devision

## Compute shortest path for undirected graphs

### [The Maze II](#the-maze-ii)

### Shortest Distance from All Buildings

* Similar to [Walls and Gates](#walls-and-gates), We should search from the "dst"
  (buildings) to "src" (empty land), while the problem is asking search from
  empty land to buildings.
* We need a 2D array to record the distance obtained by searching. The result is
  obtained at last after we finish the searching.
   Attention: we have also need to check whether the new building can reach all buildings. So use another 2D array and a counter num_buildings to check it can reach all.

```C++ tab=""
class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = m ? grid[0].size() : 0;

        int num_buildings = 0;
        vector<vector<int>> dist1(m, vector<int>(n, 0));
        vector<vector<int>> reach(m, vector<int>(n, 0));

        int x[4] = {-1, 0, 1, 0};
        int y[4] = {0, 1, 0, -1};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    num_buildings++;
                    queue<int> q;
                    vector<vector<bool>> visited(m, vector<bool>(n, false));

                    q.push(i * n + j);
                    int level = 1; // grid[a][b] is level 0, grid[u][v] is level 1
                    while (!q.empty()) {
                        int l = q.size();
                        for (int s = 0; s < l; s++) { // BFS level by level
                            int a = q.front() / n;
                            int b = q.front() % n;
                            q.pop();
                            for (int k = 0; k < 4; k++) {
                                int u = a + x[k];
                                int v = b + y[k];
                                if (u >= 0 && u < m && v >= 0 && v < n && grid[u][v] == 0 && !visited[u][v]) {
                                    dist1[u][v] += level;
                                    reach[u][v]++; // use to count num of 1s that reach this 0.
                                    visited[u][v] = true;
                                    q.push(u * n + v);
                                }
                            }
                        }
                        level++;
                    }
                }
            }
        }

        int res = INT_MAX;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0 && reach[i][j] == num_buildings) {
                    res = min(res, dist1[i][j]);
                }
            }
        }

        return res == INT_MAX ? -1 : res;
    }
};
```

* We can modify the original 2D array to record which cell is visited and which
  isn't so as to optimize the space complexity. Each time we start BFS from a
  building `grid[i][j]=1`, the land will be decreased by `1`. So after the first
  BFS, all `grid[i][j]=0` becomes `grid[i][j]=-1`. The decreasing of `grid[i][j]`
  is equivalent to use the visited variables.
* similar to Solution 1, we use a 2D array `dist[i][j]` to record the distances.
  This distance is `dist[i][j]` used to record each BFS started from a building
  (`grid[i][j]=1`), not the global distances. To accumulate all the distance from
  1s, we have to use another 2D array sum to record it.

```C++ tab="Optimized BFS solution" hl_lines="30,31,32,33"
class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = m ? grid[0].size() : 0;

        int res = INT_MAX;

        int counter = 0;
        vector<vector<int>> sum = grid;

        int x[4] = {0, -1, 0, 1};
        int y[4] = {-1, 0, 1, 0};

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) {
                    vector<vector<int>> dist = grid;
                    queue<int> q;
                    q.push(i * n + j);
                    res = INT_MAX;
                    while (!q.empty()) {
                        int a = q.front() / n;
                        int b = q.front() % n;
                        q.pop();
                        for (int k = 0; k < 4; ++k) {
                            int u = a + x[k];
                            int v = b + y[k];
                            if (u < m && u >= 0 && v < n && v >= 0 && grid[u][v] == counter) {
                                grid[u][v]--; // Mark this potential building position is visited in prev round
                                dist[u][v] = dist[a][b] + 1;
                                sum[u][v] += dist[u][v] - 1;
                                res = min(res, sum[u][v]);
                                q.push(u * n + v);
                            }
                        }
                    }
                    counter--; // mark as visited
                }
            }
        }

        return res == INT_MAX ? -1 : res;
    }
};
```

### Shortest Path Visiting All Nodes

### Shortest Path to Get All Keys

## Compute connected components for undirected graphs

### [Longest Increasing Path in a Matrix](#longest-increasing-path-in-a-matrix)

### Number of Connected Components in an Undirected Graph

* Build the undirected graph using adjacency list from the edge list representation.
* Call BFS in a for loop and use a `visited` vector to mark the status of the vertex explorations.
* Direct transform the above BFS solution to a DFS solution.

```C++ tab="BFS solution"
class Solution {
public:
    int countComponents(int n, vector<pair<int, int>>& edges) {
        vector<vector<int>> graph(n, vector<int>(0));
        vector<bool> visited(n, false);

        for (auto edge : edges) {
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }

        queue<int> q;
        int count = 0;
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                q.push(i);
                visited[i] = true;
                while (!q.empty()) {
                    int t = q.front(); q.pop();
                    for (auto a : graph[t]) {
                        if (!visited[a]) {
                            visited[a] = true;
                            q.push(a);
                        }
                    }
                }

                count++;
            }
        }

        return count;
    }
};
```

```C++ tab="DFS solution"
class Solution {
public:
    int countComponents(int n, vector<pair<int, int>>& edges) {
        vector<vector<int>> graph(n, vector<int>(0));
        vector<bool> visited(n, false);
        int count = 0;

        for (auto edge : edges) {
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfs_helper(graph, visited, i);
                count++;
            }
        }

        return count;
    }

    void dfs_helper(vector<vector<int>>& graph, vector<bool>& visited, int i) {
        visited[i] = true;
        for (auto a : graph[i]) {
            if (!visited[a]) {
                dfs_helper(graph, visited, a);
            }
        }
    }
};
```

```C++ tab="Union-Find solution"
class Solution {
    vector<int> parent;
public:
    int countComponents(int n, vector<pair<int, int>>& edges) {
        parent = vector<int>(n, -1);

        for (int i = 0; i < n; ++i) parent[i] = i;

        for (auto edge : edges) {
            int p = root(edge.first);
            int q = root(edge.second);
            if (p != q) parent[q] = p;
        }

        int count = 0;
        for (int i = 0; i < n; ++i) {
            if (parent[i] == i) {
                count++;
            }
        }

        return count;
    }

    int root(int i) {
        while (i != parent[i]) i = parent[i];
        return i;
    }
};
```

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