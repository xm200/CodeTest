//
// Created by xm200 on 05.03.2025. =uwu=
//

#ifndef AST_H
#define AST_H
// ReSharper disable CppUseStructuredBinding
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <any>
#include <utility>
#include <string>
#include <vector>
#include <queue>
#include <variant>
#include <functional>

#include "ASTExperiment.h"
#include "ASTExperiment.h"


enum types {INT, FLOAT, STRING, UNKNOWN};
enum buffer_types {VAR, VEC_VAR};

using inner_type = std::variant<
        interval::interval<typeInt>,
        interval::interval<typeFloat>,
        interval::interval<std::string>>;

using variable = std::pair<std::string, inner_type>;

using variables = std::vector<variable>;

const std::vector<std::string> operations =
        {"and", "or", "==", "!=", ">", "<", ">=", "<=", "+", "-", "//", "%"};


namespace custom {
#define init_cession(a, ...) \
template<typename> \
__VA_ARGS__; \
__VA_ARGS__ { \
switch(get_type(a)) {
#define init_struct_cession(a, ...) \
__VA_ARGS__ { \
switch(get_type(a)) {
#define enable_int(name, ...) case INT: return name<typeInt>(__VA_ARGS__);
#define enable_float(name, ...) case FLOAT: return name<typeFloat>(__VA_ARGS__);
#define enable_string(name, ...) case STRING: return name<std::string>(__VA_ARGS__);
#define close_cession(T) \
default: throw std::runtime_error("unknown type");}} \
template<typename T>
#define enable_all_types(name, ...) \
enable_int(name, __VA_ARGS__) \
enable_float(name, __VA_ARGS__) \
enable_string(name, __VA_ARGS__)



    template<typename S>
    struct vec_line {
        const S *vec = nullptr;
        std::size_t l = 0, len = 0;

        vec_line() = default;

        [[maybe_unused]] [[nodiscard]] S extract() const {
            return vec->substr(l, len);
        }

        [[maybe_unused]] vec_line(const vec_line &data) : vec(data.vec), l(data.l), len(data.len) {}

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

        [[maybe_unused]] [[nodiscard]] vec_line substr(const std::size_t pos, const std::size_t sz = -1)
        const _GLIBCXX_NOEXCEPT {
            return {vec, pos + l, std::min(sz, len - pos - l)};
        }

        [[maybe_unused]] [[nodiscard]] auto& operator[](const std::size_t i) {
            #if defined(DEBUG_MODE)
                if (i >= len) throw std::out_of_range("index out of range");
            #endif
            return vec->operator[](i + l);
        }
        [[nodiscard]] auto& operator[](const std::size_t i) const {
            #if defined(DEBUG_MODE)
                if (i >= len) throw std::out_of_range("index out of range");
            #endif
            return vec->operator[](i + l);
        }
    };



    template<typename S>
    [[maybe_unused]] vec_line<S> vl_substr(const S &vec, const std::size_t pos, const std::size_t len = -1) _GLIBCXX_NOEXCEPT {
        return {&vec, pos, std::min(len, vec.size() - pos)};
    }
    template<typename S>
    [[maybe_unused]] vec_line<S> vl_substr(const vec_line<S> &vec, const std::size_t pos, const std::size_t len = -1) _GLIBCXX_NOEXCEPT {
        return {vec.vec, pos + vec.l, std::min(len, vec.size() - pos - vec.l)};
    }


    using str_type = vec_line<std::string>;

    struct axo_node {
        axo_node *parent{};
        axo_node *children[26] = {};
        axo_node *parent_am{};
        char name{};
        std::vector<std::pair<std::string, std::size_t>> endings{};

        void check(const char where) {
            if (!children[where - 'a']) children[where - 'a'] = new axo_node(where);
        }

        void add_key(const std::string &s, std::size_t idx, const std::size_t cur = 0) {
            if (cur == s.size()) {
                endings.emplace_back(s, idx);
                return;
            }
            check(s[cur]);
            add_key(s, cur + 1);
        }

        void init() {
            std::queue<axo_node *> q;
            q.push(this);
            parent = this;
            parent_am = this;

            while (!q.empty()) {
                const auto v = q.front();
                q.pop();

                if (auto go = v->parent_am; go == this) {
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

        axo_node *jump(const char c) {
            auto go = this;
            while (!go->children[c - 'a'] && go != go->parent) go = go->parent_am;
            return go->children[c - 'a'] == nullptr ? this:go->children[c - 'a'];
        }

        [[nodiscard]] std::pair<std::pair<size_t , size_t>, size_t> go(const str_type &other) {
            auto _root  = this;
            std::pair<size_t, size_t> met[12];
            for (auto & i : met) i = {-1, -1};
            int brackets = 0;
            if (other.front()== '(' && other.back() == ')') return go(other.substr(1, other.size() - 2)); // a == 3) && (b == 4
            auto fun = [](const char c) { return 'a' <= c && c <= 'z'; };

            for (size_t idx = 0; idx < other.size(); ++idx) {
                _root = _root->jump(other[idx]);

                for (const auto &ending : _root->endings) {
                    if (other[idx] == '(') ++brackets;
                    if (other[idx] == ')') --brackets;

                    const auto buf = ending.second - ending.first.size();

                    if (brackets > 0) continue;
                    if ((buf > 0 && fun(other[buf])) || (idx + 1 < other.size() && fun(other[idx + 1]))) continue;
                    met[ending.second].first = buf + 1;
                    met[ending.second].second = idx + 1;
                }
            }
            for (auto x = 0; x < 12; ++x) if (met[x].first != -1 && met[x].second != -1) return {met[x], x};
            return {{-1, -1}, static_cast<size_t>(-1)};
        }

        explicit axo_node(const char n) : name(n) {}
        axo_node() = default;
        ~axo_node() = default;
    } inline root;



    [[nodiscard]] str_type inline erase_spaces(const str_type &other) {
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

        using variables = std::vector<std::pair<str_type, inner_type>>;

        enum types {
            INT, FLOAT, STRING, VECTOR, UNKNOWN
        };

        short type = UNKNOWN;
        inner_type data;
        str_type name{};

        custom_type() = default;
        explicit custom_type(const inner_type& d) : type(get_type(d)), data(d) {}

        ~custom_type() = default;

#define subst(fun, a, b) \
    switch (get_type(a)) { \
        case INT: return fun<typeInt>(a, b); \
        case FLOAT: return fun<typeFloat>(a, b); \
        case STRING: return fun<std::string>(a, b); \
        default: throw std::runtime_error("unknown type"); \
    }

#define subst_digit_only(fun, a, b)  \
    switch (get_type(a)) { \
        case INT: return fun<typeInt>(a, b); \
        case FLOAT: return fun<typeFloat>(a, b); \
        default: throw std::runtime_error("unknown type"); \
    }

        [[nodiscard]] inner_type friend operator<(const inner_type &a, const inner_type &b) { subst(less, a, b) }
        [[nodiscard]] inner_type friend operator>(const inner_type &a, const inner_type &b) { subst(more, a, b) }
        [[nodiscard]] inner_type friend operator<=(const inner_type &a, const inner_type &b) { subst(less_equal, a, b) }
        [[nodiscard]] inner_type friend operator>=(const inner_type &a, const inner_type &b) { subst(more_equal, a, b) }
        [[nodiscard]] inner_type friend operator==(const inner_type &a, const inner_type &b) { subst(equal, a, b) }
        [[nodiscard]] inner_type friend operator!=(const inner_type &a, const inner_type &b) { subst(not_equal, a, b) }
        [[nodiscard]] inner_type friend operator+(const inner_type &a, const inner_type &b) { subst_digit_only(add, a, b) }
        [[nodiscard]] inner_type friend operator-(const inner_type &a, const inner_type &b) { subst_digit_only(subtract, a, b) }
        [[nodiscard]] inner_type friend operator*(const inner_type &a, const inner_type &b) { subst_digit_only(multiply, a, b) }
        [[nodiscard]] inner_type friend operator/(const inner_type &a, const inner_type &b) { subst_digit_only(divide, a, b) }
        [[nodiscard]] inner_type friend operator%(const inner_type &a, const inner_type &b) { return mod(a, b); }

#undef subst_digit_only
#undef subst

        custom_type& operator=(const inner_type &a) {
            data = a;
            type = get_type(a);
            return *this;
        }

        custom_type& operator=(const custom_type &a) = default;

        friend inner_type & operator+=(inner_type &a, const inner_type &b) { checkType(a, b, "+="); a = (a + b); return a; }
        friend inner_type & operator-=(inner_type &a, const inner_type &b) { checkType(a, b, "-="); a = (a - b); return a; }
        friend inner_type & operator*=(inner_type &a, const inner_type &b) { checkType(a, b, "*="); a = (a * b); return a; }
        friend inner_type & operator/=(inner_type &a, const inner_type &b) { checkType(a, b, "/="); a = (a / b); return a; }
        friend inner_type & operator%=(inner_type &a, const inner_type &b) { checkType(a, b, "%="); a = (a % b); return a; }

        [[nodiscard]] inner_type static extract(const str_type &other) {
            for(int i = 0; i < operations.size(); ++i) root.add_key(operations[i], i);
            root.init();
            throw std::runtime_error("function did not realised");
//            return _extract(other); ///todo: make wrapper on extractor
        }


        [[nodiscard]] static int search_name(const variables &v, const str_type &s) {
            for (int i = 0; i < v.size(); ++i)
                if (v[i].first.extract() == s.extract()) return i;
            return -1;
        }

        // [[nodiscard]] static std::pair<str_type, inner_type> dereference_cast(const str_type &s, const variables &v) {
        //     switch (const auto _s = erase_spaces(s).extract(); get_type(_s)) {
        //         case UNKNOWN: {
        //             return v[search_name(v, s)];
        //         }
        //         case INT: {
        //             interval::interval<typeInt> buf;
        //             buf.add_point(std::stoll(_s));
        //             str_type data;
        //             return {data, buf};
        //         }
        //         case FLOAT: {
        //             interval::interval<typeFloat> buf;
        //             buf.add_point(std::stod(_s));
        //             str_type data;
        //             return {data, buf};
        //         }
        //         default: {
        //             interval::interval<std::string> buf;
        //             buf.add_point(_s);
        //             str_type data;
        //             return {data, buf};
        //         }
        //     }
        // }

        [[nodiscard]] static variables merge(const variables &a, const variables &b, const variables &v, const str_type &oper) {
            std::size_t id = -1;
            for (auto i = 0; i < operations.size(); ++i) if (operations[i] == oper.extract()) {id = i; break;}
            if (id == -1) throw std::runtime_error("push() called with invalid id!");

            auto res = v;
            ///todo: debug it tomorrow, because it would not work now

            return res;
        }

        [[nodiscard]] static std::pair<str_type, inner_type> merge(
                const inner_type &a, const inner_type &b, const std::size_t op_number, const str_type &name) {
            switch (op_number) {
                case 0: return {name, a * b}; // a == 3 and b == 4 -> [["a", 3], ["b", 4]]
                case 2: return {name, a == b};
                case 3: return {name, a != b};
                case 4: return {name, a > b};
                case 5: return {name, a < b};
                case 6: return {name, a >= b};
                case 7: return {name, a <= b};
                case 8: return {name, a + b};
                case 9: return {name, a - b};
                case 10: return {name, a / b};
                case 11: return {name, a % b};
                default:
                    throw std::logic_error("Function push() : Unresolved data"); // there could not be VARIABLE. Only intervals. The first arg must be resolved as interval before calling
            }
        }

        [[nodiscard]] variables static _extract(const str_type &other, variables &v) { // if (a == 2 || b == 3) && c + 2 == 5
            const auto split_idx = root.go(other);
            if (split_idx.second == -1) { // a == | 2
                return {};
            } if (split_idx.second == 1) {
                ///todo: add or processing
            }
            const auto l1 = split_idx.first.first;
            const auto r1 = split_idx.first.second;
            const auto p1 = _extract(other.substr(0, l1), v);
            const auto p2 = _extract(other.substr(r1), v);

            const auto buf = other.substr(l1, r1 - l1 + 1);
            // return merge(p1, p2, v, buf); - a < 2 // a < 2 == b > 3
            // return merge(p1, p2, v, buf); - [(a < 2) && (b > 3) && (c != 0)]
            return {};
        }

    protected:

        template<typename T>
        [[nodiscard]] static inner_type less_in(
                const std::function<void(interval::interval<T> &it, const inner_type &a)> &it,
                const inner_type &a,
                const inner_type &b,
                const std::string &n) {
            checkType(a, b, n);
            interval::interval<T> buf;
            it(buf, a);
            return buf * std::get<interval::interval<T>>(b);
        }

        void checkType(const custom_type &a, const std::string &name_op) const {
            if (this->type != a.type)
                throw std::logic_error("Function operator " + name_op + ", error: could not compare different types!");
        }

        static void checkType(const inner_type &a, const inner_type &b, const std::string &name_op) {
            if (get_type(b) != get_type(a))
                throw std::logic_error("Function operator " + name_op + ", error: could not compare different types!");
        }

        template<typename type>
        [[nodiscard]] static inline bool can_cast(const inner_type &d) {
            return std::get_if<type>(&d) != nullptr;
        }

        [[nodiscard]] static short get_type(const inner_type &d) {
            if (can_cast<interval::interval<typeInt>>(d)) return INT;
            if (can_cast<interval::interval<typeFloat>>(d)) return FLOAT;
            if (can_cast<interval::interval<std::string>>(d)) return STRING;
            if (can_cast<std::vector<custom_type*>>(d)) return VECTOR;
            throw std::runtime_error("Function: can_cast(), error: UNKNOWN TYPE");
        }

        template<typename T>
        [[nodiscard]] static inner_type less(const inner_type &a, const inner_type &b) {
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a).any().value());
            }, a, b, "<");
        }

        template<typename T>
        [[nodiscard]] static inner_type more(const inner_type &a, const inner_type &b) {
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a).any().value(),interval::maximal<T>());
            }, a, b, ">");
        }

        template<typename T>
        [[nodiscard]] static inner_type less_equal(const inner_type &a, const inner_type &b) {
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a).any().value());
                buf.add_point(std::get<interval::interval<T>>(a).any().value());
            }, a, b, "<=");
        }

        template<typename T>
        [[nodiscard]] static inner_type more_equal(const inner_type &a, const inner_type &b) {
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a).any().value(), interval::maximal<T>());
                buf.add_point(std::get<interval::interval<T>>(a).any().value());
            }, a, b, ">=");
        }

        template<typename T>
        [[nodiscard]] static inner_type equal(const inner_type &a, const inner_type &b) {
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_point(std::get<interval::interval<T>>(a).any().value());
            }, a, b, "==");
        }

        template<typename T>
        [[nodiscard]] static inner_type not_equal(const inner_type &a, const inner_type &b) {
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a).any().value(), interval::maximal<T>());
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a).any().value());
            }, a, b, "!=");
        }

        template<typename T>
        [[nodiscard]] static inner_type add(const inner_type &a, const inner_type &b) {
            checkType(a, b, "+");
            return std::get<interval::interval<T>>(a)
                   + std::get<interval::interval<T>>(b).any().value();
        }

        template<typename type>
        [[nodiscard]] static inner_type subtract(const inner_type &a, const inner_type &b) {
            checkType(a, b, "-");
            return std::get<interval::interval<type>>(a)
                   - std::get<interval::interval<type>>(b).any().value();
        }

        template<typename type>
        [[nodiscard]] static inner_type multiply(const inner_type &a, const inner_type &b) {
            checkType(a, b, "*");
            return std::get<interval::interval<type>>(a)
                   * std::get<interval::interval<type>>(b).any().value();
        }

        template<typename type>
        [[nodiscard]] static  inner_type divide(const inner_type &a, const inner_type &b) {
            checkType(a, b, "/");
            return std::get<interval::interval<type>>(a)
                   / std::get<interval::interval<type>>(b).any().value();
        }

        [[nodiscard]] static inner_type mod(const inner_type &a, const inner_type &b) {
            checkType(a, b, "%");
            return std::get<interval::interval<typeInt>>(a)
                   % std::get<interval::interval<typeInt>>(b).any().value();
        }
    };


