//
// Created by xm20 on 17.03.2025.
//

#ifndef CODETEST_ASTEXPERIMENT_H
#define CODETEST_ASTEXPERIMENT_H

#include <utility>
#include <variant>
#include <string>
#include <utility>
#include <iostream>

[[maybe_unused]] void get_v(const int &other) { std::cout << other << '\n'; }
[[maybe_unused]] void get_v(const double &other) { std::cout << other << '\n'; }
[[maybe_unused]] void get_v(const std::string &other) { std::cout << other << '\n'; }

struct variable {

protected:
    std::variant<int, double, std::string> value;

    [[nodiscard]] std::variant<int, double, std::string> get_value() const  {
        auto *pint = std::get_if<int>(&value);
        if (pint != nullptr) return *pint;

        auto *pdouble = std::get_if<double>(&value);
        if (pdouble != nullptr) return *pdouble;

        auto *pstring = std::get_if<std::string>(&value);
        if (pstring != nullptr) return *pstring;

        throw std::runtime_error("Class: variable, function: get_value, error: no value is saved while getting it!");
    }

    [[nodiscard]] short get_type() const {
        auto *pint = std::get_if<int>(&value);
        if (pint != nullptr) return 1;

        auto *pdouble = std::get_if<double>(&value);
        if (pdouble != nullptr) return 2;

        auto *pstring = std::get_if<std::string>(&value);
        if (pstring != nullptr) return 3;

        throw std::runtime_error("Class: variable, function: get_value, error: no value is saved while getting it!");
    }

public:

    variable() = default;
    ~variable() = default;

    [[maybe_unused]] explicit variable(std::variant<int, double, std::string> *val) { this->value = *val; }
    [[maybe_unused]] explicit variable(std::variant<int, double, std::string> &val) { this->value = val; }
    [[maybe_unused]] explicit variable(std::variant<int, double, std::string> val) { this->value = std::move(val); }

    [[maybe_unused]] variable(const variable &val) { this->value = val.value; }
    [[maybe_unused]] explicit variable(variable *val) { this->value = std::move(val->value); }

    inline void get_val() { std::visit([] (auto&& arg) { return get_v(arg); }, this->value); }

    variable& operator=(const variable &var) {
        this->value = var.value;
        return *this;
    }

    variable& operator=(const variable *var) {
        this->value = var->value;
        return *this;
    }

    variable& operator=(std::variant<int, double, std::string> &val) {
        this->value = val;
        return *this;
    }

    variable& operator=(std::variant<int, double, std::string> *val) {
        this->value = *val;
        return *this;
    }

    inline bool operator==(const variable *var) const { return this->get_value() == var->get_value(); }

    inline bool operator!=(const variable *var) const { return this->get_value() != var->get_value(); }

    inline bool operator>(const variable *var) const { return this->get_value() > var->get_value(); }

    inline bool operator<(const variable *var) const { return this->get_value() < var->get_value(); }

    inline bool operator>=(const variable *var) const { return this->operator>(var) || this->operator==(var); }

    variable operator+(variable other) {
        if (this->get_type() != other.get_type())
            throw std::runtime_error("Function operator+ : different types not allowed!");

        auto *pint1 = std::get_if<int>(&value);
        auto *pint2 = std::get_if<int>(&other.value);
        if (pint1 != nullptr && pint2 != nullptr) {
            this->value = *pint1 + *pint2;
            return *this;
        }

        auto *pdouble1 = std::get_if<double>(&value);
        auto *pdouble2 = std::get_if<double>(&other.value);
        if (pdouble1 != nullptr && pdouble2 != nullptr) {
            this->value = *pdouble1 + *pdouble2;
            return *this;
        }

        auto *pstring1 = std::get_if<std::string>(&value);
        auto *pstring2 = std::get_if<std::string>(&other.value);
        if (pstring2 != nullptr && pstring2 != nullptr) {
            this->value = std::string(*pstring1 + *pstring2);
            return *this;
        }

        throw std::runtime_error("Class: variable, function: get_value, error: no value is saved while getting it!");
    }
};

struct BinaryOperation {
    variable left, right;
    char operation;

    [[maybe_unused]] BinaryOperation(variable *left, variable *right, char operation)
        : left(*left), right(*right), operation(operation) {}

    [[maybe_unused]] BinaryOperation(std::variant<int, double, std::string> *left, std::variant<int, double, std::string> *right, char operation)
            : left(left), right(right), operation(operation) {}

    void get_value() {
        switch (operation) {
            case '+':
                (left + right).get_val();
                break;
            default:
                break; ///todo : add types operators: +, -, +=, -=, *, /, *=, /=
        }
    }
};

#endif //CODETEST_ASTEXPERIMENT_H
