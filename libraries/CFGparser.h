#ifndef CFG_H
#define CFG_H


#include <fstream>
#include <iomanip>
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
        void init(const std::string &way, const bool v, const std::string &output_file, const bool _cfg_only) {
            cfg_only = _cfg_only;
            verbose = v;
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
            if (output_file.empty()) out_path = path + "output.txt";
            else out_path = output_file;
                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            if constexpr (get_os() == LINUX || get_os() == MACOS) path += "cache/";
                // ReSharper disable once CppDFAUnreachableCode
            else path += "cache\\";
            system(("mkdir " + path).c_str());
            if (out_path != "--") out.open(out_path);
            inited = true;
        }

        std::ofstream& write_path() { return out; }

        ~cache_t() {
            if (!inited) return;
            if (verbose && out_path == "--" && !cfg_only)
                std::cout << "starting writing generated data" << std::endl;
            write_to_file();

            if (verbose && out_path != "--" && !cfg_only)
                std::cout << "generated data had written to " << out_path << std::endl;

            if (out_path != "--") out.close();


#if !defined(DO_NOT_REMOVE_CACHE)
            // ReSharper disable once CppDFAUnreachableCode
            // ReSharper disable once CppRedundantBooleanExpressionArgument
            if constexpr (get_os() == LINUX || get_os() == MACOS) system(("rm -rf " + path).c_str());
            // ReSharper disable once CppDFAUnreachableCode
            else system(("rd /s /q " + path).c_str()); // Remove dir
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
                if (out_path == "--") std::cout << std::setprecision(13) << it << std::endl;
                else out << std::setprecision(13) << it << '\n';
            }
            if (out_path != "--") out << std::flush;
       }

    private:
        std::string path;
        std::string out_path;
        bool inited = false, verbose = false, cfg_only = false;
        std::ofstream out;
        std::set<std::string> tests;
    } inline cache;


    [[nodiscard]] inline std::vector<std::string> *read_file
    (const std::string &path, const bool v, const std::string &output_file, const bool cfg_only) {
        std::size_t ind = -1, point = path.size();
        for (auto i = path.size(); i --> 0;) {
            if (point == path.size() && path[i] == '.') point = i;
            if (path[i] == '/' || path[i] == '\\') {
                ind = i;
                break;
            }
        }

        cache.init(path.substr(0, ind == -1 ? 0 : ind), v, output_file, cfg_only);
        file_name = path.substr(ind + 1, point - ind - 1), file_type = path.substr(point);

        std::string buf;
        auto *out = new std::vector<std::string>;
        std::ifstream file(path);
        while (std::getline(file, buf)) out->push_back(buf);
        file.close();

        if (v) {
            std::cout << "path to cache: " << cache() << '\n';
            std::cout << "file name: " << file_name << '\n';
            std::cout << "file type: " << (file_type.empty() ? "*none*" : file_type) << '\n';
            std::cout << "code have " << out->size() << " lines\n";
        }
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

    init_cession(what, std::string from_any_to_str(const custom::custom_type::inner_type &what))
        enable_all_types(from_any_to_str, what)
        close_cession(T)
        std::string from_any_to_str(const custom::custom_type::inner_type &what) {
        auto x = std::get<interval::interval<T>>(what.value()).any();
        if (x == std::nullopt) return "";
        std::stringstream ss;
        ss << std::setprecision(13) << x.value();
        return ss.str();
    }

    class parser {
        const std::vector<std::string> *code;
    public:
        explicit parser(const std::vector<std::string> *code_, const bool gm, const bool v, const bool cfg_print) {
            code = code_;
            root = new node_t("root", 0, code->size());
            graph_mode = gm;
            verbose = v;
            cfg_print_only = cfg_print;
        }

        void parse() const {
            if (graph_mode) {
                parse(root, {{}});
                if (verbose) std::cout << "\r" "processing successful                " << std::endl;
            }
            else parse_bfs();
        }

        [[nodiscard]] std::string tree() const {
            std::string s;
            return tree(root, s);
        }

        [[nodiscard]] static ast::variables_t translate(const custom::str_type &s, const ast::variables_t &orig) {
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
                                std::find(boo.begin(), boo.end(), s[i + 1]) != boo.end()) ||
                            (i != 0 && std::find(boo.begin(), boo.end(), s[i - 1]) != boo.end())) break;

                        auto root = ast::generate_ast(s.substr(i + 1)); // a = 3, b = 4
                        auto buf = root->get_variables(orig);

                        for (auto &var: buf) {
                            auto res = var.front();
                            auto *x = new custom::custom_type;
                            *x = *res;
                            x->name = custom::erase_spaces(s.substr(0, i)).extract();
                            x->history = res->history;
                            x->parent = res->parent;

                            for (auto &k : ans) {
                                bool found = false;
                                for (auto &j : k) {
                                    if (j->name == custom::erase_spaces(s.substr(0, i)).extract())
                                        { j = x; found = true; break; }
                                }
                                if (found) continue;
                                k.push_back(x);
                                std::sort(k.begin(), k.end(),
                                    custom::dereferenced_sort_comparator<custom::custom_type>);
                            }
                        }
                        return ans;
                    }
                    case '+':
                    case '-':
                    case '*':
                    case '/': {
                        const auto left = s.substr(0, i);
                        const auto right = s.substr(i + ((s.substr(i, 3).extract() == "//=") ? 3 : 2));
                        const std::string buf = custom::erase_spaces(left).extract() + " = " + custom::erase_spaces(left).extract() + " " + (s[i] == '/' ? "//" : std::string(1, s[i])) + " (" + custom::erase_spaces(right).extract() + ")";
                        custom::str_type res(buf);
                        return translate(res, orig);
                    }
                    default: break;
                }
            }
            return {};
        }

    protected:
        node_t *root = nullptr;
        bool graph_mode, verbose, cfg_print_only;

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



        ast::variables_t parse(node_t *node, const ast::variables_t &vars, const size_t depth = 0) const {
            ast::variables_t _vars = vars;

#if defined(DEBUG_MODE)
            if (node->l >= code->size()) throw
                        std::length_error("Unreachable start limit in function parse()");
            if (node->l + node->len > code->size()) throw
                        std::length_error("Unreachable end limit in function parse()");
#endif

            ast::variables_t else_data;
            std::set<std::vector<custom::custom_type*>> tmp;
            for (auto i = node->l; i < node->l + node->len; ++i) {
                if (code->operator[](i).empty() || code->operator[](i)[0] == '#') continue;
                if (verbose) std::cout << '\r' << "line " << i + 1 << " / " << code->size() << std::flush;

                std::string first_word;
                auto expr = std::stringstream(code->operator[](i));
                expr >> first_word;
                if (first_word.front() == '#') continue;
                switch (auto fw_type = get_construction_type(first_word)) {
                    case IF:
                    case ELIF:
                    case ELSE:
                    // case FOR:
                    case WHILE: {
                        auto *child = new node_t(
                                code->operator[](i).substr(get_spaces(code->operator[](i))),
                                i + 1,
                                get_code_block(i + 1));
                        node->children.push_back(child);
                        custom::str_type buf(code->operator[](i)); // "else: "
                        buf = erase_spaces(buf);
                        custom::str_type buf_fw(first_word); // "else: "
                        ast::variables_t buf_vars;
                        if (!cfg_print_only) {
                            if (fw_type != ELSE) {
                                // if (!tmp.empty()) {
                                    // _vars.clear();
                                    // for (const auto &el : tmp) _vars.push_back(el);
                                    // tmp.clear();
                                // }
                                const auto ast_root = ast::generate_ast(erase_spaces(buf.substr
                                    (buf_fw.size(), buf.size() - 1 - buf_fw.size())));
                                // ast_root->tree();
                                buf_vars = ast_root->get_variables(_vars);
                                for (auto &list : _vars) {
                                    // tmp.insert(list);
                                }
                            }
                            if (fw_type == ELSE) {
                                buf_vars = ast::ast_node::once_not(else_data, _vars);
                                else_data.clear();
                            }
                            else if (fw_type == ELIF) {
                                auto buf_else_data = ast::ast_node::once_or(buf_vars, else_data);
                                buf_vars = ast::ast_node::once_and(buf_vars,
                                    ast::ast_node::once_not(else_data, _vars));
                                else_data = buf_else_data;
                            }
                            else {
                                else_data = buf_vars;
                            }
                        }
                        ast::ast_node::cmpPush(buf_vars, _vars);
                        auto x = parse(child,  buf_vars, depth + 1);
                        // for (auto &list : x) {
                            // tmp.insert(list);
                        // }
                        i += child->len;
                        break;
                    }
                    default: {
                        // if (!tmp.empty()) {
                            // _vars.clear();
                            // for (const auto &el : tmp) _vars.push_back(el);
                            // tmp.clear();
                        // }
                        else_data.clear();
                        if (cfg_print_only) break;
                        auto buf = code->operator[](i);
                        custom::str_type res(buf);
                        auto buffer_vars = translate(res, _vars);
                        if (!buffer_vars.empty()) {
                            for (auto &x : _vars) {
                                for (const auto &y : x) {
                                    y->rollback(_vars);
                                    // cache.get_tests_set().insert(from_any_to_str(y->data));
                                }
                            }
                            _vars = buffer_vars;
                        }
                        break;
                    }
                }
            }

            for (auto &x : _vars) {
                for (const auto &y : x) {
                    y->rollback(_vars);
                    // cache.get_tests_set().insert(from_any_to_str(y->data));
                }
            }
            return _vars;
        }

        void parse_bfs() const {
            ast::variables_t vars = {{}};
            std::vector<custom::custom_type> vars_orig;
            std::queue<std::pair<node_t *, ast::variables_t>> q;
            q.emplace(root, vars);

            while (!q.empty()) {
                auto [fst, snd] = q.front();
                q.pop();

                    ast::variables_t else_data;
                    for (auto i = fst->l; i < fst->l + fst->len; ++i) {
                    std::string first_word;
                    auto expr = std::stringstream(code->operator[](i));
                    expr >> first_word;

                    switch (auto fw_type = get_construction_type(first_word)) {
                        case IF:
                        case ELIF:
                        case ELSE:
                        // case FOR:
                        case WHILE: {
                            fst->children.push_back(new node_t(
                                    code->operator[](i).substr(get_spaces(code->operator[](i))),
                                    i + 1,
                                    get_code_block(i + 1)));
                            custom::str_type buf(code->operator[](i));
                            buf = erase_spaces(buf);
                            custom::str_type buf_fw(first_word); // "else: "
                            ast::variables_t buf_vars;
                            if (!cfg_print_only) {
                                if (fw_type != ELSE) {
                                    const auto ast_root = ast::generate_ast(erase_spaces(buf.substr
                                        (buf_fw.size(), buf.size() - 1 - buf_fw.size())));
                                    // ast_root->tree();
                                    buf_vars = ast_root->get_variables(snd);
                                }
                                if (fw_type == ELSE) {
                                    buf_vars = ast::ast_node::once_not(else_data, snd);
                                    else_data.clear();
                                }
                                else if (fw_type == ELIF) {
                                    auto buf_else_data = ast::ast_node::once_or(buf_vars, else_data);
                                    buf_vars = ast::ast_node::once_and(buf_vars,
                                        ast::ast_node::once_not(else_data, snd));
                                    else_data = buf_else_data;
                                }
                                else {
                                    else_data = buf_vars;
                                }
                            }
                            ast::ast_node::cmpPush(buf_vars, snd);
                            q.emplace(fst->children.back(), buf_vars);
                            i += get_code_block(i + 1);
                            break;
                        }
                        default:
                            else_data.clear();
                            if (cfg_print_only) break;
                            auto buf = code->operator[](i);
                            custom::str_type res(buf);
                            auto buffer_vars = translate(res, snd);
                            if (!buffer_vars.empty())
                                snd = buffer_vars;
                            break;
                    }
                }
                for (auto &x : snd) {
                    for (const auto &y : x) {
                        cache.get_tests_set().insert(from_any_to_str(y->data));
                    }
                }
            }
        }
#undef sub
    private:
        enum search {
            IF, ELIF, ELSE, FOR, WHILE, ANOTHER
        };

        [[nodiscard]] static std::string tree(node_t *_root, const std::string &move) {
            std::stringstream out;
            out << move << _root->name + '\n';
            auto move_new = move;
            if (!move_new.empty()) {
                for (auto i = move_new.size(); i --> move_new.size() - 4; )
                    move_new[i] = '.';
            }
            move_new += "|---";
            for (const auto &vertex : _root->children) {
                out << tree(vertex, move_new);
            }
            return out.str();
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

inline void custom::custom_type::rollback(const ast::variables_t &v) const {
    const std::string _s = name + " = " + rollback_in();
    const str_type s(_s);
    auto _vars = parse::parser::translate(s, v);
    for (auto &x : _vars) {
        for (const auto &y : x) {
            parse::cache.get_tests_set().insert(parse::from_any_to_str(y->data));
        }
    }
}

#undef init_cession
#undef init_struct_cession
#undef enable_all_types
#undef enable_int
#undef enable_float
#undef enable_string
#undef close_cession

#endif //CFG_H


/*

a = b + 3

 */