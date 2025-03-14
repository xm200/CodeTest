#include <iostream>
#include "CFGparser.h"
#include <bits/stdc++.h>
struct custom_type {
    std::any data;
    std::string name = "undefined name";
    std::function<bool(const custom_type &, const custom_type &)> less = [this](const custom_type &a, const custom_type &b)->bool{
        throw std::logic_error("operator < is not defined for type " + name);
    };
};
int main(int argc, char *argv[]) {
    const std::string path = "test.py";
    if (argc != 2) {
        std::cout << "usage: CodeTest {DFS/BFS}";
        throw std::runtime_error("not correct usage");
    }
    parse::parser p(parse::read_file(path), (argv[1] == std::string_view("DFS")));
    p.parse();
    p.tree();
    return 0;
}