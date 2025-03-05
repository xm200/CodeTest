#include <iostream>
#include "interval.h"
#include <queue>
#include <vector>
#include <memory>
#include <any>
#include <variant>
#include "CFGparser.h"


struct NonStandartTypeNode {
    interval::interval<std::unique_ptr<std::any>> base;
};


struct StandartTypeNode {
    std::vector<std::variant<int, float, double, long long, long double, unsigned int, unsigned long long>> ranges; // As you asked to do
    interval::interval<int> code_range;
};


struct Graph {
    std::vector<std::unique_ptr<std::any>> ptrs; // pointers on Nodes
    std::vector<bool> used; // Array of used Nodes
    std::vector<std::vector<int>> g;

    Graph() = default;
    ~Graph() = default;

    void add_node(std::any *n) {
        ptrs.push_back(std::make_unique<std::any>(n));
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
    std::string path = R"(C:\Users\glebf\CLionProjects\CodeTest2\main.cpp)";
    std::cout << parse::compiler_command(path);
    return 0;
}