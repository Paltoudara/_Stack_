#pragma once

#include<exception>
#include<string>
#define _NODISCARD [[nodiscard]]

#define _CONSTEXPR20 constexpr
#define _PANAGIOTIS_BEGIN namespace panagiotis{
#define _PANAGIOTIS_END }
class bad_stack_access_ : public std::exception {
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit bad_stack_access_(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};
class pop_from_empty_stack_ :public std::exception {
private:
    std::string errorMessage; // To store the error message
public:
    // Constructor to initialize the error message
    explicit pop_from_empty_stack_(const std::string& message)
        : errorMessage(message) {
    }

    //Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};


