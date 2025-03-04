/// made with =uwu=
/*
                                                           :+=--
                                                         .*+   -#
                                                        ++.     +=
        -*+=-.                                        -#.        #.
       %-   :=*+-                                   :#-          :*
      *#       .=**-      .::::::.                 +*             +-
     .@-           -**-  *#:.....::----==-===-:  =*:               #.
     =@.              -*+==++.               .:=#+                 :#
     *%                  -*#++*.                                    =+
     #*                     . .+*:                                   #:
     #+                       -=+#-                                   #:
     ++                    =*+:                                       .#
     -%                  =*:     .--=.                                 *:
      %:                *=:-====-:.                       ........     :=
      =+                :-:                          :====-:-=:::-      -#-
      .@                                          :*%@%+:    =*.        -=%----==
       *=                                       -#@@@@@@@%:   :#     :=+#+:..  -#
       :%                                    .=*=%@@@@@@@@@.   ==             =*
        ++     :==-::.                      :=. =@@@@@@@@@@+    %           :*=
        .@.      #..::::%@@@%%@@@#              =@@@@@@@@@@*    %        .-*=
         #:     .*      %@@@@@@@@@              -@@@@@@@@@@:   :*       :*#=:
    -====----.  =-      %@@@@@@@@@               =@@@@@@@#:    *:           -*+
   .%           +-      -@@@@@@@@#                .=+++=.       .- --         =-
    .++-.       :*       =@@@@@@*      +*++.                  ===-=:     .:-=++
       :=++=     #:       .+@#*:                                    . -+=::.
           #     ..                                              .=#=
          ++    -*-+#::=:               -==-                  :+*+:      .-
         *=     .:.  ..              -*+:  :+=            :=*+-.         -#*
        .%.                          .             .:-++++-.              *:*.
         .=+=---==: --:.       ......          .===-:.. .--===.           # :#.
                      .:--------+%###.                :#:    .=*-         #  :*
                              .%#:                      *=      -#.       #   *
                             -* -*-                      ++       #:     *-  :#
                            +*    -++                     *-      .*    +* -##+=-*.
                           *=    :++=                      #.     =+  -*-        *:
                          :+    #=                         .#    +*.+*-        -#-
                          .#.   *-===:                      -*.*%-          .=*-
                           .++=:..  -=                       ++ .-===+    .==.
                               :=%: *.                        *-     :%
                               -*.  #                          #:     ++
                              :*   :+                          .%..==+=.
                              *:   +-                           :=
                              .--  %
                                  ==        Ð§Ñ ÑÐ¼Ð¾ÑÑÐ¸ÑÑ
*/
#ifdef LOCAL
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma ide diagnostic ignored "cert-dcl58-cpp"
// ReSharper disable CppDFAConstantFunctionResult
// ReSharper disable CppRedundantInlineSpecifier
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppUseStructuredBinding

#else
// #define INT
#ifdef INT
#define uwu int
#define UWU_MAX INT_MAX
#define UWU_MIN INT_MIN
#else
#define uwu long long
#define UWU_MAX LONG_LONG_MAX
#define UWU_MIN LONG_LONG_MIN
#endif
#define owo unsigned uwu
//#define _GLIBCXX_DEBUG
//#define _GLIBCXX_DEBUG_PEDANTIC
//#pragma GCC optimize("O2")
//#pragma GCC optimize("O3")
//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx")
//#pragma GCC target("avx2")

#endif

#include "interval.h"
#include <bits/stdc++.h>
//std::mt19937 rd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
//    std::uniform_int_distribution<int> ed(5, 10);
//int rnd() {
//    return ed(rd);
//}

