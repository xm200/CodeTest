#include <iostream>
#include "interval.h"
#include "CFGparser.h"

bool m = true;

enum attributes {
    HELP, BFS, DFS, UNKNOWN_ATTR
};

static int get_attr(const std::string &val) {
    if (val == "help") return HELP;
    if (val == "bfs") return BFS;
    if (val == "dfs") return DFS;
    return UNKNOWN_ATTR;
}

[[noreturn]] void help(const std::string &s) {
    std::cout << "usage: CodeTest [attributes] path_to_file\n";
    std::cout << "option -B / --bfs: use BFS (default using DFS algo)\n";
    std::cout << "option -D / --dfs: use DFS \n";
    std::cout << std::flush;
    throw std::runtime_error(s);
}

int main(int argc, char *argv[]) {

    if (argc == 1 || (argc == 2 && argv[1] == std::string_view("--help"))) {
        help("arguments are needed");
    }

    for (auto i = 1; i < argc - 1; i++) {
        // --attr
        if (const std::string arg = argv[i]; arg.size() > 2 && arg.substr(0, 2) == "--") {
            switch (get_attr(arg.substr(2))) {
                case HELP:
                    help("arguments are needed");
                case BFS:
                    m = false;
                    break;
                case DFS:
                    m = true;
                    break;
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
                case 'D': {
                    m = true;
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

    interval::interval<typeInt> buf, buf2;
    buf.add_interval(interval::minimal<typeInt>(),123);
    buf2.add_point(1223);

    custom::custom_type a(buf);
    custom::custom_type b(buf2);
    a = a + b;
    a += b;

    std::cout << std::get<interval::interval<typeInt>>(a.data).print();
    return 0;
}