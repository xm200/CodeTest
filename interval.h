#ifndef interval_H
#define interval_H
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <functional>

namespace interval {

/// return always minimal element in any interval
    template <typename T>
    [[nodiscard]] std::pair<int, T> minimal() noexcept {return {0, {}};}
/// return always maximal element in any interval
    template <typename T>
    [[nodiscard]] std::pair<int, T> maximal() noexcept {return {2, {}};}

    template<typename T>
    class interval {
    public:
        using inner_type = std::pair<int, T>;
        // structure can receive data with T type and inner_type (for interval::minimal and interval::maximal)
        using inp_type = std::variant<T, inner_type>;

        interval() = default;
        ~interval() = default;

        interval(const interval &other): points(other.points), intervals(other.intervals) {}
        interval(interval &&other) noexcept: points(std::move(other.points)), intervals(std::move(other.intervals)) {}

        interval & operator=(const interval &other) {
            if (this == &other) return *this;
            points = other.points, intervals = other.intervals;
            return *this;
        }
        interval & operator=(interval &&other) noexcept {
            if (this == &other) return *this;
            points = std::move(other.points), intervals = std::move(other.intervals);
            return *this;
        }

        friend void swap(interval &lhs, interval &rhs) noexcept {
            using std::swap;
            swap(lhs.points, rhs.points);
            swap(lhs.intervals, rhs.intervals);
        }

        [[nodiscard]] bool operator==(const interval &b) const noexcept {
            return this == &b || (points == b.points && intervals == b.intervals);
        }
        [[nodiscard]] bool operator!=(const interval &b) const noexcept {
            return !(*this == b);
        }

        // standard operations

        /// returns false if this point was inside this multitude, else return true
        bool add_point(const inp_type &a) {return add_point_in(to_point(a));}
        /// returns false if this point was not inside this multitude, else return true
        bool remove_point(const inp_type &a) {return remove_point_in(to_point(a));}

        /// returns false if all this interval was inside this multitude, else return true
        bool add_interval(const inp_type &a, const inp_type &b)
        {return add_interval_in(to_point(a), to_point(b));}
        /// returns false if all this interval was not inside this multitude, else return true
        bool remove_interval(const inp_type &a, const inp_type &b)
        {return remove_interval_in(to_point(a), to_point(b));}

        /// return true if this multitude is empty, else return false
        [[nodiscard]] bool empty() const noexcept {return points.empty() && intervals.empty();}
        /// clear multitude data
        void clear() {points.clear(); intervals.clear();}

        // math operations

        /// a | b
        [[nodiscard]] friend interval operator+(interval a, const interval &b) {
            a += b;
            return a;
        }
        /// a |= b
        interval &operator+=(const interval &b) {
            if (this == &b) return *this;
            for (auto &i : b.points) add_point_in(i);
            for (auto &i : b.intervals) add_interval_in(i.first, i.second);
            return *this;
        }

        /// a / b (in math style)
        [[nodiscard]] friend interval operator-(interval a, const interval &b) {
            a -= b;
            return a;
        }
        /// a = a / b (in math style)
        interval &operator-=(const interval &b) {
            if (this == &b) {
                clear();
                return *this;
            }
            for (auto &i : b.points) remove_point_in(i);
            for (auto &i : b.intervals) remove_interval_in(i.first, i.second);
            return *this;
        }

        /// a & b
        [[nodiscard]] friend interval operator*(const interval &a, const interval &b) {
            interval x, y, z;
            a.invert_in(x), b.invert_in(y);
            x += y;
            x.invert_in(z);
            return z;
        }
        /// a &= b
        friend interval &operator*=(interval &a, const interval &b) {
            if (&a == &b) return a;
            a = a * b;
            return a;
        }

        // transfer operations

        [[nodiscard]] interval operator+(const T val) const {interval b; plus_in(b, val); return b;}
        friend interval & operator+=(interval &a, const T val)
                {interval b; a.plus_in(b, val); a = std::move(b); return a;}

