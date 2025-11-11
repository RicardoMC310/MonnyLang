#pragma once

#include <exception>
#include <string>

class SyntaxError : public std::exception
{
private:
    std::string message;

public:
    SyntaxError(const std::string &message) : message(std::move(message)) {}

    const char *what() const noexcept override { return message.c_str(); }
};