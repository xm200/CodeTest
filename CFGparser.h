#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
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
#include <variant>
#include <functional>
#include <iostream>

namespace custom {
    template<typename S>
    struct vec_line {
        const S *vec = nullptr;
        std::size_t l = 0, len = 0;

        vec_line() = default;

        [[maybe_unused]] [[nodiscard]] S extract() const {
            return vec->substr(l, len);
        }

        [[maybe_unused]] vec_line(vec_line<S> &data) : vec(data.vec), l(data.l), len(data.len) {}

        [[maybe_unused]] vec_line(const S *vec_, std::size_t l_, std::size_t len_) : vec(vec_), l(l_), len(len_) {}

        [[maybe_unused]] explicit vec_line(S data) : vec(&data) {
            len = data.size();
        }

        [[maybe_unused]] [[nodiscard]] std::size_t size() const _GLIBCXX_NOEXCEPT { return len; }

        [[maybe_unused]] [[nodiscard]] typename S::iterator begin() _GLIBCXX_NOEXCEPT
        { return vec->begin() + l; }
        [[maybe_unused]] [[nodiscard]] typename S::const_iterator begin() const _GLIBCXX_NOEXCEPT
        {return vec->cbegin() + l; }

        [[maybe_unused]] [[nodiscard]] typename S::iterator end() _GLIBCXX_NOEXCEPT
        { return vec->begin() + l + len; }
        [[maybe_unused]] [[nodiscard]] typename S::const_iterator end() const _GLIBCXX_NOEXCEPT
        {return vec->cbegin() + l + len; }

        [[maybe_unused]] [[nodiscard]] auto &front() _GLIBCXX_NOEXCEPT {
            return vec->operator[](l);
        }
        [[maybe_unused]] [[nodiscard]] auto &front() const _GLIBCXX_NOEXCEPT {
            return vec->operator[](l);
        }

        [[maybe_unused]] [[nodiscard]] auto &back() _GLIBCXX_NOEXCEPT {
            return vec->operator[](l + len - 1);
        }
        [[maybe_unused]] [[nodiscard]] auto &back() const _GLIBCXX_NOEXCEPT {
            return vec->operator[](l + len - 1);
        }

        [[maybe_unused]] [[nodiscard]] vec_line<S> substr(const std::size_t pos, const std::size_t sz)
        const _GLIBCXX_NOEXCEPT {
            return {vec, pos + l, std::min(sz, len - pos - l)};
        }

        [[maybe_unused]] [[nodiscard]] auto& operator[](std::size_t i) _GLIBCXX_NOEXCEPT {
#if defined(DEBUG_MODE)
            if (i >= len) throw std::out_of_range("index out of range");
#endif
            return vec->operator[](i + l);
        }
        [[nodiscard]] auto& operator[](std::size_t i) const _GLIBCXX_NOEXCEPT {
#if defined(DEBUG_MODE)
            if (i >= len) throw std::out_of_range("index out of range");
#endif
            return vec->operator[](i + l);
        }
    };

    template<typename S>
    [[maybe_unused]] vec_line<S> vl_substr(const S &vec, const std::size_t pos, const std::size_t len) _GLIBCXX_NOEXCEPT {
        return {&vec, pos, std::min(len, vec.size() - pos)};
    }
    template<typename S>
    [[maybe_unused]] vec_line<S> vl_substr(const vec_line<S> &vec, const std::size_t pos, const std::size_t len) _GLIBCXX_NOEXCEPT {
        return {vec.vec, pos + vec.l, std::min(len, vec.size() - pos - vec.l)};
    }

    std::vector<std::string> operations = {"and", "or", "==", "!=", ">", "<", ">=", "<=", "+", "-", "//", "%"};

    using str_type = custom::vec_line<std::string>;
    struct node {
        node *parent{};
        node *children[26] = {};
        node *parent_am{};
        char name{};
//            node *am[26] = {};
        std::vector<std::pair<std::string, int>> endings{};

