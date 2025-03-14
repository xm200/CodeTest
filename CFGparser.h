//
// Created by xm200 on 05.03.2025. =uwu=
//

#ifndef CODE_TEST_CFG_PARSER_H
#define CODE_TEST_CFG_PARSER_H

#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <any>
#include <queue>

namespace parse {

    inline std::string file_name, file_type;

    enum os {
        LINUX, MACOS, WIN
    };

    enum operators {
        LT, GT, LE, GE, EQ, NE, PLE, PWE, DE, ME, DDE, MLE, NOT_OPERATOR
    };

    constexpr short get_os() {
        #if defined(__linux__)
            return LINUX;
        #elif defined(_WIN32) || defined(_WIN64)
            return WIN;
        #elif defined(__APPLE__) || defined(__MACH__)
            return MACOS;
        #endif
    }

    struct cache_t {
        void init(const std::string &way) {
            #if defined(DEBUG_MODE)
                if (inited) {
                    if (way + '/' == path || way == path || way + '\\' == path) return;
                    throw std::logic_error("cache with multiple initialization and different paths");
                }
            #endif

            if (way.empty()) path = way;
            else if constexpr (get_os() == LINUX || get_os() == MACOS) path = way + '/';
            else path = way + '\\';

            if constexpr (get_os() == LINUX || get_os() == MACOS) path += "cache/";
            else path += "cache\\";
            system(("mkdir " + path).c_str());
            inited = true;
        }

        ~cache_t() {
            if (!inited) return;
            #if !defined(DO_NOT_REMOVE_CACHE)
                if constexpr (get_os() == LINUX || get_os() == MACOS) system(("rm -rf " + path).c_str());
                else system(("rd /s /q " + path).c_str()); // Remove dir
            #endif
        }

        const std::string &operator()() const {
            #if defined(DEBUG_MODE)
                if (!inited) throw std::logic_error("cache dir does not exists");
            #endif
            return path;
        }

    private:
        std::string path;
        bool inited = false;
    } inline cache;


    inline std::vector<std::string> *read_file(const std::string &path) {
        std::size_t ind = -1, point = path.size();
        for (auto i = path.size(); i --> 0;) {
            if (point == path.size() && path[i] == '.') point = i;
            if (path[i] == '/' || path[i] == '\\') {
                ind = i;
                break;
            }
        }

        cache.init(path.substr(0, (ind == -1 ? 0 : ind)));
        file_name = path.substr(ind + 1, point - ind - 1), file_type = path.substr(point);

        std::string buf;
        auto *out = new std::vector<std::string>;
        std::ifstream file(path);
        while (std::getline(file, buf)) out->push_back(buf);
        file.close();

        #if defined(VERBOSE)
            std::cout << "path to cache: " << cache() << '\n';
            std::cout << "file name: " << file_name << '\n';
            std::cout << "file type: " << (file_type.empty() ? "*none*" : file_type) << '\n';
            std::cout << "code have " << out->size() << " lines\n";
        #endif

        return out;
    }

    struct basic_variable {
        [[nodiscard]] inline static char tolower(const char c) { return (c > 'A' && c < 'Z') ? static_cast<char> (c - 'a' + 'A') : c; }

        void generate_value() {} /// todo: ask fedor

        void print() {
            if (const int *pvi = std::get_if<int>(&start_value)) { std::cout << *pvi; delete pvi; }
            else if (const double *pvd = std::get_if<double>(&start_value)) { std::cout << *pvd; delete pvd; }
            else if (const int *pvs = std::get_if<int>(&start_value)) { std::cout << *pvs; delete pvs; }
            std::cout << ' ';
        }

        basic_variable() = default;

        explicit basic_variable (const std::string &value_from_code) { start_value = mega_cast(value_from_code); }

        ~basic_variable() = default;
    private:
        std::variant<int, double, std::string> start_value;
        std::vector<std::string> changes;
        enum types { INT, DOUBLE, STRING, UNKNOWN };

    protected:
        // get string with value - return type of value
        [[nodiscard]] static inline std::variant<int, double, std::string> mega_cast(const std::string &s) {
            // cast variable to normal type
            switch (get_type(s)) {
                #if defined(DEBUG_MODE)
                    case UNKNOWN: throw std::logic_error("Function mega_cast : unknown type found!");
                #endif
                case INT: return std::stoi(s);
                case DOUBLE: return std::stod(s);
                default: return s;
            }
        }

        [[nodiscard]] static short get_type(const std::string &s) {
            if (s.find("\'") != -1 || s.find("\"") != -1) return STRING;
            if (s.find('.') == -1) return INT;
            if (s.find('.') != -1) return DOUBLE;
            else return UNKNOWN;
        }

    };

    struct variables {
        std::map<std::string, std::variant<basic_variable>> v{};

        inline void add_var(const std::string &name, const std::string &val) {
            v[name] = basic_variable(val); /// todo: add compose structures saving
        }

        variables() = default;
        ~variables() = default;

    };