        [[nodiscard]] interval operator-(const T val) const {interval b; plus_in(b, -val); return b;}
        friend interval & operator-=(interval &a, const T val) {
            static_assert(std::is_arithmetic<T>(), "you cannot do transfer operations with strings");
            interval b; a.plus_in(b, -val); a = std::move(b); return a; // because I use -val, in strings it is error
        }

        [[nodiscard]] interval operator*(const T val) const {interval b; multiply_in(b, val); return b;}
        friend interval & operator*=(interval &a, const T val)
                {interval b; a.multiply_in(b, val); a = std::move(b); return a;}

        [[nodiscard]] interval operator/(const T val) const {interval b; division_in(b, val); return b;}
        friend interval & operator/=(interval &a, const T val)
                {interval b; a.division_in(b, val); a = std::move(b); return a;}

        [[nodiscard]] interval operator%(const T val) const {interval b; remainder_in(b, val); return b;}
        friend interval & operator%=(interval &a, const T val)
        {interval b; a.remainder_in(b, val); a = std::move(b); return a;}

        // advanced operations

        /// returns (-INF; +INF) / this multitude
        [[nodiscard]] interval invert() const {
            interval buf;
            invert_in(buf);
            return buf;
        }

        /// return string with all data in mathematical style
        [[nodiscard]] std::string print() const {return print_in();}

        /**
         * @brief return any elem that is in data
         *
         * function returns @c std::optional, because returning value does not always exists
         *
         * @tableofcontents if there is any point, it will be returned
         *
         * @tableofcontents if there is interval (-INF; +INF), will be returned T{}
         *
         * @tableofcontents if T is integer or not-integer digit,
         * smart algorithm will try to found any number in intervals
         *
         * @tableofcontents if T is std::string, smart algorithm will try to found any string in intervals,
         * considering, that string may have only capital english letters
         *
         * @tableofcontents if T - other type, or smart algorithm will not found any elem in data,
         * will be returned @c std::nullopt
         *
         * for custom types and algorithms consider to use this function with additional arguments
         */
        [[nodiscard]] std::optional<T> any() const {
            if (!points.empty()) return points.begin()->second; // if there is any point: return it
            if constexpr (std::is_arithmetic<T>()) {
                return get_any_for_number(intervals); // try to return something in intervals
            }
            return get_any_for_not_number(intervals); // try to return something in intervals
        }

        /**
         * @brief return any elem that is in data
         *
         * function returns @c std::optional, because returning value does not always exists
         *
         * @brief function gets 3 lambda functions that must return @c std::optional<T>
         *
         * @arg first lambda: return if there is interval (-INF; x), gets one const T& argument
         *
         * @arg second lambda: return if there is interval (x; +INF), gets one const T& argument
         *
         * @arg third lambda: return if there is interval (x; y), gets two const T& arguments
         *
         * if there is interval (-INF; +INF), will be returned T{}.
         *
         * because there lambdas returns std::optional, lambda can return @c std::nullopt,
         * if this interval has not any integer value
         *
         * @warning Please note, that you ourselves detect, that returning value is on interval.
         * Do not forget to monitor for overflows
         */
        [[nodiscard]] std::optional<T> any(
            const std::function<std::optional<T>(const T&)> &MINUS_INF_x,
            const std::function<std::optional<T>(const T&)> &x_PLUS_INF,
            const std::function<std::optional<T>(const T&, const T&)> &x_y) const {
            if (!points.empty()) return points.begin()->second; // if there is any point: return it
            return get_any_functional(intervals, MINUS_INF_x, x_PLUS_INF, x_y);
        }

