#pragma once

#include <iostream>

class Debug
{
public:
    template <class... M>
    static void error(M... messages)
    {
        (std::cout << ... << messages) << '\n';
    }

    template <class... M>
    static void info(M... messages)
    {
        (std::cout << ... << messages) << '\n';
    }

    template <class... M>
    static void warn(M... messages)
    {
        (std::cout << ... << messages) << '\n';
    }
};