    struct node_t {
        std::string name;
        std::vector<node_t *> children{};
        variables vars{};
        size_t l{}, len{};
        node_t(std::string n, const std::size_t b, const std::size_t e) : name(std::move(n)), l(b), len(e) {}
        node_t() = default;
    };


    class parser {
        const std::vector<std::string> *code;
    public:
        explicit parser(const std::vector<std::string> *code_, const bool gm) {
            code = code_;
            root = new node_t("root", 0, code->size());
            graph_mode = gm;
        }

        void parse() {
            if (graph_mode) parse(root);
            else parse_bfs();
        }

        void tree() {
            std::string s;
            tree(root, s);
            std::cout << std::flush;
        }

    protected:
        node_t *root;
        bool graph_mode;

        [[ nodiscard ]] inline static short get_op(const std::string &buf) {
            std::map<std::string, short> op = {{"<", LT}, {">", GT}, {"<=", LE}, {">=", GE}, {"==", EQ}, {"**=", PWE},
                                               {"!=", NE}, {"+=", PLE}, {"-=", ME}, {"/=", DDE}, {"*=", MLE}, {"//=", DE}};
            try { return op[buf]; }
            catch (const std::string &err) { return NOT_OPERATOR; }
        }

        static void parse_expr(const std::string &buf, node_t &node) { /// todo: think about +=, -=, ..., not only assign
            auto ss = std::stringstream(buf);
            std::vector<std::string> part1, part2;
            bool part = false; // false - first, true - second
            std::string word;
            while (ss >> word) {
                if (get_op(word) != EQ) part = true;
                /// todo: make a += 3 -> a = a + 3; b += a + 3 -> b = b + a + 3;

                if (!part) part1.emplace_back(word);
                else part2.emplace_back(word);
            }
            /// todo: make equation solver

        };

        void parse_bfs() {
            std::queue<node_t *> q;
            q.push(root);

            while (!q.empty()) {
                node_t *_root = q.front();
                q.pop();

                for (auto i = _root->l; i < _root->l + _root->len; ++i) {
                    std::string first_word;
                    auto expr = std::stringstream(code->operator[](i));
                    expr >> first_word;

                    switch (get_construction_type(first_word)) {
                        case IF:
                        case ELIF:
                        case ELSE:
                        case FOR:
                        case WHILE: {
                            _root->children.push_back(new node_t(
                                    code->operator[](i).substr(get_spaces(code->operator[](i))),
                                    i + 1,
                                    get_code_block(i + 1)));
                            q.push(_root->children.back());
                            i += get_code_block(i + 1);
                            break;
                        }
                        default:
                            parse_expr(code->operator[](i), *_root);
                            break;
                    }
                }
            }
        }

        [[ nodiscard ]] static size_t get_spaces(const std::string &buf) {
            size_t spaces = 0;
            for (auto &s : buf) if (s == ' ') ++spaces; else break;
            return spaces;
        }

        [[ nodiscard ]] size_t get_code_block(const size_t l = 0) const {
        #if defined(DEBUG_MODE)
                if (l >= code->size()) throw
                        std::length_error("Unreachable start limit in function get_code_block()");
        #endif

            const size_t block_spaces = get_spaces(code->operator[](l));
            for (size_t i = l; i < code->size(); ++i) if (get_spaces(code->operator[](i)) < block_spaces) return i - l;
            return code->size() - l;
        }

        void parse(node_t *node, const size_t depth = 0) {
            #if defined(DEBUG_MODE)
                if (node->l >= code->size()) throw
                            std::length_error("Unreachable start limit in function parse()");
                if (node->l + node->len > code->size()) throw
                            std::length_error("Unreachable end limit in function parse()");
            #endif

            for (auto i = node->l; i < node->l + node->len; ++i) {
                std::string first_word;
                auto expr = std::stringstream(code->operator[](i));
                expr >> first_word;

                switch (get_construction_type(first_word)) {
                    case IF:
                    case ELIF:
                    case ELSE:
                    case FOR:
                    case WHILE: {
                        auto *child = new node_t(
                                code->operator[](i).substr(get_spaces(code->operator[](i))),
                                i + 1,
                                get_code_block(i + 1));
                        node->children.push_back(child);
                        parse(child, depth + 1);
                        i += child->len;
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    private:
        enum search {
            IF, ELIF, ELSE, FOR, WHILE, ANOTHER
        };

        static void tree(node_t *_root, std::string &move) {
            std::cout << move << _root->name + '\n';
            auto move_new = move;
            if (!move_new.empty()) {
                for (auto i = move_new.size(); i --> move_new.size() - 4; )
                    move_new[i] = '.';
            }
            move_new += "|---";
            for (auto &vertex : _root->children) {
                tree(vertex, move_new);
            }
        }

        // ANOTHER - operators, types, functions calls and all that can be in the code
        [[ nodiscard ]] static inline int get_construction_type(const std::string &buf) {
            if (buf == "if") return IF;
            if (buf == "elif") return ELIF;
            if (buf == "else" || buf == "else:") return ELSE;
            if (buf == "for") return FOR;
            if (buf == "while") return WHILE;
            return ANOTHER;
        }
    };
}
#endif //CODE_TEST_CFG_PARSER_H
