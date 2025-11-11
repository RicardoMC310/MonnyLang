#pragma once
#include <string>

class SystemError : public std::exception
{
private:
    std::string message;

public:
    SystemError(const std::string &message): message(std::move(message)) {}

    const char* what() const noexcept override { return message.c_str(); }
};