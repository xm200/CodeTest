#include <iostream>
#include "interval.h"
#include "ASTExperiment.h"
#include "CFGparser.h"

void print_help_menu() { /// todo: normal help
    std::cout << "[] Usage : .\\CodeTest.exe [path to your file]\n";
}

int main(int argc, char *argv[]) {
    if (argc == 1)
        throw std::runtime_error("Proceed path or start program with --help");
    for (int i = 0; i < argc; ++i) {
        const std::string buf = argv[i];
        if (buf == "--help") {
            print_help_menu();
            abort();
        }
    }
    const std::string path = argv[1];
    parse::parser p(parse::read_file(path), false);
    p.parse();
    p.tree();
//    parse::non_basic_variable test("[[1, 2] 3]");
    variable a("ab");
    variable b("oba");
    (a + b).get_val();
    BinaryOperation c(&a, &b, '+');
    c.get_value();
    return 0;
}