        inline void check(char where) {
            if (!children[where - 'a']) children[where - 'a'] = new node(where);
        }

        void add_key(const std::string &s, int idx, int cur = 0) {
            if (cur == s.size()) {
                endings.emplace_back(s, idx);
                return;
            }
            check(s[cur]);
            add_key(s, cur + 1);
        }

        void init() {
            std::queue<node *> q;
            q.push(this);
            parent = this;
            parent_am = this;

            while (!q.empty()) {
                auto v = q.front();
                q.pop();

                auto go = v->parent_am;
                if (go == this) {
                    v->parent_am = this;
                }
                else {
                    while (!go->children[name - 'a'] && go != go->parent) go = go->parent_am;
                    v->parent_am = go->children[name - 'a'];
                }
                for (auto &x : v->parent_am->endings) v->endings.emplace_back(x);
                for (auto &x : v->children) q.push(x);
            }
        }

        node *detect(char c) {
            auto go = this;
            while (!go->children[c - 'a'] && go != go->parent) go = go->parent_am;
            return (go->children[c - 'a'] == nullptr ? this:go->children[c - 'a']);
        }

        [[nodiscard]] std::pair<std::pair<size_t , size_t>, size_t> go(const str_type &other) {
            auto _root  = this;
            std::pair<size_t, size_t> met[12];
            for (auto & i : met) i = {-1, -1};
            int brackets = 0;
            if (other.front()== '(' && other.back() == ')') return go(other.substr(1, other.size() - 2));
            auto lamda = [](const char c) { return 'a' <= c && c <= 'z'; };

            for (size_t idx = 0; idx < other.size(); ++idx) {
                _root = _root->detect(other[idx]);

                for (auto & ending : _root->endings) {
                    if (other[idx] == '(') ++brackets;
                    if (other[idx] == ')') --brackets;

                    auto buf = ending.second - ending.first.size();

                    if (brackets > 0) continue;
                    if ((buf > 0 && lamda(other[buf])) || (idx + 1 < other.size() && lamda(other[idx + 1]))) continue;
                    met[ending.second].first = buf + 1;
                    met[ending.second].second = idx + 1;
                }
            }
            for (auto x = 0; x < 12; ++x) if (met[x].first != -1 && met[x].second != -1) return {met[x], x};
            return {{-1, -1}, static_cast<size_t>(-1)};
        }

        explicit node(char n) : name(n) {};
        node() = default;
        ~node() = default;
    } root;


    [[nodiscard]] str_type erase_spaces(const str_type &other) {
        size_t l = 0, r = other.size();
        while (other[l] == ' ' && l < other.size()) ++l;
        if (l == other.size()) throw std::logic_error("empty string if function erase_spaces");
        while (other[r - 1] == ' ' && r > 0) --r;
        return other.substr(l, other.size() - r);
    }

    struct custom_type {
        using inner_type = std::variant<
                interval::interval<typeInt>,
                interval::interval<typeFloat>,
                interval::interval<std::string>,
                std::vector<custom_type*>>;


        enum types {
            INT, FLOAT, STRING, VECTOR, UNKNOWN
        };

        short type = UNKNOWN;
        inner_type data;
        std::string name = "undefined name";

        custom_type() = default;
        explicit custom_type(const inner_type& d) : data(d), type(get_type(d)) {}

        ~custom_type() = default;

#define subst(fun, a) \
    switch (type) { \
        case INT: return fun<typeInt>(a); \
        case FLOAT: return fun<typeFloat>(a); \
        case STRING: return fun<std::string>(a); \
        default: throw std::runtime_error("unknown type"); \
    }

#define subst_digit_only(fun, a)  \
    switch (type) { \
        case INT: return fun<typeInt>(a); \
        case FLOAT: return fun<typeFloat>(a); \
        default: throw std::runtime_error("unknown type"); \
    }

