//
// Created with =uwu= 05.03.2025.
//

#ifndef AST_H
#define AST_H
// ReSharper disable CppUseStructuredBinding
#include <utility>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <algorithm>
#include <iostream>

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
#define enable_none(name, ...) case custom::custom_type::types::NONE: return name<void>(__VA_ARGS__);
#define close_cession(T) \
default: throw std::runtime_error("unknown type");}} \
template<typename T>
#define enable_all_types(name, ...) \
enable_int(name, __VA_ARGS__) \
enable_float(name, __VA_ARGS__) \
enable_string(name, __VA_ARGS__)

namespace custom {
    struct custom_type;
}

namespace parse {
    std::string from_any_to_str(const std::optional<std::variant<
            interval::interval<typeInt>,
            interval::interval<typeFloat>,
            interval::interval<std::string>,
            std::vector<custom::custom_type*>>> &what);
}

namespace ast {
    const std::vector<std::string> inline operations =
        {"and", "or", "not", "==", "!=", ">=", "<=", ">", "<", "+", "-", "*", "//", "%"};
    enum operations {
        AND, OR, NT, EQ, NQ, ME, LE, MO, LS, PL, MN, PW, DL, PS
    };
}

namespace custom {

    template<typename type>
    [[maybe_unused]] [[nodiscard]] bool dereferenced_sort_comparator(const type *_a, const type *_b) {
        return *_a < *_b;
    }

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
            return {vec, pos + l, std::min(sz, len - pos)};
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

    using str_type = vec_line<std::string>;

    [[nodiscard]] str_type inline erase_spaces(const str_type &other) {
        size_t l = 0, r = other.size();
        while (l < other.size() && other[l] == ' ') ++l;
        if (l == other.size()) return other.substr(0, 0);
        while (r > 0 && other[r - 1] == ' ') --r;
        return other.substr(l,  r - l);
    }


    struct custom_type;
    [[nodiscard]] static short get_in_type(const
                std::optional<std::variant< // aka custom_type::inner_type, but this using will be written after
                interval::interval<typeInt>,
                interval::interval<typeFloat>,
                interval::interval<std::string>,
                std::vector<custom_type*>>> &d);



    struct custom_type {
        using inner_type = std::optional<std::variant<
                interval::interval<typeInt>,
                interval::interval<typeFloat>,
                interval::interval<std::string>,
                std::vector<custom_type*>>>;

        enum types {
            INT, FLOAT, STRING, VECTOR, NONE
        };


        template<typename type>
        [[nodiscard]] static bool can_cast(const std::variant<
                interval::interval<typeInt>,
                interval::interval<typeFloat>,
                interval::interval<std::string>,
                std::vector<custom_type*>> &d) { return std::get_if<type>(&d) != nullptr; }

        [[nodiscard]] static short extract_type_from_string(const std::string &s) {
            if ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"')) return STRING;
            bool has_dot = false;
            if (s.size() == 1 && s.front() == '.') return NONE;
            for (auto i = 0; i < s.size(); ++i) {
                if (s[i] == '-' && i == 0) continue;
                if (s[i] == '.') {
                    if (has_dot) throw std::runtime_error("Function get_type() : double dot in integer-like type!");
                    has_dot = true;
                }
                else if (s[i] > '9' || s[i] < '0') return NONE;
            }
            return has_dot ? FLOAT : INT;
        }

        short type = NONE;
        inner_type data;
        std::string name{};
        std::pair<std::size_t, custom_type*> history{};
        custom_type *parent = nullptr;

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

        [[nodiscard]] bool operator<(const custom_type &a) const { return name < a.name; }

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

        friend inner_type & operator+=(inner_type &a, const inner_type &b)
                { checkType(a, b, "+="); a = (a + b); return a; }
        friend inner_type & operator-=(inner_type &a, const inner_type &b)
                { checkType(a, b, "-="); a = (a - b); return a; }
        friend inner_type & operator*=(inner_type &a, const inner_type &b)
                { checkType(a, b, "*="); a = (a * b); return a; }
        friend inner_type & operator/=(inner_type &a, const inner_type &b)
                { checkType(a, b, "/="); a = (a / b); return a; }
        friend inner_type & operator%=(inner_type &a, const inner_type &b)
                { checkType(a, b, "%="); a = (a % b); return a; }

        void rollback(const std::vector<std::vector<custom::custom_type*>> &v) const;

    protected:

        [[nodiscard]] std::string rollback_in() const {
            if (name.empty()) {
                return parse::from_any_to_str(data);
            }
            return rollback_in(name);
        }

