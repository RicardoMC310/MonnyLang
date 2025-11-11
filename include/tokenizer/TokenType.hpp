#pragma once

enum class TokenType
{
    THIS_EOF,

    OUTPUT,

    LEFT_PAREN, RIGHT_PAREN,
    COMMA, SEMICOLON,

    STRING, NUMBER, IDENTIFIER
};