namespace std {
#define tae(...) __VA_ARGS__, __VA_ARGS__
    template <typename T1, typename T2>
    std::istream inline &operator >>(std::istream &_a, std::pair<T1, T2> &_b) {
        return _a >> _b.first >> _b.second;
    }
    template <typename T1, typename T2>
    std::ostream inline &operator <<(std::ostream &a_, const std::pair<T1, T2> &b_) {
        return a_ << b_.first << ' ' << b_.second;
    }
    template <typename T1, typename T2>
    inline std::pair<T1, T2> &operator --(std::pair<T1, T2> &a) {
        --a.first, --a.second;
        return a;
    }
    template <typename T1, typename T2>
    inline std::pair<T1, T2> &operator ++(std::pair<T1, T2> &a) {
        ++a.first, ++a.second;
        return a;
    }
    template<typename T1, typename T2 = T1, typename T3 = T2>
    class tpair {
    public:
        T1 first = {};
        T2 second = {};
        T3 third = {};
        tpair() = default;
        [[maybe_unused]] tpair(T1 f_, T2 s_, T3 t_) : first(f_), second(s_), third(t_) {}
        constexpr friend bool inline operator ==(const tpair &a, const tpair &b) {
            return a.first == b.first && a.second == b.second && a.third == b.third;
        }
        constexpr friend bool inline operator !=(const tpair &a, const tpair &b) {
            return !(a == b);
        }
        constexpr friend bool operator <(const tpair &a, const tpair &b) {
            if (a.first == b.first) {
                if (a.second == b.second) return a.third < b.third;
                return a.second < b.second;
            }
            return a.first < b.first;
        }
        constexpr friend inline bool operator <=(const tpair &a, const tpair &b) {
            if (a == b) return true;
            return a < b;
        }
        constexpr friend inline bool operator >(const tpair &a, const tpair &b) {
            return !(a <= b);
        }
        constexpr friend inline bool operator >=(const tpair &a, const tpair &b) {
            return !(a < b);
        }
        friend inline std::istream & operator >>(std::istream &a_, tpair &b_) {
            return a_ >> b_.first >> b_.second >> b_.third;
        }
        friend inline std::ostream & operator <<(std::ostream &a_, const tpair &b_) {
            return a_ << b_.first << ' ' << b_.second << ' ' << b_.third;
        }
        inline tpair& operator --() {
            --first, --second, --third;
            return *this;
        }
        inline tpair& operator ++() {
            ++first, ++second, ++third;
            return *this;
        }
        [[maybe_unused]] inline tpair& zero() {
            --first;
            --second;
            return *this;
        }

    };
    template<typename T1, typename T2 = T1, typename T3 = T2>
    [[nodiscard]][[maybe_unused]]tpair<T1, T2, T3> make_tpair(T1 _a, T2 _b, T3 _c) {
        return {_a, _b, _c};
    }
    template <typename T>
    [[maybe_unused]] inline bool maxof(T &a, const T b) {
        return a < b && (a = b, true);
    }
    template <typename T>
    [[maybe_unused]] inline bool minof(T &a, const T b) {
        return a > b && (a = b, true);
    }
    namespace fbp {
        template<typename type>
        [[maybe_unused]] [[nodiscard]] inline bool dereferenced_sort_comparator(const type *_a, const type *_b) {
            return *_a < *_b;
        }
        template <unsigned char _a, unsigned char _b, unsigned char _c, class T1, class T2 = T1, class T3 = T2>
        [[maybe_unused]] [[nodiscard]] bool tpair_spec_priority_comparator(
                const std::tpair<T1, T2, T3> &p1,
                const std::tpair<T1, T2, T3> &p2) {
            if ((_a == 1 && p1.first  == p2.first ) ||
                (_b == 1 && p1.second == p2.second) ||
                (_c == 1 && p1.third  == p2.third )) {
                if ((_a == 2 && p1.first  == p2.first ) ||
                    (_b == 2 && p1.second == p2.second) ||
                    (_c == 2 && p1.third  == p2.third )) {
                    return _a == 3 ? p1.first  < p2.first :
                           _b == 3 ? p1.second < p2.second:
                           p1.third  < p2.third;
                }
                return _a == 2 ? p1.first  < p2.first :
                       _b == 2 ? p1.second < p2.second:
                       p1.third  < p2.third;
            }
            return _a == 1 ? p1.first  < p2.first :
                   _b == 1 ? p1.second < p2.second:
                   p1.third  < p2.third;
        }
        template<class T1, class T2>
        [[maybe_unused]] [[nodiscard]] bool pair_second_priority_comparator(
                const std::pair<T1, T2> &_a,
                const std::pair<T1, T2> &_b) {
            if (_a.second == _b.second) return _a.first < _b.first;
            return _a.second < _b.second;
        }
    }
}// namespace std
int interactive_docs() {
#ifdef LOCAL
    {[[maybe_unused]] std::pair<tae(int)> _;}// unused_define hack: it does not do anything
#else
    //    freopen("repair.in" , "r", stdin );
    //    freopen("repair.out", "w", stdout);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    using tt = uwu;
    std::vector<interval::interval<tt>> vec(1);
    auto print = [&vec] {
        for (auto i = 0; i < vec.size(); ++i) {
            std::cout << i + 1 << ": ";
            std::cout << vec[i].print();
        }
    };
    auto index_not_exist = [&vec](uwu a, uwu b) {
        if (a >= vec.size() || b >= vec.size()) {
            std::cout << "this index don`t exists\n";
            return true;
        }
        return false;
    };
    std::cout << "for get help: print \"help\"\n";
    print();
    std::cout << "now you work with index 1\n";
    std::cout << "> " << std::flush;
    std::string cmd;
    uwu i = 0;
    while (std::cin >> cmd) {
        if (vec.empty()) vec.emplace_back();
        if (cmd == "help") {
            std::cout << "as x: work with interval with x index\n";
            std::cout << "pb: add empty interval\n";
            std::cout << "rb: remove last interval\n";
            std::cout << "ra: remove all intervals\n";
            std::cout << "pa: print all intervals\n";
            std::cout << "gi: print index with whom I work\n";
            std::cout << "cp: add interval that is equal this\n";
            std::cout << "eo x y: print yes if x-th interval == y-th interval\n";
            std::cout << "\n";

            std::cout << "work with current interval:\n";
            std::cout << "ap x: add point x to current interval\n";
            std::cout << "rp x: remove point x from current interval\n";
            std::cout << "ai x y: add interval (x; y) to current interval\n";
            std::cout << "ri x y: remove interval (x; y) from current interval\n";
            std::cout << "rv: change this interval to (-INF; +INF) / this interval (math style)\n";
            std::cout << "ga: get any element in this interval\n";
            std::cout << "\n";
            std::cout << "functions ai and ri can add -INF and +INF using symbols l and r in the end of command\n";
            std::cout << "for example\n";
            std::cout << "ail x: add interval (-INF; x)\n";
            std::cout << "air x: add interval (x; +INF)\n";
            std::cout << "ailr: add interval (-INF; +INF)\n";
            std::cout << "\n";

            std::cout << "math operations\n";
            std::cout << "pn x y: add interval with value x-th interval | y-th interval\n";
            std::cout << "mn x y: add interval with value x-th interval - y-th interval\n";
            std::cout << "an x y: add interval with value x-th interval & y-th interval\n";
            std::cout << "pf x y: x-th interval |= y-th interval\n";
            std::cout << "mf x y: x-th interval -= y-th interval\n";
            std::cout << "af x y: x-th interval &= y-th interval\n";
            std::cout << "\n";

            std::cout << "transfer operations\n";
            std::cout << "mr x: transfer this multitude on x right\n";
            std::cout << "ml x: transfer this multitude on x left\n";
            std::cout << "mp x: multiply every elem in this multitude on x\n";
            std::cout << "md x: divide every elem in this multitude on x\n";
            std::cout << "ms x: rewrite multitude to remain of divide on x of every elem in this multitude\n";
            std::cout << "\n";

            std::cout << "print \"exit\" for quit\n";
            std::cout << "\n";

            cmd = "pa";
            goto point;
        }
        if (cmd == "gi") {
            std::cout << "now you work with index " << i + 1 << '\n';
        }
        if (cmd == "as") {
            uwu a;
            std::cin >> a;
            --a;
            if (a >= vec.size()) {
                std::cout << "this index don`t exists\n";
                goto point;
            }
            i = a;
        }
        if (cmd == "cp") {
            vec.push_back(vec[i]);
            i = static_cast<uwu>(vec.size() - 1);
        }
        if (cmd == "pb") {
            i = static_cast<uwu>(vec.size());
            vec.emplace_back();
        }
        if (cmd == "rb") {
            vec.pop_back();
            if (i != 0 && i >= vec.size()) {
                std::cout << "now you work with index 1, because " << i + 1 << " index don`t exists\n";
                i = 0;
            }
            if (vec.empty()) vec.emplace_back();
            goto point;
        }
        if (cmd == "ra") {
            vec.clear();
            i = 0;
            vec.emplace_back();
            goto point;
        }
        if (cmd == "ap") {
            tt a;
            std::cin >> a;
            vec[i].add_point(a);
        }
        if (cmd == "rp") {
            tt a;
            std::cin >> a;
            vec[i].remove_point(a);
        }
        if (cmd.substr(0, 2) == "ai") {
            if (cmd.size() == 2) {
                tt a, b;
                std::cin >> a >> b;
                vec[i].add_interval(a, b);
            }
            if (cmd.size() == 3) {
                tt buf;
                std::cin >> buf;
                if (cmd.back() == 'l') vec[i].add_interval(interval::minimal<tt>(), buf);
                if (cmd.back() == 'r') vec[i].add_interval(buf, interval::maximal<tt>());
            }
            if (cmd.size() == 4 && cmd.substr(2) == "lr")
                vec[i].add_interval(interval::minimal<tt>(), interval::maximal<tt>());
        }
        if (cmd.substr(0, 2) == "ri") {
            if (cmd.size() == 2) {
                tt a, b;
                std::cin >> a >> b;
                vec[i].remove_interval(a, b);
            }
            if (cmd.size() == 3) {
                tt buf;
                std::cin >> buf;
                if (cmd.back() == 'l') vec[i].remove_interval(interval::minimal<tt>(), buf);
                if (cmd.back() == 'r') vec[i].remove_interval(buf, interval::maximal<tt>());
            }
            if (cmd.size() == 4 && cmd.substr(2) == "lr")
                vec[i].remove_interval(interval::minimal<tt>(), interval::maximal<tt>());
        }
        if (cmd == "rv") {
            vec[i] = vec[i].invert();
        }
        if (cmd == "ga") {
            if (auto x = vec[i].any(); x.has_value()) std::cout << x.value() << '\n';
            else std::cout << "*none*\n";
        }
        if (cmd == "pn") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            i = static_cast<uwu>(vec.size());
            vec.push_back(vec[a] + vec[b]);
        }
        if (cmd == "mn") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            i = static_cast<uwu>(vec.size());
            vec.push_back(vec[a] - vec[b]);
        }
        if (cmd == "an") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            i = static_cast<uwu>(vec.size());
            vec.push_back(vec[a] * vec[b]);
        }
        if (cmd == "pf") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            vec[a] += vec[b];
            i = a;
        }
        if (cmd == "mf") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            vec[a] -= vec[b];
            i = a;
        }
        if (cmd == "af") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            vec[a] *= vec[b];
            i = a;
        }
        if (cmd == "eo") {
            uwu a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            std::cout << (vec[a] == vec[b] ? "yes\n" : "no\n");
        }
        if (cmd == "mr") {
            tt a;
            std::cin >> a;
            vec[i] += a;
        }
        if (cmd == "ml") {
            tt a;
            std::cin >> a;
            vec[i] -= a;
        }
        if (cmd == "mp") {
            tt a;
            std::cin >> a;
            vec[i] *= a;
        }
        if (cmd == "md") {
            tt a;
            std::cin >> a;
            vec[i] /= a;
        }
        if (cmd == "ms") {
            tt a;
            std::cin >> a;
            vec[i] %= a;
        }
        if (cmd == "aboba") {
            vec.push_back(vec[i]._custom_transfer(
                    [](const std::pair<int, tt> &a, const std::pair<int, tt> &b) {
                        return std::make_pair(a, b);
                        },
                    [](const tt &a) {return a + 1;}
                    ));
            i = static_cast<uwu>(vec.size()) - 1;
        }

        if (cmd == "exit") break;

        point:
        if (cmd == "pa") {
            print();
        }
        else std::cout << vec[i].print() << '\n';
        std::cout << "> " << std::flush;
    }
    return 0;
}