#undef init_cession
#undef init_struct_cession
#undef enable_all_types
#undef enable_int
#undef enable_float
#undef enable_string
#undef close_cession
}



struct ast_node {
    variables value;

    explicit ast_node(const std::string &what) { build(what); }

    ast_node() = default;
    ~ast_node() = default;

    void build(const std::string &str) {
        int opened = 0;

        if (str.front() == '(' && str.back() == ')') {
            ++opened;
            for (int i = 1; i < str.size() - 1; ++i) {
                if (str[i] == '(') ++opened;
                if (str[i] == ')') --opened;
                if (!opened && i < str.size()) goto filtered;
            }
            build(str.substr(1, str.size() - 2));
            return;
        }

        filtered:
        what = str;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '(') ++opened;
            if (str[i] == ')') --opened;
            if (opened) continue;
            for (auto &x : operations) {
                if (str.substr(i, x.size()) != x) continue;
                left = new ast_node(erase_spaces(str.substr(0, i)));
                oper = str.substr(i, x.size());
                right = new ast_node(erase_spaces(str.substr(i + x.size() + 1)));
            }
        }
    }

protected:
    ast_node *left = nullptr, *right = nullptr;
    std::string oper, what;
    std::variant<variable, variables> buffer;

    [[nodiscard]] static std::string erase_spaces(const std::string &s) {
        size_t l = 0, r = s.size() - 1;
        while (s[l] == ' ') ++l;
        while (s[r] == ' ') --r;
        return s.substr(l, r - l + 1);
    }

    [[nodiscard]] static short get_type(const std::string &s) {
        if ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"')) return STRING;
        bool has_dot = false;
        if (s.size() == 1 && s.front() == '.') return UNKNOWN;
        for (const auto chr: s) {
            if (chr == '.') {
                if (has_dot) throw std::runtime_error("Function get_type() : double dot in integer-like type!");
                has_dot = true;
            }
            else if (chr > '9' || chr < '0') return UNKNOWN;
        }
        return (has_dot) ? FLOAT : INT;
    }

    [[nodiscard]] static int search_name(const variables &v, const std::string &s) {
        for (int i = 0; i < v.size(); ++i)
            if (v[i].first == s) return i;
        return -1;
    }

    [[nodiscard]] static std::size_t get_op_id(const std::string &op) {
        for (auto i = 0; i < operations.size(); ++i)
            if (operations[i] == op) return i;
        return -1;
    }

    [[nodiscard]] static short get_buf_type(const std::variant<variable, variables> &v) {
        if (std::get_if<variable>(&v) != nullptr) return VAR;
        if (std::get_if<variables>(&v) != nullptr) return VEC_VAR;
        return UNKNOWN;
    }

    variable merge(const variable &a, const variable &b) {
        switch (get_op_id(oper)) {
            case 0:
                return {};//a.second * b.second;
            default:
                return {};
        }
    }

    void merge(const variables &v) {
        const auto tl = get_buf_type(left->buffer);
        const auto tr = get_buf_type(right->buffer);

        if (tl == tr && tl == VAR) {
            variables vec;
            vec.emplace_back(std::get<variable>(left->buffer));
            vec.emplace_back(std::get<variable>(right->buffer));
            buffer = std::move(vec);
        }

        ///todo: add case, when one is variable and other is variables; add case when both are variables

        if (tl == VEC_VAR) {
            if (tr == VEC_VAR) {
                variables vec;
                for (auto &x : v) {
                    const auto buf1 = search_name(std::get<variables>(left->buffer), x.first);
                    const auto buf2 = search_name(std::get<variables>(right->buffer), x.first);
                    if (buf1 != -1 && buf2 != -1) {
                        vec.push_back(merge(x, std::get<variables>(left->buffer)[buf1]));
                        vec.push_back(merge(vec.front(), std::get<variables>(right->buffer)[buf2]));
                    } else if (buf1 != -1) {
                        vec.push_back(merge(x, std::get<variables>(left->buffer)[buf1]));
                    } else if (buf2 != -1) {
                        vec.push_back(merge(x, std::get<variables>(right->buffer)[buf2]));
                    }
                }
                buffer = std::move(vec);
            }
        }
    }

    void set(const variables &v) {
        value = v;
        // Дойти до листьев
        if (left != nullptr)
            left->set(v);
        if (right != nullptr)
            right->set(v);

        // Сделать их интервалами
        if (left == nullptr && right == nullptr) {
            if (const auto buf = search_name(v, what); buf != -1) {
                buffer = v[buf];
                return;
            }

            switch (get_type(what)) {
                case STRING: {
                    interval::interval<std::string> buf;
                    buf.add_point(what);
                    std::get<variable>(buffer).second = buf;
                    return;
                }
                case FLOAT: {
                    interval::interval<typeFloat> buf;
                    buf.add_point(std::stod(what));
                    std::get<variable>(buffer).second = buf;
                    return;
                }

                case INT: {
                    interval::interval<typeInt> buf;
                    buf.add_point(std::stoll(what));
                    std::get<variable>(buffer).second = buf;
                    return;
                }

                default:
                    throw std::runtime_error("Unknown type!");
            }
        }

        // Вернуться и начать их объединять
        if (left != nullptr && right != nullptr) {
            merge(v);
        }
    }

};

#endif