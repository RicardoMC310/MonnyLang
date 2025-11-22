#pragma once

#include <iostream>

class Monny
{
    static void run(const std::string &source);

public:
    static void runScriptFile(const std::string &path);
    static void runREPL();
};