#include <iostream>
#include "interval.h"
#include <queue>
#include <vector>
#include <memory>
#include <any>


struct Node {
    std::vector<std::any> ranges; // As you asked to do
    interval::interval<int> code_range;
};


struct Graph {
    std::vector<std::unique_ptr<Node>> ptrs; // pointers on Nodes
    std::vector<bool> used; // Array of used Nodes
    std::vector<std::vector<int>> g;

    Graph() = default;
    ~Graph() = default;

    template<class T>
    void add_Node(Node *n) {
        ptrs.push_back(std::make_unique<Node>(Node({n->ranges, n->code_range})));
        used.push_back(false);
        g.emplace_back();
    }

    void _dfs(int start) {
        if (used[start]) return;
        used[start] = true;
        for (int &x: g[start]) _dfs(x);
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
        for (auto && i : used) i = false;
        _bfs(start);
    }

    void dfs(int start) {
        for (auto && i : used) i = false;
        _dfs(start);
    }
};



int main() {
    std::cout << "aboba";
    return 0;
}