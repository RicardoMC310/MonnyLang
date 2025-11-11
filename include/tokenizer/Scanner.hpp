#pragma once

#include <tokenizer/Scanner.hpp>
#include <tokenizer/Token.hpp>
#include <tokenizer/TokenType.hpp>
#include <vector>
#include <unordered_map>
#include <string>

class Scanner
{
private:
    std::vector<Token> tokens;
    std::string source;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    std::unordered_map<std::string, TokenType> keyWords = {
        {"output", TokenType::OUTPUT}
    };

    bool isAtEnd();
    char advance();
    void scanToken();
    void addToken(TokenType type, std::any literal);
    void addToken(TokenType type);
    bool match(char expected);
    char peek();
    char peekNext();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void number();
    void string();
    void identifier();
    char previous();
    char escapeSequence();

public:
    Scanner(const std::string &source);
    std::vector<Token> scanTokens();
};