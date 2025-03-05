#include <iostream>
#include "interval.h"
#include <any>

struct Node {
    std::vector<interval::interval<std::any>> variables;
};

// ANOTHER - operators, types, functions calls and all that can be in the code
enum search {
    IF, ELSE, FOR, WHILE, ANOTHER
};

int get_construction_type(const std::string &buf) {
    if (buf == "if") return IF;
    if (buf == "else") return ELSE;
    if (buf == "for") return FOR;
    if (buf == "while") return WHILE;
    else return ANOTHER;
}

void parse(const std::string &expr) {
    std::string result;
    for (auto &i : expr) {
        if (i != ' ') {
            result += i;
        } else {
            get_construction_type(result);
        }
    }
}
