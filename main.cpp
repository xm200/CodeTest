#include <iostream>
#include "interval.h"
#include <variant>
#include "CFGparser.h"


int main(int argc, char *argv[]) {
#if !defined(DEBUG_MODE)
    if (argc == 1)
        throw std::runtime_error("Proceed path or start program with --help");
#endif
    const std::string path = "test.py";
    parse::parser p(parse::read_file(path), false);
    p.parse();
    p.tree();
    return 0;
}