        /**
         * @brief transfer all elem that is in data. return new @c interval::interval
         *
         * function gets 1 lambda function that must return T.
         * -INF and +INF remain unchanged
         *
         * @arg lambda: return new value of point / border of interval, gets one const T& argument
         *
         * if first value of interval become more, than second,
         * function will swap their automatically
         */
        [[nodiscard]] interval custom_transfer(const std::function<T(const T&)> &fun) const {
            interval b;
            custom_transfer_in(b, fun);
            return b;
        }
        /**
         * @brief transfer all elem that is in data. return new @c interval::interval
         *
         * function gets 1 lambda function (that must return T)
         * and 2 values - result of conversion -INF and +INF.
         * new values can not be -INF and +INF
         *
         * @arg lambda: return new value of point / border of interval, gets one const T& argument
         * @arg raw value: new value of border of interval that begin from -INF
         * @arg raw value: new value of border of interval that end with +INF
         *
         * if first value of interval become more, than second,
         * function will swap their automatically
         */
        [[nodiscard]] interval custom_transfer(const std::function<T(const T&)> &fun,
                                               const T MINUS_INF, const T PLUS_INF) const {
            interval b;
            custom_transfer_in(b, fun, MINUS_INF, PLUS_INF);
            return b;
        }
        /**
         * @brief transfer all elem that is in data. return new @c interval::interval
         *
         * @warning this function is need to work with raw memory
         *
         * function gets 2 lambda functions
         * @arg lambda: return new value of interval in raw view (std::pair\<inner_type, inner_type\>),
         * gets one const std::pair\<inner_type, inner_type\>& argument - last value of interval
         * @arg lambda: return new value of point in normal view,
         * gets one const T& argument - last value of point
         *
         * point cannot be -INF or +INF, so it processing standard.
         *
         * in text before @c inner_type is std::pair\<int, T\>. Value of first elem:
         *
         * @tableofcontents 0: -INF. second elem will became T{} automatically
         * @tableofcontents 1: standard value. second elem has necessary value
         * @tableofcontents 2: +INF. second elem will became T{} automatically
         * @tableofcontents other: it is undefined behavior, will throw @c std::range_error automatically
         *
         * if first value of interval become more, than second,
         * will be throw @c std::overflow_error automatically, because it is not normal.
         *
         * if first value of interval is equal to second,
         * this interval will not added
         */
        [[nodiscard]] interval _custom_transfer(
                const std::function<std::pair<inner_type, inner_type>
                        (const inner_type&, const inner_type&)> &L_R,
                const std::function<T(const T&)> &POINT) const {
            interval b;
            custom_transfer_in(b, L_R, POINT);
            return b;
        }

    protected:
        std::set<inner_type> points;
        std::set<std::pair<inner_type, inner_type>> intervals;

        /// convert T-type object to pair inner-type {1; T-type elem};
        /// if object already have inner_type, do nothing
        [[nodiscard]] inner_type to_point(const inp_type &a) const {
            auto x = std::get_if<T>(&a);
            if (x == nullptr) return std::get<inner_type>(a);
            return {1, std::get<T>(a)};
        }

        /// convert second index among -INF and +INF to {}
        [[nodiscard]] static inner_type recover(const inner_type &a) {
            if (a.first < 0 || a.first > 2) throw std::range_error("point or border of interval has undefined value");
            if (a.first != 1) return {a.first, {}};
            return a;
        }

        bool add_point_in(const inner_type &p) {
            if (p.first != 1) return false; // you cannot add points -INF and +INF

            // (x1; a); (a; x2) + {a} = (x1; x2)
            auto x = intervals.lower_bound({p, {}});
            if (x != intervals.end() && x->first == p && x != intervals.begin()) {
                auto y = x;
                --y;
                if (y->second == p) {
                    merge_intervals(*y, *x);
                    return true;
                }
            }

            // (x1; x2) + {a} = (x1; x2) if a in (x1; x2)
            if (get_interval_that_include_this_point(p) != intervals.end()) return false;

            // {a} + {a} = {a}
            if (points.count(p)) {
                return false;
            }

            // something + {a} = something + {a} if {a} not in something
            points.insert(p);
            return true;
        }

        bool remove_point_in(const inner_type &p) {
            // (x1; x2) - {a} = (x1; a); (a; x2) if {a} in (x1, x2)
            auto x = get_interval_that_include_this_point(p);
            if (x != intervals.end()) {
                split_interval(*x, p);
                return true;
            }

            // (something + {a}) - {a} = something
            if (points.count(p)) {
                points.erase(p);
                return true;
            }
            return false;
        }

