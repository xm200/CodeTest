#ifndef CFG_H
#define CFG_H


#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <set>
#include "ASTExperiment.h"


namespace parse {

    inline std::string file_name, file_type;

    enum os {
        LINUX, MACOS, WIN
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
                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            else if constexpr (get_os() == LINUX || get_os() == MACOS) path = way + '/';
                // ReSharper disable once CppDFAUnreachableCode
            else path = way + '\\';
            out.open(path + "output.txt");
                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            if constexpr (get_os() == LINUX || get_os() == MACOS) path += "cache/";
                // ReSharper disable once CppDFAUnreachableCode
            else path += "cache\\";
            system(("mkdir " + path).c_str());
            inited = true;
        }

        std::ofstream& write_path() { return out; }

        ~cache_t() {
            if (!inited) return;
#if !defined(DO_NOT_REMOVE_CACHE)
                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            if constexpr (get_os() == LINUX || get_os() == MACOS) system(("rm -rf " + path).c_str());
                // ReSharper disable once CppDFAUnreachableCode
            else system(("rd /s /q " + path).c_str()); // Remove dir

            write_to_file();

            out.close();
#endif
        }

        std::set<std::string>& get_tests_set() { return tests; }

        const std::string &operator()() const {
#if defined(DEBUG_MODE)
            if (!inited) throw std::logic_error("cache dir does not exists");
#endif
            return path;
        }

       void write_to_file() {
            for (const auto& it : tests) {
                if (it.empty()) continue;
                out << it << '\n';
            }
       }

    private:
        std::string path;
        bool inited = false;
        std::ofstream out;
        std::set<std::string> tests;
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

    struct node_t {
        std::string name;
        std::vector<node_t *> children{};
        std::vector<custom::custom_type> variables{};
        size_t l{}, len{};
        node_t(std::string n, const std::size_t b, const std::size_t e) : name(std::move(n)), l(b), len(e) {}
        node_t() = default;
    };

    static short get_interval_type(const custom::custom_type &b) {
        if (std::get_if<interval::interval<typeInt>>(&b.data.value()) != nullptr) return custom::custom_type::types::INT;
        if (std::get_if<interval::interval<typeFloat>>(&b.data.value()) != nullptr) return custom::custom_type::types::FLOAT;
        if (std::get_if<interval::interval<std::string>>(&b.data.value()) != nullptr) return custom::custom_type::types::INT;
        return custom::custom_type::types::NONE;
    }

    class parser {
        const std::vector<std::string> *code;
    public:
        explicit parser(const std::vector<std::string> *code_, const bool gm) {
            code = code_;
            root = new node_t("root", 0, code->size());
            graph_mode = gm;
        }

        void parse() {
            if (graph_mode) parse(root, {{}});
            else parse_bfs();
        }

        void tree() const {
            std::string s;
            tree(root, s);
            std::cout << std::flush;
        }

        static ast::variables_t translate(const custom::str_type &s, const ast::variables_t &orig) {
            auto check = [](const custom::str_type &s, const std::string_view it) {
                return s.size() >= it.size() && s.substr(0, it.size()).extract() == (it) &&
                    (s[it.size()] == ' ' || s[it.size()] == '(');
            };
            auto ans = orig;
            std::vector<char> boo = {'>', '<', '=', '!'};
            for (int i = 0; i < s.size(); ++i) {
                switch (s[i]) {
                    case '=': {
                        auto _s = custom::erase_spaces(s.substr(i + 1));

                        if (check(_s, "int") || check(_s, "float") || check(_s, "input")) {
                            auto *x = new custom::custom_type;
                            x->name = custom::erase_spaces(s.substr(0, i - 1)).extract();

                            if (check(_s, "int")) {
                                interval::interval<typeInt> buf;
                                buf.add_interval(interval::minimal<typeInt>(), interval::maximal<typeInt>());
                                x->data = buf;
                            }

                            if (check(_s, "float")) {
                                interval::interval<typeFloat> buf;
                                buf.add_interval(interval::minimal<typeFloat>(), interval::maximal<typeFloat>());
                                x->data = buf;
                            }

                            if (check(_s, "input")) {
                                interval::interval<std::string> buf;
                                buf.add_interval(interval::minimal<std::string>(), interval::maximal<std::string>());
                                x->data = buf;
                            }

                            x->reset_type();
                            for (auto &j : ans) {
                                bool found = false;
                                for (auto &k : j) {
                                    if (k->name == x->name) {
                                        x->history = {0, k};
                                        k = x;
                                        found = true;
                                    }
                                }
                                if (!found)
                                    j.push_back(x);
                            }
                            return ans;
                        }

                        if ((i + 1 < s.size() &&
                                std::find(boo.begin(), boo.end(), s.extract()[i + 1]) != boo.end()) ||
                            (i != 0 && std::find(boo.begin(), boo.end(), s.extract()[i - 1]) != boo.end())) break;

                        auto root = ast::generate_ast(s.substr(i + 1)); // a = 3, b = 4
                        auto buf = root->get_variables(orig);

                        for (auto &var: buf) {
                            auto res = var.front();
                            auto *x = new custom::custom_type;
                            *x = *res;
                            res->name = custom::erase_spaces(s.substr(0, i)).extract();
                            res->history = std::pair<std::size_t, custom::custom_type*>(0, x);

                            for (auto &k : ans) {
                                bool found = false;
                                for (auto &j : k) {
                                    if (j->name == custom::erase_spaces(s.substr(0, i)).extract()) { j = res; found = true; break; }
                                }
                                if (found) continue;
                                k.push_back(res);
                                std::sort(k.begin(), k.end());
                            }
                        }
                        return ans;
                    }
                    case '+':
                    case '-':
                    case '*':
                    case '/': {
                        if (s[i + 1] != '=' || (s[i] == '/' && s.substr(i, 3).extract() != "//=")) break;
                        const auto left = s.substr(0, i);
                        const auto right = s.substr(i + 2);
                        const std::string buf = "(" + custom::erase_spaces(right).extract() + ") " + (s[i] == '/' ? "//" : std::string(1, s[i])) + " " + custom::erase_spaces(left).extract();
                        custom::str_type res(buf);
                        return translate(res, orig);
                    }
                    default: break;
                }
            }
            return {};
        }

