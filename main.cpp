#include "CFGparser.h"
#include <iostream>

bool m = true;
enum attributes {
    HELP, UNKNOWN_ATTR
};
static int get_attr(const std::string &val) {
    if (val == "help") return HELP;
    return UNKNOWN_ATTR;
}

[[noreturn]] void help(const std::string &s) {
    std::cout << "usage: CodeTest [-B] path_to_file\n";
    std::cout << "option -B: use BFS (default using DFS algo)\n";
    std::cout << std::flush;
    throw std::runtime_error(s);
}


int main(const int argc, char *argv[]) {

    if (argc == 1 || (argc == 2 && argv[1] == std::string_view("--help"))) {
        help("arguments are needed");
    }

    for (auto i = 1; i < argc - 1; i++) {
        // --attr
        if (const std::string arg = argv[i]; arg.size() > 2 && arg.substr(0, 2) == "--") {
            switch (get_attr(arg.substr(2))) {
                case HELP:
                    help("arguments are needed");
                default:
                    help("unknown attribute: " + arg);
            }
        }
        // -attr (attr have 1 symbol)
        else if (arg.size() == 2 && arg[0] == '-') {
            switch (arg[1]) {
                case 'B': {
                    m = false;
                    break;
                }
                default: {
                    help("unrecognized option: " + arg);
                }
            }
        }
        // unknown data
        else {
            help("unknown argument: " + arg);
        }
    }



    const std::string path = argv[argc - 1];
    parse::parser p(parse::read_file(path), m);
    p.parse();
    p.tree();
    return 0;
}