        bool add_interval_in(inner_type f, inner_type s) {
            // (a; a) == empty
            if (f == s) return false;
            if (s < f) throw std::logic_error("interval overlaps intervals");
            auto x = get_interval_that_include_this_point(f), y = get_interval_that_include_this_point(s);
            if (x == y && x != intervals.end()) return false;
            // (x1; x2); (x3; x4) + (f; s) = (x1; x2); (x3; x4) + (x1; s) if {f} in (x1; x2)
            if (x != intervals.end()) f = x->first;
            if (y != intervals.end()) s = y->second;

            // (f; x1); (x2; x3); (x4; s) + (f; s) = empty + (f; s)
            for (auto it = intervals.lower_bound({f, {}}); it != intervals.end() && it->first < s;) {
                auto old = it++;
                intervals.erase(old);
            }

            // something + (f; s) = something + (f; s) if (f; s) not in something
            intervals.emplace(f, s);

            // (f; s) + {s} + (s; x) = (f; x)
            x = intervals.find({f, s});
            y = x;
            ++y;
            if (y != intervals.end()) {
                if (y->first == s && points.count(s)) {
                    merge_intervals(*x, *y);
                    points.erase(s);
                }
            }
            // (x; f) + {f} + (f; s) = (x; s)
            x = intervals.lower_bound({f, s});
            if (x != intervals.begin()) {
                y = x;
                --y;
                if (y->second == f && points.count(f)) {
                    merge_intervals(*y, *x);
                    points.erase(f);
                }
            }

            // (f; s) + {x} = (f; s) if x in (f; s)
            for (auto it = points.upper_bound(f); it != points.end() && *it < s;) {
                auto old = it++;
                points.erase(old);
            }
            return true;
        }

        bool remove_interval_in(const inner_type &f, const inner_type &s) {
            // (a; a) == empty
            if (f == s) return false;
            if (s < f) throw std::logic_error("interval overlaps intervals");
            auto x = get_interval_that_include_this_point(f), y = get_interval_that_include_this_point(s);
            bool ret = false;

            // (x1; x2) - (f; s) = (x1; f] + [s; x2) if f, s in (x1; x2)
            if (x != intervals.end() && x == y) {
                points.insert(f);
                points.insert(s);
                intervals.emplace(x->first, f);
                intervals.emplace(s, x->second);
                intervals.erase(x);
                ret = true;
                goto point;
            }

            // (x1; x2) - (f; s) + something = (x1; f] - (f; s) + something if f in (x1; x2)
            if (x != intervals.end()) {
                intervals.emplace(x->first, f);
                points.insert(f);
                intervals.erase(x);
                ret = true;
            }
            // (x1; x2) - (f; s) + something = [s; x2) - (f; s) + something if s in (x1; x2)
            if (y != intervals.end()) {
                intervals.emplace(s, y->second);
                points.insert(s);
                intervals.erase(y);
                ret = true;
            }

            point:
            // something + (x1; x2) - (f; s) = something - (f; s) if (x1; x2) in (f; s)
            for (auto it = intervals.lower_bound({f, {}}); it != intervals.end() && it->first < s;) {
                auto old = it++;
                intervals.erase(old);
                ret = true;
            }
            // something + {x} - (f; s) = something - (f; s) if {x} in (f; s)
            for (auto it = points.upper_bound(f); it != points.end() && *it < s;) {
                auto old = it++;
                points.erase(old);
            }
            return ret;
        }

        void invert_in(interval &buf) const {
            buf.add_interval_in(minimal<T>(), maximal<T>());
            for (auto &it : intervals) {
                buf.remove_interval(it.first, it.second);
            }
            for (auto &it : points) buf.remove_point(it);
        }