        [[nodiscard]] inner_type operator<(const custom_type &a) const {subst(less, a)}
        [[nodiscard]] inner_type operator>(const custom_type &a) const {subst(more, a)}
        [[nodiscard]] inner_type operator<=(const custom_type &a) const {subst(less_equal, a)}
        [[nodiscard]] inner_type operator>=(const custom_type &a) const {subst(more_equal, a)}
        [[nodiscard]] inner_type operator==(const custom_type &a) const {subst(equal, a)}
        [[nodiscard]] inner_type operator!=(const custom_type &a) const {subst(not_equal, a)}
        [[nodiscard]] inner_type operator+(const custom_type &a) const { subst_digit_only(add, a) }
        [[nodiscard]] inner_type operator-(const custom_type &a) const { subst_digit_only(subtract, a) }
        [[nodiscard]] inner_type operator*(const custom_type &a) const { subst_digit_only(multiply, a) }
        [[nodiscard]] inner_type operator/(const custom_type &a) const { subst_digit_only(divide, a) }
        [[nodiscard]] inner_type operator%(const custom_type &a) const { return mod(a); }

#undef subst_digit_only
#undef subst

        custom_type& operator=(const inner_type &a) {
            data = a;
            type = get_type(a);
            return *this;
        }

        custom_type& operator=(const custom_type &a) = default;

        void operator+=(const custom_type &a) { checkType(a, "+="); data = this->operator+(a); }
        void operator-=(const custom_type &a) { checkType(a, "-="); data = this->operator-(a); }
        void operator*=(const custom_type &a) { checkType(a, "*="); data = this->operator*(a); }
        void operator/=(const custom_type &a) { checkType(a, "/="); data = this->operator/(a); }
        void operator%=(const custom_type &a) { checkType(a, "%="); data = this->operator%(a); }

        [[nodiscard]] inner_type extract(const str_type &other) {
            for(int i = 0; i < operations.size(); ++i) root.add_key(operations[i], i);
            root.init();
            return _extract(other);
        }


        [[nodiscard]] static short get_type(const std::string &s) {
            if ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"')) return STRING;
            bool has_dot = false;
            if (s.size() == 1 && s.front() == '.') return UNKNOWN;
            for (auto chr: s) {
                if (chr == '.') {
                    if (has_dot) throw std::runtime_error("Function get_type() : double dot in integer-like type!");
                    else has_dot = true;
                }
                else if (chr > '9' || chr < '0') return UNKNOWN;
            }
            return (has_dot) ? FLOAT : INT;
        }

        [[nodiscard]] static inline inner_type mega_cast(const str_type &s_buf) {
            auto s = s_buf.extract();
            switch (get_type(s)) {
                case UNKNOWN: throw std::runtime_error("Function mega_cast() : got unknown type!"); /// todo: add variable
                case INT: {
                    interval::interval<typeInt> buf;
                    buf.add_point(std::stoll(s));
                    return buf;
                }
                case FLOAT: {
                    interval::interval<typeFloat> buf;
                    buf.add_point(std::stod(s));
                    return buf;
                }
                default: {
                    interval::interval<std::string> buf;
                    buf.add_point(s);
                    return buf;
                }
            }
        }

        [[nodiscard]] inner_type _extract(const str_type &other) { // if (a == 2 || b == 3) && c + 2 == 5
            auto split_idx = root.go(other);
            if (split_idx.second == -1) return mega_cast(other);
            auto l1 = split_idx.first.second;
            auto r1 = split_idx.first.second;
            auto ignor = _extract(other.substr(0, l1));
            auto ignor2 = _extract(other.substr(l1, r1));
        }

    protected:

        template<typename T>
        [[nodiscard]] inner_type less_in(
                     const std::function<void(interval::interval<T> &it, const custom_type &a)> &it,
                     const custom_type &a,
                     const std::string &n) const {
            checkType(a, n);
            interval::interval<T> buf;
            it(buf, a);
            return buf * std::get<interval::interval<T>>(data);
        }

