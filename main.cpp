#include <iostream>
#include "interval.h"
#include <queue>
#include <vector>


template <class T>
struct Node {
    interval::interval<T> range; // Node With code and interval
};

struct Graph {
    std::vector<int> ptrs; // pointers on Nodes
    std::vector<bool> used; // Array of used Nodes
    std::vector<std::vector<int>> g;

    Graph(int v) {
        ptrs.resize(v);
        used.resize(v);
        g.resize(v);
    }

    void _dfs(int start) {
        if (used[start]) return;
        used[start] = true;
        for (int &x: g[start]) _dfs(x);
    }

    void dfs(int start) {
        for (int i = 0; i < used.size(); ++i) used[i] = false;
        _dfs(start);
    }

    void _bfs(int start) {
        std::queue<int> q;
        q.push(start);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (used[v]) continue;
            used[v] = true;
            for (int x : g[start]) q.push(x);
        }
    }

    void bfs(int start) {
        for (int i = 0; i < used.size(); ++i) used[i] = false;
        _bfs(start);
    }
};
int main() {
    return 0;
}