        void plus_in(interval &buf, const T &val) const {
            static_assert(std::is_arithmetic<T>(), "you cannot do transfer operations with strings");
            // if point was -INF or +INF, recover will return second elem to {}
            for (auto &[fst, snd] : intervals) {
                buf.intervals.emplace(recover(std::make_pair(fst.first, fst.second + val)),
                                      recover(std::make_pair(snd.first, snd.second + val)));
            }
            for (auto &i : points) {
                buf.points.insert(recover(std::make_pair(i.first, i.second + val)));
            }
        }

        void multiply_in(interval &buf, const T &val) const {
            static_assert(std::is_arithmetic<T>(), "you cannot multiply strings");
            // if point was -INF or +INF second elem if set to 0 -> 0 * val = 0
            if (val == 0) {
                if (!intervals.empty() || !points.empty()) {
                    buf.clear();
                    buf.points.emplace(1, T{});
                }
                else buf.clear();
                return;
            }
            for (auto &[fst, snd] : intervals) {
                auto a = std::make_pair(fst.first, fst.second * val),
                     b = std::make_pair(snd.first, snd.second * val);
                // if val < 0: fst < snd -> a > b
                buf.intervals.emplace(std::min(a, b), std::max(a, b));
            }
            for (auto &i : points) {
                buf.points.insert(std::make_pair(i.first, i.second * val));
            }
        }

        void division_in(interval &buf, const T &val) const {
            static_assert(std::is_arithmetic<T>(), "you cannot divide with strings");
            // if point was -INF or +INF second elem if set to 0 -> 0 / val = 0
            for (auto &[fst, snd] : intervals) {
                auto a = std::make_pair(fst.first, fst.second / val),
                     b = std::make_pair(snd.first, snd.second / val);
                // if val < 0: fst < snd -> a > b
                buf.intervals.emplace(std::min(a, b), std::max(a, b));
            }
            for (auto &i : points) {
                buf.points.insert(std::make_pair(i.first, i.second / val));
            }
        }

        void remainder_in(interval &buf, const T &val) const {
            static_assert(std::is_signed<T>() || std::is_unsigned<T>(), "you cannot divide with strings");
            for (auto &[fst, snd] : intervals) {
                auto len = snd.second - fst.second;
                // fst.first != 1 || snd.first != 1 -> -INF or +INF -> there is interval with len > val
                if (fst.first != 1 || snd.first != 1 || len > val) {
                    // [0, val)
                    buf.add_point(T{});
                    buf.add_interval(T{}, val);
                    break;
                }
                auto r = fst.second % val + len; // l of interval is fst.second % val
                if (r > val) { // l in [0, val); l + len not in [0, val) -> (l, val) U [0, (l + len) % val)
                    buf.add_interval(fst.second % val, val);
                    buf.add_point(T{});
                    buf.add_interval(T{}, r % val);
                }
                else buf.add_interval(fst.second % val, r); // l and l + len in [0, val) -> (l, l + len)
            }
            for (auto &i : points) {
                buf.add_point(i.second % val);
            }
        }

        void custom_transfer_in(interval &buf, const std::function<T(const T&)> &fun) const {
            // if point was -INF or +INF, recover will return second elem to {}
            for (auto &[fst, snd] : intervals) {
                auto x = recover(std::make_pair(fst.first, fun(fst.second)));
                auto y = recover(std::make_pair(snd.first, fun(snd.second)));
                buf.add_interval_in(std::min(x, y), std::max(x, y));
            }
            for (auto &i : points) {
                buf.add_point_in(recover(std::make_pair(i.first, fun(i.second))));
            }
        }