        [[nodiscard]] std::string rollback_in(const std::string &a) const {
            if (parent == nullptr) {
                return a;
            }
            if (history.first <= ast::operations::LS) return parent->rollback_in(a);

            const std::string s = "(" + a + " " + inverse_operator(history.first) + " " + history.second->rollback_in() + ")";
            return parent->rollback_in(s);
            /*
             a += 3
             a *= 2
             -> ((a / 2) - 3)
             */
        }

        static std::string inverse_operator(const std::size_t op) {
            switch (op) {
                case ast::operations::EQ: return "!=";
                case ast::operations::NQ: return "==";
                case ast::operations::ME: return "<";
                case ast::operations::LE: return ">";
                case ast::operations::MO: return "<=";
                case ast::operations::LS: return ">=";
                case ast::operations::PL: return "-";
                case ast::operations::MN: return "+";
                case ast::operations::PW: return "//";
                case ast::operations::DL: return "*";
                default: throw std::runtime_error("unknown operation in custom::custom_type::inverse_operator");
            }
        }

        template<typename T>
        [[nodiscard]] static inner_type less_in(
                const std::function<void(interval::interval<T> &it, const inner_type &a)> &it,
                inner_type &a,
                inner_type &b,
                const std::string &n) {
            checkType(a, b, n);
            auto check = [](const inner_type &a, inner_type &b) -> void {
                if (get_in_type(a) == FLOAT) {
                    if (get_in_type(b) == INT)
                        b = std::get<interval::interval<typeInt>>(b.value()).cast<typeFloat>();
                }
            };
            check(a, b);
            check(b, a);
            interval::interval<T> buf;
            it(buf, b);
            return buf * std::get<interval::interval<T>>(a.value());
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
            inner_type c = a, d = b;
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a.value()).any().value());
            }, c, d, "<");
        }

        template<typename T>
        [[nodiscard]] static inner_type more(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a.value()).any().value(),interval::maximal<T>());
            }, c, d, ">");
        }

        template<typename T>
        [[nodiscard]] static inner_type less_equal(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a.value()).any().value());
                buf.add_point(std::get<interval::interval<T>>(a.value()).any().value());
            }, c, d, "<=");
        }

        template<typename T>
        [[nodiscard]] static inner_type more_equal(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a.value()).any().value(), interval::maximal<T>());
                buf.add_point(std::get<interval::interval<T>>(a.value()).any().value());
            }, c, d, ">=");
        }

        template<typename T>
        [[nodiscard]] static inner_type equal(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_point(std::get<interval::interval<T>>(a.value()).any().value());
            }, c, d, "==");
        }

        template<typename T>
        [[nodiscard]] static inner_type not_equal(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return less_in<T>([](interval::interval<T> &buf, const inner_type &a) {
                buf.add_interval(std::get<interval::interval<T>>(a.value()).any().value(), interval::maximal<T>());
                buf.add_interval(interval::minimal<T>(), std::get<interval::interval<T>>(a.value()).any().value());
            }, c, d, "!=");
        }

        template<typename T>
        [[nodiscard]] static inner_type add(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return std::get<interval::interval<T>>(c.value())
                   + std::get<interval::interval<T>>(d.value()).any().value();
        }

        template<typename type>
        [[nodiscard]] static inner_type subtract(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return std::get<interval::interval<type>>(c.value())
                   - std::get<interval::interval<type>>(d.value()).any().value();
        }

        template<typename type>
        [[nodiscard]] static inner_type multiply(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return std::get<interval::interval<type>>(c.value())
                   * std::get<interval::interval<type>>(d.value()).any().value();
        }

        template<typename type>
        [[nodiscard]] static inner_type divide(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return std::get<interval::interval<type>>(c.value())
                   / std::get<interval::interval<type>>(d.value()).any().value();
        }

        [[nodiscard]] static inner_type mod(const inner_type &a, const inner_type &b) {
            inner_type c = a, d = b;
            return std::get<interval::interval<typeInt>>(c.value())
                   % std::get<interval::interval<typeInt>>(d.value()).any().value();
        }
    };



    [[nodiscard]] static short get_in_type(const custom_type::inner_type &d) {
        if (!d.has_value()) return custom_type::types::NONE;
        if (custom_type::can_cast<interval::interval<typeInt>>(d.value())) return custom_type::types::INT;
        if (custom_type::can_cast<interval::interval<typeFloat>>(d.value())) return custom_type::types::FLOAT;
        if (custom_type::can_cast<interval::interval<std::string>>(d.value())) return custom_type::types::STRING;
        if (custom_type::can_cast<std::vector<custom_type*>>(d.value())) return custom_type::types::VECTOR;
        throw std::runtime_error("Function: can_cast(), error: UNKNOWN TYPE");

    }

}



