//
// Created by xm20 on 17.03.2025.
//

#ifndef CODETEST_ASTEXPERIMENT_H
#define CODETEST_ASTEXPERIMENT_H

#include <variant>
#include <string>
#include <utility>
#include <iostream>

struct AstNode {
    AstNode() = default;
    ~AstNode() = default;
    virtual std::variant<int, double, std::string> get_value() const = 0;
};

struct variable final : AstNode {
protected:
    std::variant<int, double, std::string> value;

    struct TypesBehavior {
        int operator()(const int &other) { return other; }
        double operator()(const double &other) { return other; }
        std::string operator()(const std::string &other) { return other; }
    };

public:
    variable(std::variant<int, double, std::string> *val) { this->value = *val; }
    variable(std::variant<int, double, std::string> val) { this->value = val; }

    std::variant<int, double, std::string> get_value() const override {
        auto *pint = std::get_if<int>(&value);
        if (pint != nullptr) return *pint;

        auto *pdouble = std::get_if<double>(&value);
        if (pdouble != nullptr) return *pdouble;

        auto *pstring = std::get_if<std::string>(&value);
        if (pstring != nullptr) return *pstring;

        throw std::runtime_error("Class: variable, function: get_value, error: no value is saved while getting it!");
    }

    variable operator=(const variable &var) {
        this->value = var.value;
        return *this;
    }

    variable operator=(std::variant<int, double, std::string> &val) {
        this->value = val;
        return *this;
    }

    inline bool operator==(const variable *var) { return this->get_value() == var->get_value(); }

    inline bool operator!=(const variable *var) { return this->get_value() != var->get_value(); }

    inline bool operator>(const variable *var) { return this->get_value() > var->get_value(); }

    inline bool operator<(const variable *var) { return this->get_value() < var->get_value(); }

    inline bool operator>=(const variable *var) { return this->operator>(var) || this->operator==(var); }
};

struct BinaryOperation : AstNode {
    variable *left, *right;
    char operation;

    BinaryOperation(variable *left, variable *right, char operation)
        : left(left), right(right), operation(operation) {}

    std::variant<int, double, std::string> get_value() const override {
        switch (operation) {
            default:
                break; ///todo : add types operators: +, -, +=, -=, *, /, *=, /=
        }
    }
};

#endif //CODETEST_ASTEXPERIMENT_H