        void custom_transfer_in(interval &buf, const std::function<T(const T&)> &fun, const T& a, const T& b) const {
            for (auto &[fst, snd] : intervals) {
                auto x = fst.first == 0 ? std::make_pair(1, a):std::make_pair(fst.first, fun(fst.second));
                auto y = snd.first == 2 ? std::make_pair(1, b):std::make_pair(snd.first, fun(snd.second));
                buf.add_interval_in(std::min(x, y), std::max(x, y));
            }
            for (auto &i : points) {
                buf.add_point_in(recover(std::make_pair(i.first, fun(i.second))));
            }
        }
        void custom_transfer_in(interval &buf,
                                const std::function<std::pair<inner_type, inner_type>
                                        (const inner_type&, const inner_type&)> &L_R,
                                const std::function<T(const T&)> &POINT) const {
            for (auto &[fst, snd] : intervals) {
                auto x = L_R(fst, snd);
                x.first = recover(x.first);
                x.second = recover(x.second);
                if (x.first == x.second) continue;
                if (x.second < x.first) throw std::overflow_error("right border of interval is less than left");
                buf.add_interval_in(x.first, x.second);
            }
            for (auto &i : points) {
                buf.add_point_in(recover(std::make_pair(i.first, POINT(i.second))));
            }
        }

    private:
        [[nodiscard]] std::string print_in() const {
            auto point_iter = points.begin();
            auto interval_iter = intervals.begin();
            std::string out;

            // convert (0; T-type elem) to "-INF" and (2; T-type elem) to "+INF". (1; T-type elem) prints normally
            auto data = [this](const inner_type &p)->std::string {
                switch (p.first) {
                    case 0:
                        return "-INF";
                    case 2:
                        return "+INF";
                    default:
                        return spec_to_string(p.second);
                }
            };

            // check empty data
            if (empty()) {
                out += "*Empty*\n";
                return out;
            }

            // add_key a buffer to convert {x1}; {x2} to {x1; x2} and (x1; x2) + {x1} to [x1; x2)
            std::vector<typename std::set<inner_type>::iterator> point_buffer;

            // print a buffer to "{x1; x2; x3; ...; xk}" and clear buffer
            auto print_buffer = [&data, &point_buffer, &out] {
                if (!point_buffer.empty()) {
                    out += "{" + data(*point_buffer.front());
                    for (auto i = 1; i < point_buffer.size(); ++i) {
                        out += "; " + data(*point_buffer[i]);
                    }
                    out += "} U ";
                }
                point_buffer.clear();
            };

            while (point_iter != points.end() || interval_iter != intervals.end()) {
                // if point data less than begin of next interval or intervals ends
                if (interval_iter == intervals.end() ||
                    (point_iter != points.end() && *point_iter <= interval_iter->first)) {
                    // add point to buffer
                    point_buffer.push_back(point_iter);
                    ++point_iter;
                }
                else {
                    // b1 - begin from '['; b2 - end with ']'
                    bool b1 = false, b2 = false;

                    // (x1; x2) + {x1} = [x1; x2)
                    if (!point_buffer.empty()) {
                        if (*point_buffer.back() == interval_iter->first) {
                            point_buffer.pop_back();
                            b1 = true;
                        }
                    }

                    print_buffer();

                    // if next point = x2; (x1; x2) + {x2} = (x1; x2]
                    if (point_iter != points.end() && *point_iter == interval_iter->second) {
                        ++point_iter;
                        b2 = true;
                    }
                    out += (b1 ? '[':'(') + data(interval_iter->first) + "; " +
                           data(interval_iter->second) + (b2 ? ']':')') + " U ";
                    ++interval_iter;
                }
            }

            print_buffer();
            out.pop_back();
            out.pop_back();
            out.pop_back();
            return out;
        }


        template <typename T2>
        [[nodiscard]] static std::string spec_to_string(const T2 &a) {
            std::stringstream ss; // emulate standard output
            ss << a;
            return ss.str();
        }
        [[nodiscard]] static std::string spec_to_string(const std::string &a) {
            return "\"" + a + "\""; // for strings
        }

        [[nodiscard]] typename std::set<std::pair<inner_type, inner_type>>::iterator
        get_interval_that_include_this_point(const inner_type &point) const {
            auto x = intervals.upper_bound({point, maximal<T>()}); // get interval (>point; x2)
            if (x != intervals.begin()) {
                --x; // interval (x1; <=point)
                if (x->second > point && x->first < point) return x;
            }
            return intervals.end();
        }