        void checkType(const custom_type &a, const std::string &name_op) const {
            if (this->type != a.type)
                throw std::logic_error("Function operator " + name_op + ", error: could not compare different types!");
        }

        template<typename type>
        [[nodiscard]] static inline bool can_cast(const inner_type &d) {
            return std::get_if<type>(&d) != nullptr;
        }

        [[nodiscard]] static short get_type(const inner_type &d) {
            if (can_cast<interval::interval<typeInt>>(d)) return types::INT;
            if (can_cast<interval::interval<typeFloat>>(d)) return types::FLOAT;
            if (can_cast<interval::interval<std::string>>(d)) return types::STRING;
            if (can_cast<std::vector<custom_type*>>(d)) return types::VECTOR;
            throw std::runtime_error("Function: can_cast(), error: UNKNOWN TYPE");
        }

        template<typename T>
        [[nodiscard]] inner_type less(const custom_type &a) const {
            return less_in<T>([](interval::interval<T> &buf, const custom_type &a) {
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a.data).any().value());
            }, a, "<");
        }

        template<typename T>
        [[nodiscard]] inner_type more(const custom_type &a) const {
            return less_in<T>([](interval::interval<T> &buf, const custom_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a.data).any().value(),interval::maximal<T>());
            }, a, ">");
        }

        template<typename T>
        [[nodiscard]] inner_type less_equal(const custom_type &a) const {
            return less_in<T>([](interval::interval<T> &buf, const custom_type &a) {
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a.data).any().value());
                buf.add_point(std::get<interval::interval<T>>(a.data).any().value());
            }, a, "<=");
        }

        template<typename T>
        [[nodiscard]] inner_type more_equal(const custom_type &a) const {
            return less_in<T>([](interval::interval<T> &buf, const custom_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a.data).any().value(), interval::maximal<T>());
                buf.add_point(std::get<interval::interval<T>>(a.data).any().value());
            }, a, ">=");
        }

        template<typename T>
        [[nodiscard]] inner_type equal(const custom_type &a) const {
            return less_in<T>([](interval::interval<T> &buf, const custom_type &a) {
                buf.add_point(std::get<interval::interval<T>>(a.data).any().value());
            }, a, "==");
        }

        template<typename T>
        [[nodiscard]] inner_type not_equal(const custom_type &a) const {
            return less_in<T>([](interval::interval<T> &buf, const custom_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a.data).any().value(), interval::maximal<T>());
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a.data).any().value());
            }, a, "!=");
        }

        template<typename T>
        [[nodiscard]] inner_type add(const custom_type &a) const {
            checkType(a, "+");
            return std::get<interval::interval<T>>(data)
                   + std::get<interval::interval<T>>(a.data).any().value();
        }

        template<typename type>
        [[nodiscard]] inner_type subtract(const custom_type &a) const {
            checkType(a, "-");
            return std::get<interval::interval<type>>(data)
                   - std::get<interval::interval<type>>(a.data).any().value();
        }

        template<typename type>
        [[nodiscard]] inner_type multiply(const custom_type &a) const {
            checkType(a, "*");
            return std::get<interval::interval<type>>(data)
                   * std::get<interval::interval<type>>(a.data).any().value();
        }

        template<typename type>
        [[nodiscard]] inner_type divide(const custom_type &a) const {
            checkType(a, "/");
            return std::get<interval::interval<type>>(data)
                   / std::get<interval::interval<type>>(a.data).any().value();
        }

        [[nodiscard]] inner_type mod(const custom_type &a) const {
            checkType(a, "%");
            return std::get<interval::interval<typeInt>>(data)
                   % std::get<interval::interval<typeInt>>(a.data).any().value();
        }
    };
}



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

        static void parse_expr(const std::string &buf, node_t &node) { /// todo: think about +=, -=, ..., not only assign
            auto ss = std::stringstream(buf);
            std::vector<std::string> part1, part2;
            std::string word;

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
//                            parse_expr(code->operator[](i), *_root);
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

#pragma clang diagnostic pop