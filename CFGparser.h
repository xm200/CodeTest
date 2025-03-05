//
// Created by xm200 on 05.03.2025.
//
#pragma once
#include <fstream>

//#ifndef CODETEST_CFGPARSER_H
//#define CODETEST_CFGPARSER_H
namespace parse {

    enum compilers {
        GNU,
        MSC,
        CLANG
    };

    short get_c() {
        #ifdef __GNUC__
            return GNU;
        #endif
        #ifdef __clang__
            return CLANG;
        #endif
        #ifdef _MSC_VER
            return MSC;
        #endif
    }

    std::string compiler_command(std::string &path) {
        std::string command;
        std::string buf, out;
        if (get_c() == GNU) command = "g++ -E " + path + " -o main.o";
        else if (get_c() == CLANG) command = "clang -E " + path + " -o main.o";
        else {
            command = "cl /P " + path;
            path = path.substr(0, path.length() - 2);
        }
        system(command.c_str());
        std::ifstream file((get_c() == MSC) ? path + ".i" : "main.o");
        while (file >> buf) out += buf + ' ';
        return out;
    }

    class Parser {

    };
}
//#endif //CODETEST_CFGPARSER_H