        // (x1; x2) + (x2; x3) + {x2} = (x1; x3)
        void merge_intervals(const std::pair<inner_type, inner_type> &f, const std::pair<inner_type, inner_type> &s) {
            intervals.emplace(f.first, s.second);
            intervals.erase(f);
            intervals.erase(s);
        }

        // (x1; x3) - {x2} = (x1; x2) + (x2; x3) if {x2} in (x1; x3)
        void split_interval(const std::pair<inner_type, inner_type> &f, const inner_type &s) {
            intervals.emplace(f.first, s);
            intervals.emplace(s, f.second);
            intervals.erase(f);
        }

        template <typename T2>
        [[nodiscard]] std::optional<T2> get_any_for_number // for degrees
                (const std::set<std::pair<std::pair<int, T2>, std::pair<int, T2>>> &a) const {
            for (auto &i : a) {
                if (i.first.first == 0 && i.second.first == 2) return {}; // (-INF; +INF) -> 0
                // i.second.second - 1 < i.second.second and i.first.second + 1 > i.first.second
                // are need for detect overflow
                if (i.first.first == 0 && i.second.first == 1 && i.second.second - 1 < i.second.second)
                    return i.second.second - 1; // (-INF; x) -> x - 1
                if (i.first.first == 1 && i.second.first == 2 && i.first.second + 1 > i.first.second)
                    return i.first.second + 1; // (x; +INF) -> x + 1
                if (i.first.first == 1 && i.second.first == 1) { // (x1; x2) -> (x1 + x2) / 2
                    auto x = (i.second.second - i.first.second) / 2 + i.first.second;
                    if (x > i.first.second && x < i.second.second) return x;
                }
            }
            return std::nullopt;
        }

        [[nodiscard]] static std::optional<std::string> get_any_for_not_number // for string
                (const std::set<std::pair<std::pair<int, std::string>, std::pair<int, std::string>>> &a) {
            for (const auto &[fst, snd] : a) {
                if (fst.first == 0 && snd.first == 2) return "aboba"; // (-INF; +INF) -> "aboba"
                if (fst.first == 0 && snd.first == 1 && snd.second > "a") return "a";
                // (-INF; x) -> "a"
                if (fst.first == 1 && snd.first == 2) return 'z' + fst.second;
                // (x; +INF) -> "z" + x (> x)
                if (fst.first == 1 && snd.first == 1) { // (x1; x2) -> x1 + "a"
                    auto x = fst.second;
                    x += 'a';
                    if (x > fst.second && x < snd.second) return x;
                }
            }
            return std::nullopt;
        }

        template <typename T2>
        [[nodiscard]] static std::optional<T2> get_any_for_not_number // non-number types
        (const std::set<std::pair<std::pair<int, T2>, std::pair<int, T2>>> &a) {
            if (a.size() == 1 && a.begin()->first.first == 0 && a.begin()->second.first == 2) return T2{};
            return std::nullopt; // for unknown type I don`t know what I need to do
        }

        [[nodiscard]] static std::optional<T> get_any_functional
                (const std::set<std::pair<std::pair<int, T>, std::pair<int, T>>> &a,
                const std::function<std::optional<T>(const T&)> &MINUS_INF_x,
                const std::function<std::optional<T>(const T&)> &x_PLUS_INF,
                const std::function<std::optional<T>(const T&, const T&)> &x_y) {
            for (auto &i : a) {
                if (i.first.first == 0 && i.second.first == 2) return {}; // (-INF; +INF) -> 0
                if (i.first.first == 0 && i.second.first == 1)
                    if (auto x = MINUS_INF_x(i.second.second); x.has_value()) return x; // (-INF; x)
                if (i.first.first == 1 && i.second.first == 2)
                    if (auto x = x_PLUS_INF(i.first.second); x.has_value()) return x; // (x; +INF)
                if (i.first.first == 1 && i.second.first == 1) { // (x1; x2)
                    if (auto x = x_y(i.first.second, i.second.second); x.has_value()) return x; // (x1; x2)
                }
            }
            return std::nullopt;
        }
    };

} // interval

#endif //interval_H
