#include "libraries/interval.h"
#include "libraries/CFGparser.h"
#include <iostream>

bool m = true;
bool v = false;
int cfg_print = 0;
std::string output_file;

enum attributes {
    HELP, BFS, DFS, VERBOSE, OUTPUT, CFG_PRINT, CFG_PRINT_ONLY, UNKNOWN_ATTR
};

static int get_attr(const std::string &val) {
    if (val == "help") return HELP;
    if (val == "bfs") return BFS;
    if (val == "dfs") return DFS;
    if (val == "verbose") return VERBOSE;
    if (val == "output") return OUTPUT;
    if (val == "cfg") return CFG_PRINT;
    if (val == "cfg-only") return CFG_PRINT_ONLY;
    return UNKNOWN_ATTR;
}

[[noreturn]] void help(const std::string &s) {
    std::cout << "usage: CodeTest [attributes] path_to_file\n";
    std::cout << "option -B / --bfs: use BFS\n";
    std::cout << "\tdefault using DFS algo\n";
    std::cout << "option -D / --dfs: use DFS\n";
    std::cout << "option -v / --verbose: print additional info\n";
    std::cout << "option -o path / --output path: path to output file\n";
    std::cout << "\tdefault using output.txt in dir with testing file\n";
    std::cout << "\tif you write -o -- / --output --, generated data will be written to stdout\n";
    std::cout << "option -c / --cfg: print CFG tree\n";
    std::cout << "option -C / --cfg-only: print CFG tree without generating data\n";
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
                case CFG_PRINT:
                    cfg_print = 1;
                    break;
                case CFG_PRINT_ONLY:
                    cfg_print = 2;
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
                case 'c':
                    cfg_print = 1;
                    break;
                case 'C':
                    cfg_print = 2;
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

#if defined(DEBUG_MODE)
    if (v) std::cout << "You now in debug mode" << std::endl;
#endif

    if (cfg_print == 2 && !output_file.empty()) help("You cannot set output file now");

    parse::parser p(parse::read_file(path, v, output_file, (cfg_print == 2)), m, v, (cfg_print == 2));
    p.parse();
    if (cfg_print) {
        p.tree();
    }
    return 0;
}
