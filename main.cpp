#include "interval.h"
#include "ASTExperiment.h"
#include "CFGparser.h"
#include <iostream>

bool m = true;
bool v = false;
std::string output_file;

enum attributes {
    HELP, BFS, DFS, VERBOSE, OUTPUT, UNKNOWN_ATTR
};

static int get_attr(const std::string &val) {
    if (val == "help") return HELP;
    if (val == "bfs") return BFS;
    if (val == "dfs") return DFS;
    if (val == "verbose") return VERBOSE;
    if (val == "output") return OUTPUT;
    return UNKNOWN_ATTR;
}

[[noreturn]] void help(const std::string &s) {
    std::cout << "usage: CodeTest [attributes] path_to_file\n";
    std::cout << "option -B / --bfs: use BFS (default using DFS algo)\n";
    std::cout << "option -D / --dfs: use DFS \n";
    std::cout << "option -v / --verbose: print additional info \n";
    std::cout <<
        "option -o path / --output path: path to output file (default using output.txt in dir with file for debug) \n";
    std::cout << std::flush;
    throw std::runtime_error(s);
}

int main(const int argc, char *argv[]) {
#if defined(DEBUG_MODE)
    if (v) std::cout << "You now in debug mode" << std::endl;
#endif
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
                case VERBOSE:
                    v = true;
                    break;
                case OUTPUT:
                    output_file = argv[++i];
                    if (i == argc - 1) help("you need to write input file");
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
                case 'v': {
                    v = true;
                    break;
                }
                case 'o':
                    output_file = argv[++i];
                    if (i == argc - 1) help("you need to write input file");
                    break;
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

    parse::parser p(parse::read_file(path, v, output_file), m, v);
    p.parse();
    p.tree();

    const std::string line = "a > 3 or (a < 4 and b > 5)";
    const std::string abc = "a = 3 * 3 + 123456 % 12345";
    // std::cout << line << std::endl;
    // const custom::str_type x(line);
    // auto node = ast::generate_ast(x);
    // node->tree();
    return 0;
}