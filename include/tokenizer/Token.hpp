#pragma once

#include <tokenizer/TokenType.hpp>
#include <any>
#include <string>

class Token 
{
private:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
public:

    Token(TokenType type, const std::string &, const std::any &, const int &);
    std::string toString();
};