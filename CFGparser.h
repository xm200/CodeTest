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
#include <iostream>
#include <variant>
#include "interval.h"

namespace custom {
    template<typename T>
    struct vec_line {
        const std::vector<T> *vec = nullptr;
        std::size_t l = 0, len = 0;

        [[nodiscard]] std::size_t size() const { return len; }

        [[nodiscard]] typename std::vector<T>::iterator begin() _GLIBCXX_NOEXCEPT
        { return vec->begin() + l; }
        [[nodiscard]] typename std::vector<T>::const_iterator begin() const _GLIBCXX_NOEXCEPT
        {return vec->cbegin() + l; }

        [[nodiscard]] typename std::vector<T>::iterator end() _GLIBCXX_NOEXCEPT
        { return vec->begin() + l + len; }
        [[nodiscard]] typename std::vector<T>::const_iterator end() const _GLIBCXX_NOEXCEPT
        {return vec->cbegin() + l + len; }

        [[nodiscard]] T& operator[](std::size_t i) _GLIBCXX_NOEXCEPT {
            #if defined(DEBUG_MODE)
                if (i >= len) throw std::out_of_range("index out of range");
            #endif
            return vec->operator[](i) + l;
        }
        [[nodiscard]] T& operator[](std::size_t i) const _GLIBCXX_NOEXCEPT {
            #if defined(DEBUG_MODE)
                if (i >= len) throw std::out_of_range("index out of range");
            #endif
            return vec->operator[](i) + l;
        }
    };

    template<typename T>
    vec_line<T> vl_substr(const std::vector<T> &vec, const std::size_t pos, const std::size_t len) _GLIBCXX_NOEXCEPT {
        return {&vec, pos, std::min(len, vec.size() - pos)};
    }
    template<typename T>
    vec_line<T> vl_substr(const vec_line<T> &vec, const std::size_t pos, const std::size_t len) _GLIBCXX_NOEXCEPT {
        return {vec.vec, pos + vec.l, std::min(len, vec.size() - pos - vec.l)};
    }

    struct custom_type {
        using inner_type = std::variant<typeInt, typeFloat, char, std::vector<custom_type*>>;
        enum types {
            INT, FLOAT, CHAR, VECTOR
        };
        short type;
        inner_type data;
        std::string name = "undefined name";
        [[nodiscard]] inner_type operator<(const inner_type &a) {

            return {};
        }
    protected:
        template<typename type>
        inline bool can_cast() const {return std::get_if<type>(&data) != nullptr;}
        short get_type() const {
            if (can_cast<typeInt>()) return types::INT;
            if (can_cast<typeFloat>()) return types::FLOAT;
            if (can_cast<char>()) return types::CHAR;
            if (can_cast<std::vector<custom_type*>>()) return types::VECTOR;

        }
    };

}

namespace parse {

    inline std::string file_name, file_type;

    enum os {
        LINUX, MACOS, WIN
    };

    enum operators {
        LT, GT, LE, GE, EQ, NE, NOT_OPERATOR
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



    struct variables {
        std::map<std::string, std::any> v{};

    };



    struct node_t {
        std::string name;
        std::vector<node_t *> children{};
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
            #if defined(VERBOSE)
                std::cout << "using " << (gm ? "DFS" : "BFS") << " algo\n" << std::flush;
            #endif
        }

        void parse() {
            if (graph_mode) parse(root);
            else parse_bfs();
        }

        void tree() const {
            std::string s;
            tree(root, s);
            std::cout << std::flush;
        }

    protected:
        node_t *root;
        bool graph_mode;

        static short get_op(const std::string &buf) {
            if (buf == "<") return LT;
            if (buf == ">") return GT;
            if (buf == "<=") return LE;
            if (buf == ">=") return GE;
            if (buf == "==") return EQ;
            if (buf == "!=") return NE;
            return NOT_OPERATOR;
        }

        void parse_bfs() const {
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
                            break;
                    }
                }
            }
        }

        [[nodiscard]] static size_t get_spaces(const std::string &buf) {
            size_t spaces = 0;
            for (auto &s : buf) if (s == ' ') ++spaces; else break;
            return spaces;
        }

        [[nodiscard]] size_t get_code_block(const size_t l = 0) const {
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

        static void tree(const node_t *_root, const std::string &move) {
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
        static int get_construction_type(const std::string &buf) {
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
