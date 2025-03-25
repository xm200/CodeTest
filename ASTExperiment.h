//
// Created by xm200 on 05.03.2025. =uwu=
//

#ifndef AST_H
#define AST_H
// ReSharper disable CppUseStructuredBinding
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <utility>
#include <string>
#include <vector>
#include <queue>
#include <variant>
#include <functional>

namespace custom {
#define init_cession(a, ...) \
template<typename> \
__VA_ARGS__; \
__VA_ARGS__ { \
switch(get_in_type(a)) {
#define init_struct_cession(a, ...) \
__VA_ARGS__ { \
switch(get_in_type(a)) {
#define enable_int(name, ...) case custom::custom_type::types::INT: return name<typeInt>(__VA_ARGS__);
#define enable_float(name, ...) case custom::custom_type::types::FLOAT: return name<typeFloat>(__VA_ARGS__);
#define enable_string(name, ...) case custom::custom_type::types::STRING: return name<std::string>(__VA_ARGS__);
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

        [[maybe_unused]] explicit vec_line(const S &data) : vec(&data), len(data.size()) {}

        [[maybe_unused]] vec_line(const S *vec_, const std::size_t l_, const std::size_t len_) :
                vec(vec_), l(l_), len(len_) {}

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



    std::vector<std::string> inline operations =
        {"and", "or", "==", "!=", ">", "<", ">=", "<=", "+", "-", "//", "%"};

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
            if (other.front()== '(' && other.back() == ')') return go(other.substr(1, other.size() - 2));
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
        return other.substr(l,  r);
    }


    struct custom_type;
    [[nodiscard]] static short get_in_type(const std::variant<
                interval::interval<typeInt>,
                interval::interval<typeFloat>,
                interval::interval<std::string>,
                std::vector<custom_type*>> &d);


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


        template<typename type>
        [[nodiscard]] static bool can_cast(const inner_type &d) {
            return std::get_if<type>(&d) != nullptr;
        }

        [[nodiscard]] static short extract_type_from_string(const std::string &s) {
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

        short type = UNKNOWN;
        inner_type data;
        std::string name{};

        custom_type() = default;
        explicit custom_type(const inner_type& d) : type(get_in_type(d)), data(d) {}

        ~custom_type() = default;



        void reset_type() { type = get_in_type(data); }

#define subst(fun, a, b) \
    switch (get_in_type(a)) { \
        case INT: return fun<typeInt>(a, b); \
        case FLOAT: return fun<typeFloat>(a, b); \
        case STRING: return fun<std::string>(a, b); \
        default: throw std::runtime_error("unknown type"); \
    }

#define subst_digit_only(fun, a, b)  \
    switch (get_in_type(a)) { \
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
            type = get_in_type(a);
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


        [[nodiscard]] static std::size_t search_name(const variables &v, const str_type &s) {
            for (std::size_t i = 0; i < v.size(); ++i)
                if (v[i].first.extract() == s.extract()) return i;
            return -1;
        }

        [[nodiscard]] static std::pair<str_type, inner_type> dereference_cast(const str_type &s, const variables &v) {
            switch (const auto _s = erase_spaces(s).extract(); extract_type_from_string(_s)) {
                case INT: {
                    interval::interval<typeInt> buf;
                    buf.add_point(std::stoll(_s));
                    str_type data;
                    return {data, buf};
                }
                case FLOAT: {
                    interval::interval<typeFloat> buf;
                    buf.add_point(std::stod(_s));
                    str_type data;
                    return {data, buf};
                }
                case STRING: {
                    interval::interval<std::string> buf;
                    buf.add_point(_s);
                    str_type data;
                    return {data, buf};
                }
                default: {
                    return v[search_name(v, s)];
                }
            }
        }

        [[nodiscard]] static std::vector<std::pair<str_type, inner_type>> merge(
            const std::vector<std::pair<str_type, inner_type>> &a,
            const std::vector<std::pair<str_type, inner_type>> &b,
            const std::vector<std::pair<str_type, inner_type>> &v,
            const str_type &oper) {

            short id = -1;
            for (int i = 0; i < operations.size(); ++i) if (operations[i] == oper.extract()) {id = i; break;}
            if (id == -1) throw std::runtime_error("push() called with invalid id!");

            std::vector<std::pair<str_type, inner_type>> res(v.size());
            ///todo: debug it tomorrow, because it would not work now
            for (int i = 0; i < v.size(); ++i) {
                const auto buf = merge(a[i].second, b[i].second, id, v[i].first);
                res[i].first = v[i].first;
                res[i].second = v[i].second * buf.second;
            }
        }

        [[nodiscard]] static std::pair<str_type, inner_type> merge(
                const inner_type &a, const inner_type &b, const short op_number, const str_type &name) {
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
                return {dereference_cast(other, v)};
            } if (split_idx.second == 1) {
                ///todo: add or processing
            }
            const auto l1 = split_idx.first.first;
            const auto r1 = split_idx.first.second;
            auto p1 = _extract(other.substr(0, l1), v);
            auto p2 = _extract(other.substr(r1), v);

            const auto buf = other.substr(l1, r1 - l1 + 1);

            // return merge(p1, p2, buf);
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
            if (get_in_type(b) != get_in_type(a))
                throw std::logic_error("Function operator " + name_op + ", error: could not compare different types!");
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


    [[nodiscard]] static short get_in_type(const std::variant<
                 interval::interval<typeInt>,
                 interval::interval<typeFloat>,
                 interval::interval<std::string>,
                 std::vector<custom_type*>> &d) {
        if (custom_type::can_cast<interval::interval<typeInt>>(d)) return custom_type::types::INT;
        if (custom_type::can_cast<interval::interval<typeFloat>>(d)) return custom_type::types::FLOAT;
        if (custom_type::can_cast<interval::interval<std::string>>(d)) return custom_type::types::STRING;
        if (custom_type::can_cast<std::vector<custom_type*>>(d)) return custom_type::types::VECTOR;
        throw std::runtime_error("Function: can_cast(), error: UNKNOWN TYPE");

    }

}



namespace ast {
    const std::vector<std::string> inline operations =
        {"and", "or", "==", "!=", ">", "<", ">=", "<=", "+", "-", "//", "%"};
    struct ast_node {
        ast_node *parent = nullptr;
        ast_node *l = nullptr, *r = nullptr;
        std::optional<custom::custom_type> data{};
        std::size_t op = -1;

        explicit ast_node(ast_node *p) : parent(p) {}
        void gen(const custom::str_type &orig_s) {
            auto s = erase_spaces(orig_s);


            if (s.front() == '(' && s.back() == ')') {
                std::size_t depth = 0;
                for (auto i = 0; i < s.size() - 1; ++i) {
                    if (s[i] == '(') ++depth;
                    if (s[i] == ')') --depth;
                    if (depth == 0) goto point;
                }
                gen(s.substr(1, s.size() - 2));
                return;
            }

            point:
            std::vector<std::size_t> search(operations.size(), -1);
            std::size_t depth = 0;
            for (auto i = 0; i < s.size(); ++i) {
                if (s[i] == '(') ++depth;
                if (s[i] == ')') --depth;
                if (depth == 0) {
                    for (auto op = 0; op < operations.size(); ++op) {
                        if (s.substr(i, operations[op].size()).extract() == operations[op]) {
                            search[op] = i;
                        }
                    }
                }
            }

            for (auto i = 0; i < search.size(); ++i) {
                if (search[i] != -1) {
                    l = new ast_node(this);
                    r = new ast_node(this);
                    op = i;
                    l->gen(s.substr(0, search[i]));
                    r->gen(s.substr(search[i] + operations[i].size()));
                    return;
                }
            }

            //no operator found
            const auto type = custom::custom_type::extract_type_from_string(orig_s.extract());
            data = dereference_cast(s.extract(), type);

        }

        void tree() {
            const std::string s;
            tree(this, s);
            std::cout << std::flush;
        }

    private:
        [[nodiscard]] static custom::custom_type dereference_cast (const std::string &s, const short type) {
            custom::custom_type ret;
            switch (type) {
                case custom::custom_type::INT: {
                    interval::interval<typeInt> buf;
                    buf.add_point(std::stoll(s));
                    ret.data = buf;
                    break;
                }
                case custom::custom_type::FLOAT: {
                    interval::interval<typeFloat> buf;
                    buf.add_point(std::stod(s));
                    ret.data = buf;
                    break;
                }
                case custom::custom_type::STRING: {
                    interval::interval<std::string> buf;
                    buf.add_point(s);
                    ret.data = buf;
                    break;
                }
                default: {
                    ret.name = s;
                    break;
                }
            }
            ret.reset_type();
            return ret;
        }

        init_struct_cession(v, static void sub_print(custom::custom_type::inner_type &v))
            enable_all_types(sub_print, v)
        close_cession(T)
        static void sub_print(custom::custom_type::inner_type &v) {
            std::cout << std::get<interval::interval<T>>(v).print();
        }
        static void tree(ast_node *_root, const std::string &move) {
            if (_root->data != std::nullopt) {
                std::cout << move;
                sub_print(_root->data.value().data);
                std::cout << std::endl;
                return;
            }
            std::cout << move << operations[_root->op] + '\n';
            auto move_new = move;
            if (!move_new.empty()) {
                for (auto i = move_new.size(); i --> move_new.size() - 4; )
                    move_new[i] = '.';
            }
            move_new += "|---";
            tree(_root->l, move_new);
            tree(_root->r, move_new);
        }
    };

    [[nodiscard]] inline ast_node * generate_ast(const custom::str_type &s) {
        auto *root = new ast_node(nullptr);
        root->gen(s);
        return root;
    }

}
#undef init_cession
#undef init_struct_cession
#undef enable_all_types
#undef enable_int
#undef enable_float
#undef enable_string
#undef close_cession
#endif