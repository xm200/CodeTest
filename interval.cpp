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
#include "libraries/interval.h"
#include <iostream>
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    using tt = int64_t; // type of value interval
    std::vector<interval::interval<tt>> vec(1);
    auto print = [&vec] {
        for (auto i = 0; i < vec.size(); ++i) {
            std::cout << i + 1 << ": ";
            std::cout << vec[i].print();
        }
    };
    auto index_not_exist = [&vec](const int64_t a, const int64_t b) {
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
    int64_t i = 0;
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
            int64_t a;
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
            i = static_cast<int64_t>(vec.size() - 1);
        }
        if (cmd == "pb") {
            i = static_cast<int64_t>(vec.size());
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
            int64_t a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            i = static_cast<int64_t>(vec.size());
            vec.push_back(vec[a] + vec[b]);
        }
        if (cmd == "mn") {
            int64_t a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            i = static_cast<int64_t>(vec.size());
            vec.push_back(vec[a] - vec[b]);
        }
        if (cmd == "an") {
            int64_t a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            i = static_cast<int64_t>(vec.size());
            vec.push_back(vec[a] * vec[b]);
        }
        if (cmd == "pf") {
            int64_t a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            vec[a] += vec[b];
            i = a;
        }
        if (cmd == "mf") {
            int64_t a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            vec[a] -= vec[b];
            i = a;
        }
        if (cmd == "af") {
            int64_t a, b;
            std::cin >> a >> b;
            --a, --b;
            if (index_not_exist(a, b)) goto point;
            vec[a] *= vec[b];
            i = a;
        }
        if (cmd == "eo") {
            int64_t a, b;
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