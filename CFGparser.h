//
// Created by xm200 on 05.03.2025.
//
#pragma once
#include <fstream>

//#ifndef CODETEST_CFGPARSER_H
//#define CODETEST_CFGPARSER_H
namespace parse {

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

    enum compilers {
        GNU, MSC, CLANG
    };

    enum os {
        LINUX, MACOS, WIN
    };

    short get_c() {
        #if defined(__GNUC__)
            return GNU;
        #elif defined(__clang__)
            return CLANG;
        #elif defined(_MSC_VER)
            return MSC;
        #endif
    }

    constexpr short get_os() {
        #if defined(__linux__)
            return LINUX;
        #elif defined(_WIN32) || defined(_WIN64)
            return WIN;
        #elif defined(__APPLE__) || defined(__MACH__)
            return MACOS;
        #endif
    }

    void init() {
        system("mkdir cache"); // Make dir
    }

    void clear() {
        if constexpr (get_os() == WIN) system("rd /s /q cache"); // Remove dir
        if constexpr (get_os() == LINUX || get_os() == MACOS) system("rm -rf cache");
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