namespace ast {
    using variables_t = std::vector<std::vector<custom::custom_type*>>;
    inline std::size_t inverse_operator(const std::size_t op) {
        switch (op) {
            case MO: return LE;
            case EQ: return NQ;
            case NQ: return EQ;
            case LS: return ME;
            case ME: return LS;
            case LE: return MO;
            default:
                throw std::runtime_error("Unknown operation");
        }
    }

    struct ast_node {
        ast_node *parent = nullptr;
        ast_node *l = nullptr, *r = nullptr;
        std::optional<custom::custom_type*> data{};
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
                    for (auto j = 0; j < operations.size(); ++j) {
                        if (s.substr(i, operations[j].size()).extract() == operations[j] &&
                                    !std::isalpha(s[i + operations[j].size()])) {
                            search[j] = i;
                        }
                    }
                }
            }

            for (auto i = 0; i < search.size(); ++i) {
                if (search[i] != -1) {
                    if (i == MN) {
                        if (erase_spaces(s.substr(0, search[i])).size() == 0) break;
                    }
                    l = new ast_node(this);
                    r = new ast_node(this);
                    op = i;
                    if (op != NT) l->gen(s.substr(0, search[i]));
                    r->gen(s.substr(search[i] + operations[i].size()));
                    return;
                }
            }

            //no operator found
            if (l == nullptr || r == nullptr) {
                const auto type = custom::custom_type::extract_type_from_string(s.extract());
                data = new custom::custom_type;
                *data.value() = (dereference_cast(s.extract(), type));
            } else {
                *data.value() = *l->data.value();
                apply_operator(data.value()->data, r->data.value()->data, op);
            }

        }

        static void apply_operator(custom::custom_type::inner_type &a,
                                   const custom::custom_type::inner_type &b,
                                   const std::size_t op) {
            switch (op) {
                case PL: a += b; break;
                case MN: a -= b; break;
                case PW: a *= b; break;
                case DL: a /= b; break;
                case PS: a %= b; break;
                case EQ: a = a == b; break;
                case NQ: a = a != b; break;
                case MO: a = a > b; break;
                case ME: a = a >= b; break;
                case LS: a = a < b; break;
                case LE: a = a <= b; break;
                default: throw std::runtime_error("unknown operator in ast::ast_node::apply_operator");
            }
        }

        init_struct_cession(a.data, static custom::custom_type not_reverse(const custom::custom_type &a))
            enable_all_types(not_reverse, a)
        close_cession(T)
        static custom::custom_type not_reverse(const custom::custom_type &a) {
            auto out = a;
            out.data = std::get<interval::interval<T>>(a.data.value()).invert();
            return out;
        }

        static void not_operator(const variables_t &orig, variables_t &out, const std::size_t ind = 0) {
            static std::vector<custom::custom_type*> stack;
            if (ind == orig.size()) {
                out.emplace_back();
                std::vector<custom::custom_type *> sub;
                for (const auto &i : stack) {
                    sub.push_back(new custom::custom_type);
                    *sub.back() = not_reverse(*i);
                }
                std::sort(sub.begin(), sub.end(),
                    custom::dereferenced_sort_comparator<custom::custom_type>);
                if (sub.empty()) return;
                out.back().push_back(sub.front());
                for (auto i = 1; i < sub.size(); ++i) {
                    if (sub[i]->name != sub[i - 1]->name) {
                        out.back().push_back(sub[i]);
                    }
                    else {
                        out.back().back()->data = sub_and(sub[i]->data, out.back().back()->data);
                    }
                }
                return;
            }
            for (auto &i : orig[ind]) {
                stack.push_back(new custom::custom_type);
                *stack.back() = *i;
                not_operator(orig, out, ind + 1);
                stack.pop_back();
            }
        }

        init_struct_cession(a,
            static custom::custom_type::inner_type sub_and(const custom::custom_type::inner_type &a,
                    const custom::custom_type::inner_type &b))
            enable_all_types(sub_and, a, b)
        close_cession(T)
        static custom::custom_type::inner_type sub_and(const custom::custom_type::inner_type &a,
            const custom::custom_type::inner_type &b) {
            return std::get<interval::interval<T>>(a.value()) * std::get<interval::interval<T>>(b.value());
        }

        [[nodiscard]] static variables_t once_not(const variables_t &value, const variables_t &orig) {
            variables_t out;
            not_operator(value, out);
            cmpPush(out, orig);
            variables_t ld = orig, rd = out;
            out.clear();
            for (auto &i : ld) {
                for (auto &j : rd) {
                    std::sort(i.begin(), i.end(),
                        custom::dereferenced_sort_comparator<custom::custom_type>);
                    std::sort(j.begin(), j.end(),
                        custom::dereferenced_sort_comparator<custom::custom_type>);
                    out.emplace_back();
                    std::size_t i1 = 0, i2 = 0;
                    while (i1 < i.size() && i2 < j.size()) {
                        if (i[i1]->name == j[i2]->name) {
                            out.back().push_back(new custom::custom_type);
                            *out.back().back() = *i[i1];
                            out.back().back()->data = sub_and(i[i1]->data, j[i2]->data);
                            ++i1;
                            ++i2;
                        }
                        else if (i[i1]->name < j[i2]->name) {
                            ++i1;
                        }
                        else {
                            out.back().push_back(j[i2]);
                            ++i2;
                        }
                    }
                    while (i2 < j.size()) {
                        out.back().push_back(j[i2]);
                        ++i2;
                    }
                }
            }
            return out;
        }

        [[nodiscard]] static variables_t once_and(const variables_t &a, const variables_t &b) {
            variables_t ld = a, rd = b, out;
            for (auto &i : ld) {
                for (auto &j : rd) {
                    std::sort(i.begin(), i.end(),
                        custom::dereferenced_sort_comparator<custom::custom_type>);
                    std::sort(j.begin(), j.end(),
                        custom::dereferenced_sort_comparator<custom::custom_type>);
                    out.emplace_back();
                    std::size_t i1 = 0, i2 = 0;
                    while (i1 < i.size() && i2 < j.size()) {
                        if (i[i1]->name == j[i2]->name) {
                            out.back().push_back(new custom::custom_type);
                            *out.back().back() = *i[i1];
                            out.back().back()->data = sub_and(i[i1]->data, j[i2]->data);
                            ++i1;
                            ++i2;
                        }
                        else if (i[i1]->name < j[i2]->name) {
                            out.back().push_back(i[i1]);
                            ++i1;
                        }
                        else {
                            out.back().push_back(j[i2]);
                            ++i2;
                        }
                    }
                    while (i1 < i.size()) {
                        out.back().push_back(i[i1]);
                        ++i1;
                    }
                    while (i2 < j.size()) {
                        out.back().push_back(j[i2]);
                        ++i2;
                    }
                }
            }
            return out;
        }

        [[nodiscard]] static variables_t once_or(const variables_t &a, const variables_t &b) {
            variables_t ld = a, rd = b, out;
            std::set<std::vector<custom::custom_type*>> buf_out;
            for (auto &i : ld) {
                std::sort(i.begin(), i.end(),
                    custom::dereferenced_sort_comparator<custom::custom_type>);
                buf_out.insert(i);
            }
            for (auto &i : rd) {
                std::sort(i.begin(), i.end(),
                    custom::dereferenced_sort_comparator<custom::custom_type>);
                buf_out.insert(i);
            }
            for (auto &i : buf_out) {
                out.push_back(i);
            }
            return out;
        }

        [[nodiscard]] variables_t get_variables(const variables_t &orig) const {
            variables_t out;
            if (data.has_value()) {
                for (const auto &v : orig) {
                    if (data.value()->name.empty()) {
                        out.push_back({data.value()});
                    }
                    else {
                        for (const auto &i : v) {
                            if (i->name == data.value()->name) {
                                out.push_back({i});
                                break;
                            }
                        }
                    }
                }
            }
            else {
                auto check = [](const custom::custom_type::inner_type &a, custom::custom_type::inner_type &b) -> void {
                    if (custom::get_in_type(a) == custom::custom_type::types::FLOAT) {
                        if (custom::get_in_type(b) == custom::custom_type::types::INT)
                            /// todo fix: WARNING: BROKEN_POLITIC::DESTRUCTED_ALLOCATOR
                            b = std::get<interval::interval<typeInt>>(b.value()).cast<typeFloat>();
                    }
                };
                variables_t ld, rd = r->get_variables(orig);
                if (op != NT) ld = l->get_variables(orig);
                for (auto &v : ld) std::sort(v.begin(), v.end(),
                        custom::dereferenced_sort_comparator<custom::custom_type>);
                for (auto &v : rd) std::sort(v.begin(), v.end(),
                        custom::dereferenced_sort_comparator<custom::custom_type>);

                switch (op) {
                    case PL:
                    case MN:
                    case PW:
                    case DL:
                    case PS:
                    case EQ:
                    case NQ:
                    case MO:
                    case ME:
                    case LS:
                    case LE: {
                        for (auto &i : ld) {
                            for (auto &j : rd) {
                                if (i.front()->name == j.front()->name && !i.front()->name.empty()) {
                                    out.push_back(i);
                                    continue;
                                }
                                if (i.size() != 1 || j.size() != 1)
                                    throw std::logic_error("Wrong using operator " + operations[op]);
                                out.push_back({new custom::custom_type});
                                if (i.front()->name.empty()) {
                                    *out.back().front() = *j.front();
                                    check(out.back().front()->data, i.front()->data);
                                    check(i.front()->data, out.back().front()->data);
                                    out.back().front()->history = {op, i.front()};
                                    out.back().front()->parent = j.front();
                                    apply_operator(out.back().front()->data, i.front()->data, op);
                                }
                                else {
                                    *out.back().front() = *i.front();
                                    check(out.back().front()->data, j.front()->data);
                                    check(j.front()->data, out.back().front()->data);
                                    out.back().front()->history = {op, j.front()};
                                    out.back().front()->parent = i.front();
                                    apply_operator(out.back().front()->data, j.front()->data, op);
                                }
                            }
                        }
                        break;
                    }
                    case NT: {
                        if (!ld.empty()) throw std::logic_error("Wrong using operator " + operations[op]);
                        return once_not(rd, orig);
                    }
                    case AND: { return once_and(ld, rd); }
                    case OR: { return once_or(ld, rd); }
                    default: { throw std::runtime_error("unknown operator in ast::ast_node::get_variables"); }
                }

            }
            return out;
        }

        init_struct_cession(a, [[nodiscard]] static bool is_empty(const custom::custom_type::inner_type &a))
            enable_all_types(is_empty, a)
        close_cession(T)
        [[nodiscard]] static bool is_empty(const custom::custom_type::inner_type &a) {
            return std::get<interval::interval<T>>(a.value()).empty();
        }

        static void cmpPush(variables_t &write, const variables_t &orig) {
            auto buf = write;
            write.clear();
            std::set<std::vector<custom::custom_type*>> tmp;
            for (auto &i : orig) {
                for (auto &j : buf) {
                    // write.push_back(i);
                    std::vector<custom::custom_type*> w = i;
                    auto buf_index = 0;
                    for (auto &k : w) {
                        if (buf_index < j.size() && k->name == j[buf_index]->name) {
                            k = j[buf_index++];
                        }
                    }
                    tmp.insert(w);
                }
            }
            for (auto &i : tmp) {
                write.push_back(i);
            }
        }

        void tree() const {
            const std::string s;
            tree(this, s);
            std::cout << std::flush;
        }

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
                    buf.add_point(s.substr(1, s.size() - 2));
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

    private:
        init_struct_cession(v.data, static void sub_print(custom::custom_type &v))
            enable_all_types(sub_print, v)
            enable_none(sub_print, v)
        close_cession(T)
        static void sub_print(custom::custom_type &v) {
            if constexpr (std::is_void<T>()) {
                std::cout << v.name;
            }
            else std::cout << std::get<interval::interval<T>>(v.data.value()).print();
        }
        static void tree(const ast_node *_root, const std::string &move) {
            if (_root->data != std::nullopt) {
                std::cout << move << ' ';
                sub_print(*_root->data.value());
                std::cout << '\n';
                return;
            }
            auto move_new = move;
            if (!move_new.empty()) {
                for (auto i = move_new.size(); i --> move_new.size() - 4; )
                    move_new[i] = '.';
            }
            move_new += "|---";
            tree(_root->l, move_new);
            std::cout << move << " operator " << operations[_root->op] + '\n';
            tree(_root->r, move_new);
        }


    };

    [[nodiscard]] inline ast_node * generate_ast(const custom::str_type &s) {
        auto *root = new ast_node(nullptr);
        root->gen(s);
        return root;
    }

}
#endif
// not (a > 3 and b < 4) or (c > 5 and d < 6) {{a > 3; b < 4}; {c > 5; d < 6}}
//  {{a <= 3; c <= 5}; {a <= 3; d >= 6}; {b >= 4; c <= 5}; {b >= 4; d >= 6}}

// not {{a; b}, {c; d}, {e; f}}
// {{a, c, e}, {a, d, }}
