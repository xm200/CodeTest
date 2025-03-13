#include <iostream>
#include "interval.h"
#include <queue>
#include <vector>
#include <memory>
#include <any>
#include <variant>
#include "CFGparser.h"


struct NonStandardTypeNode {
    interval::interval<std::unique_ptr<std::any>> base;
};


struct StandardTypeNode {
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
int main(int argc, char *argv[]) {
    const std::string path = "test.py";
//#if defined(DEBUG_MODE)
//    if (argc < 2) {
//        throw std::logic_error("Proceed test generation mode");
//    }
//#endif
    parse::parser p(parse::read_file(path), (argv[1] == "DFS"));
    p.parse();
//    p.tree();
    parse::basic_variable test(s);
    test.print();
    return 0;
}