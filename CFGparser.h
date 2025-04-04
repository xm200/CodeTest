#ifndef CFG_H
#define CFG_H


#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include "ASTExperiment.h"


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
                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            else if constexpr (get_os() == LINUX || get_os() == MACOS) path = way + '/';
                // ReSharper disable once CppDFAUnreachableCode
            else path = way + '\\';

                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            if constexpr (get_os() == LINUX || get_os() == MACOS) path += "cache/";
                // ReSharper disable once CppDFAUnreachableCode
            else path += "cache\\";
            system(("mkdir " + path).c_str());
            inited = true;
        }

        ~cache_t() {
            if (!inited) return;
#if !defined(DO_NOT_REMOVE_CACHE)
                // ReSharper disable once CppDFAUnreachableCode
                // ReSharper disable once CppRedundantBooleanExpressionArgument
            if constexpr (get_os() == LINUX || get_os() == MACOS) system(("rm -rf " + path).c_str());
                // ReSharper disable once CppDFAUnreachableCode
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
            auto ans = orig;
            std::vector<char> boo = {'>', '<', '=', '!'};
            for (int i = 0; i < s.size(); ++i) {
                switch (s[i]) {
                    case '=': {
                        auto _s = custom::erase_spaces(s.substr(i + 1));
                        if (_s.substr(0, 4).extract() == "int(" || _s.substr(0, 4).extract() == "int ") {
                            auto *x = new custom::custom_type;
                            x->name = custom::erase_spaces(_s.substr(0, i - 1)).extract();
                            interval::interval<typeInt> buf;
                            buf.add_interval(interval::minimal<typeInt>(), interval::maximal<typeInt>());
                            x->data = buf;
                            x->reset_type();
                            for (auto &j : ans) {
                                if (j.front().name == x->name) { j.front() = *x; return ans; }
                            }
                            ans.push_back({*x});
                            return ans;
                        }
                        if (_s.size() >= 6 && (_s.substr(i, 6).extract() == "float(" || _s.substr(i, 6).extract() == "float ")) {
                            auto *x = new custom::custom_type;
                            x->name = custom::erase_spaces(_s.substr(0, i - 1)).extract();
                            interval::interval<typeFloat> buf;
                            buf.add_interval(interval::minimal<typeFloat>(), interval::maximal<typeFloat>());
                            x->data = buf;
                            x->reset_type();
                            for (auto &j : ans) {
                                if (j.front().name == x->name) { j.front() = *x; return ans; }
                            }
                            ans.push_back({*x});
                            return ans;
                        }
                        if (_s.size() >= 6 && (_s.substr(i, 6).extract() == "input(" || _s.substr(i, 6).extract() == "input ")) {
                            auto *x = new custom::custom_type;
                            x->name = custom::erase_spaces(_s.substr(0, i - 1)).extract();
                            interval::interval<std::string> buf;
                            buf.add_interval(interval::minimal<std::string>(), interval::maximal<std::string>());
                            x->data = buf;
                            x->reset_type();
                            for (auto &j : ans) {
                                if (j.front().name == x->name) { j.front() = *x; return ans; }
                            }
                            ans.push_back({*x});
                            return ans;
                        }
                        if ((i + 1 < s.size() &&
                                std::find(boo.begin(), boo.end(), s.extract()[i + 1]) != boo.end()) ||
                            (i != 0 && std::find(boo.begin(), boo.end(), s.extract()[i - 1]) != boo.end())) break;
                        auto root = ast::generate_ast(s.substr(i + 1)); // a = 3, b = 4
                        auto buf = root->get_variables(orig);
                        auto res = buf.front().front();
                        auto *x = new custom::custom_type;
                        *x = res;
                        res.history = std::pair<std::size_t, custom::custom_type*>(0, x);
                        res.name = x->name;
                        res.data = x->data;
                        for (auto &j : ans) {
                            if (!j.empty() && !res.name.empty() && j.front().name == s.substr(0, i).extract()) { j.front() = res; goto end; }
                        }
                        ans.back().push_back(res);
                        end:
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
                            vars = translate(res, vars);
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

        template<typename T>
        void write_to_file(const custom::custom_type &what) {
            if (!what.data.has_value()) return;
            std::size_t ind = -1;
            for (auto i = cache().size(); i --> 0;) {
                if (cache()[i] == '/' || cache()[i] == '\\') {
                    ind = i;
                    break;
                }
            }

            std::ofstream out(cache().substr(0, ind));

            if (const auto buf = std::get_if<interval::interval<T>>(&what.data.value()); buf != nullptr)
                out << buf->any().value();

            out.close();
        }

        void parse(node_t *node, const ast::variables_t &vars, const size_t depth = 0) {
            ast::variables_t _vars = vars;
            bool is_end = false;
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
                        is_end = true;
                        break;
                    }
                    default: {
                        auto buf = code->operator[](i);
                        custom::str_type res(buf);
                        _vars = translate(res, _vars);
                        break;
                    }
                }
            }
            if (!is_end) {
                for (auto &x : _vars) {
                    for (auto &y : x) {
                        switch (get_interval_type(y)) {
                            case custom::custom_type::types::INT: {
                                write_to_file<typeInt>(y);
                                break;
                            }
                            case custom::custom_type::types::FLOAT: {
                                write_to_file<typeFloat>(y);
                                break;
                            }
                            case custom::custom_type::types::STRING: {
                                write_to_file<std::string>(y);
                                break;
                            }
                            default:
                                break;
                        }
                    }
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
#endif //CFG_H