    protected:
        node_t *root;
        bool graph_mode;

        static short get_type(const custom::str_type &buf) {
            for (int i = 0; i < buf.size(); ++i) {
                if (buf.substr(i, 4).extract() == "int(" || buf.substr(i, 4).extract() == "int ") return custom::custom_type::types::INT;
                if (buf.substr(i, 6).extract() == "float(" || buf.substr(i, 6).extract() == "float ") return custom::custom_type::types::FLOAT;
                if (buf.substr(i, 6).extract() == "input(" || buf.substr(i, 6).extract() == "input ") return custom::custom_type::types::STRING;
            }
            return custom::custom_type::extract_type_from_string(buf.extract());
        };

        void parse_bfs() const noexcept {
            ast::variables_t vars;
            std::vector<custom::custom_type> vars_orig;
            std::queue<std::pair<node_t *, ast::variables_t>> q;
            q.emplace(root, vars);

            while (!q.empty()) {
                auto [fst, snd] = q.front();
                q.pop();

                for (auto i = fst->l; i < fst->l + fst->len; ++i) {
                    std::string first_word;
                    auto expr = std::stringstream(code->operator[](i));
                    expr >> first_word;

                    switch (get_construction_type(first_word)) {
                        case IF:
                        case ELIF:
                        case ELSE:
                        case FOR:
                        case WHILE: {
                            fst->children.push_back(new node_t(
                                    code->operator[](i).substr(get_spaces(code->operator[](i))),
                                    i + 1,
                                    get_code_block(i + 1)));
                            const custom::str_type buf(code->operator[](i));
                            auto a = ast::generate_ast(buf);
                            q.emplace(fst->children.back(), a->get_variables(vars));
                            i += get_code_block(i + 1);
                            break;
                        }
                        default:
                            auto buf = code->operator[](i);
                            custom::str_type res(buf);
                            auto asd = translate(res, vars);
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

        init_struct_cession(what, std::string from_any_to_str(custom::custom_type::inner_type &what))
        enable_all_types(from_any_to_str, what)
        close_cession(T)
        std::string from_any_to_str(custom::custom_type::inner_type &what) {
            std::stringstream ss;
            ss << std::get<interval::interval<T>>(what.value()).any().value();
            return ss.str();
        }

        void parse(node_t *node, const ast::variables_t &vars, const size_t depth = 0) {
            ast::variables_t _vars = vars;

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
                        custom::str_type buf(code->operator[](i)); // "else: "
                        buf = erase_spaces(buf);
                        custom::str_type buf_fw(first_word); // "else: "
                        const auto root = ast::generate_ast(erase_spaces(buf.substr
                            (buf_fw.size(), buf.size() - 1 - buf_fw.size())));
                        parse(child,  root->get_variables(_vars), depth + 1);
                        i += child->len;
                        break;
                    }
                    default: {
                        auto buf = code->operator[](i);
                        custom::str_type res(buf);
                        auto buffer_vars = translate(res, _vars);
                        if (!buffer_vars.empty())
                            _vars = buffer_vars;
                        break;
                    }
                }
            }

            for (auto &x : _vars) {
                for (const auto &y : x) {
                    cache.get_tests_set().insert(from_any_to_str(y->data));
                }
            }
        }
#undef sub
    private:
        enum search {
            IF, ELIF, ELSE, FOR, WHILE, ANOTHER
        };

        static void tree(node_t *_root, const std::string &move) {
            std::cout << move << _root->name + '\n';
            auto move_new = move;
            if (!move_new.empty()) {
                for (auto i = move_new.size(); i --> move_new.size() - 4; )
                    move_new[i] = '.';
            }
            move_new += "|---";
            for (const auto &vertex : _root->children) {
                tree(vertex, move_new);
            }
        }

        // ANOTHER - operators, types, functions calls and all that can be in the code
        [[ nodiscard ]] static int get_construction_type(const std::string &buf) {
            if (buf == "if") return IF;
            if (buf == "elif") return ELIF;
            if (buf == "else" || buf == "else:") return ELSE;
            if (buf == "for") return FOR;
            if (buf == "while") return WHILE;
            return ANOTHER;
        }
    };
}
#undef init_cession
#undef init_struct_cession
#undef enable_all_types
#undef enable_int
#undef enable_float
#undef enable_string
#undef close_cession

#endif //CFG_H
