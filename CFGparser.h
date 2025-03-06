//
// Created by xm200 on 05.03.2025.
//
//#pragma once
#include <fstream>

#ifndef CODETEST_CFGPARSER_H
#define CODETEST_CFGPARSER_H
namespace parse {

    std::string file_name, file_type;

    // ANOTHER - operators, types, functions calls and all that can be in the code
    enum search {
        IF, ELIF, ELSE, FOR, WHILE, ANOTHER
    };

    int get_construction_type(const std::string &buf) {
        if (buf == "if") return IF;
        if (buf == "elif") return ELIF;
        if (buf == "else") return ELSE;
        if (buf == "for") return FOR;
        if (buf == "while") return WHILE;
        else return ANOTHER;
    }

    enum os {
        LINUX, MACOS, WIN
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
            if (inited) {
                if (way + '/' == path || way == path) return;
#if defined(DEBUG_MODE)
                throw std::logic_error("cache with multiple initialization and different paths");
#endif
            }

            if (way.empty()) path = way;
            else path = way + '/';
            path += "cache/";
            system(("mkdir " + path).c_str());
            inited = true;
        }

        ~cache_t() {
#if !defined(DO_NOT_REMOVE_CACHE)
            if (!inited) return;
            if constexpr (get_os() == WIN) system("rd /s /q cache"); // Remove dir
            if constexpr (get_os() == LINUX || get_os() == MACOS) system("rm -rf cache");
#endif
        }
        const std::string &operator()() const {
#if defined(DEBUG_MODE)
            if (!inited) throw std::logic_error("cache dir don`t exists");
#endif
            return path;
        }

    private:
        std::string path;
        bool inited = false;
    } cache;

    std::vector<std::string> compiler_command(const std::string &path) {
        std::size_t ind = 0, point = path.size();

        for (auto i = path.size(); i --> 0;) {
            if (point == path.size() && path[i] == '.') point = i;
            if (path[i] == '/' || path[i] == '\\') {
                ind = i;
                break;
            }
        }

        cache.init(path.substr(0, ind));
        std::string buf;
        std::vector<std::string> out;
        file_name = path.substr(ind + 1, point - ind - 1), file_type = path.substr(point);
        std::ifstream file(path);
        while (std::getline(file, buf)) out.push_back(buf);
        file.close();

#if defined(VERBOSE)
        std::cout << "path to cache: " << cache() << '\n';
        std::cout << "file name: " << file_name << '\n';
        std::cout << "file type: " << (file_type.empty() ? "*none*":file_type) << '\n';
        std::cout << "code have " << out.size() << " lines\n";
#endif

        return out;
    }

    class Parser {
        [[nodiscard]] static size_t get_spaces(const std::string &buf) {
            size_t spaces = 0;
            for (auto &s : buf) if (s == ' ') ++spaces; else break;
            return spaces;
        }

        static size_t get_code_block(const std::vector<std::string> &code, size_t l = 0) {
#if defined(DEBUG_MODE)
            if (l >= code.size()) throw
                    std::length_error("Unreachable start limit in function get_code_block()");
#endif
            size_t block_spaces = get_spaces(code[l]);
            for (size_t i = l; i < code.size(); ++i)
                if (get_spaces(code[i]) < block_spaces) return i - l;
            return code.size() - l;
        }


    };
}
#endif //CODETEST_CFGPARSER_H
