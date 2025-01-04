#pragma once
#include<exception>
#include<string>
#if _HAS_CXX17
#define _NODISCARD [[nodiscard]]
#endif
#define _CONSTEXPR20 constexpr
#define _PANAGIOTIS_BEGIN namespace panagiotis{
#define _PANAGIOTIS_END }
class bad_stack_access : public std::exception {
private:
    std::string errorMessage; // To store the error message
public:
     //Constructor to initialize the error message
    explicit bad_stack_access(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};
class pop_from_empty_stack :public std::exception {
private:
    std::string errorMessage; // To store the error message
public:
    // Constructor to initialize the error message
    explicit pop_from_empty_stack(const std::string& message)
        : errorMessage(message) {
    }

     //Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};
