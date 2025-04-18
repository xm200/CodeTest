#include "../libraries/interval.h"
#include "../libraries/CFGparser.h"
bool test_similar(const std::vector<std::string> &a, const std::string &b) {
    std::ifstream inp(b);
    for (const auto & ind : a) {
        std::string line;
        if (!std::getline(inp, line)) {
            std::cerr << "different size" << std::endl;
            std::cerr << "***test failure***" << std::endl;
            inp.close();
            return false;
        }
        if (ind != line) {
            inp.close();
            return false;
        }
    }
    if (std::string line; std::getline(inp, line)) {
        std::cerr << "different size" << std::endl;
        std::cerr << "***test failure***" << std::endl;
        inp.close();
        return false;
    }
    inp.close();
    return true;
}
bool test_similar(const std::string &a, const std::string &b) {
    std::stringstream ss(a);
    std::vector<std::string> v;
    std::string buf;
    while (std::getline(ss, buf)) {
        v.push_back(buf);
    }
    return test_similar(v, b);
}

std::vector dif = {false, false, false, false, false, false, true};

int main(const int argc, char *argv[]) {
    std::cout << "***begin of test***" << std::endl;
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " test_num parser_algorithm" << std::endl;
        return EXIT_FAILURE;
    }
    if (argv[1] == std::string_view("1")) {
        const auto x = parse::read_file("Testing/test1_2.py", true, "--", true);
        if (!test_similar(*x, "Testing/test1.a")) return EXIT_FAILURE;
    }
    else if (argv[1] == std::string_view("2")) {
        parse::parser p
            (parse::read_file("Testing/test1_2.py", true, "--", true),
            (argv[2] != std::string_view("bfs")), true, true);
        p.parse();
        const auto x = p.tree();
        if (!test_similar(x, "Testing/test2.a")) return EXIT_FAILURE;
    }
    else if (std::string_view(argv[1]).size() == 1 && argv[1][0] >= '3' && argv[1][0] <= '7') {
        std::string path = "Testing/test" + std::string(argv[1]);

        parse::parser p
        (parse::read_file(path + ".py", true,
            "Testing/cache/output.txt", false),
        (argv[2] != std::string_view("bfs")), true, false);

        p.parse();

        parse::cache.write_to_file();

        std::ifstream inp("Testing/cache/output.txt");
        std::string line; std::vector<std::string> v;
        while (std::getline(inp, line)) {
            v.push_back(line);
        }
        if (dif[argv[1][0] - '1']) {
            if (!test_similar(v, path + "_" + std::string(argv[2]) + ".a")) return EXIT_FAILURE;
        }
        else {
            if (!test_similar(v, path + ".a")) return EXIT_FAILURE;
        }
    }
    else {
        std::cerr << "Usage: " << argv[0] << " test_num parser_algorithm" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "***test done***" << std::endl;
    return EXIT_SUCCESS;
}