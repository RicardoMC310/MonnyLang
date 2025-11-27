#pragma once

#include <iostream>
#include <utils/termcolor.hpp>

class Debug
{
private:
public:
    template <class... Args>
    static void error(Args... args)
    {
        std::cout << "[" << termcolor::bright_red << termcolor::bold << "ERROR" << termcolor::reset << "]: ";
        ((std::cout << args << ' '), ...) << std::endl;
    }

    template <class... Args>
    static void info(Args... args) 
    {
        std::cout << "[" << termcolor::green << termcolor::bold << "INFO" << termcolor::reset << "]: ";
        ((std::cout << args << ' '), ...) << std::endl;
    }

    template <class... Args>
    static void warn(Args... args) 
    {
        std::cout << "[" << termcolor::yellow << termcolor::bold << "WARN" << termcolor::reset << "]: ";
        ((std::cout << args << ' '), ...) << std::endl;
    }

    template <class... Args>
    static void debug(Args... args) 
    {
        std::cout << "[" << termcolor::blue << termcolor::bold << "DEBUG" << termcolor::reset << "]: ";
        ((std::cout << args << ' '), ...) << std::endl;
    }
};
