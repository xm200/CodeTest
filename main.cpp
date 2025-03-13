#include <iostream>
#include "interval.h"
#include <variant>
#include "CFGparser.h"


int main(int argc, char *argv[]) {
#if defined(DEBUG_MODE)
    if (argc == 3)
        throw std::runtime_error("Proceed test generation mode");
#endif
    const std::string path = argv[argc - 1];
    parse::parser p(parse::read_file(path), (argv[1] == "DFS"));
    p.parse();
//    p.tree();
    return 0;
}