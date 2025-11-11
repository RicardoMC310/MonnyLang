#include <tokenizer/Token.hpp>

Token::Token(TokenType type, const std::string &lexeme, const std::any &literal, const int &line) : type(type),
                                                                                                    lexeme(lexeme),
                                                                                                    literal(literal),
                                                                                                    line(line) {}

std::string Token::toString()
{
    std::string valLiteral = "no literal";

    if (literal.has_value())
    {

        const auto &type = literal.type();

        if (type == typeid(nullptr))
        {
            valLiteral = "nil";
        }
        else if (type == typeid(std::string))
        {
            valLiteral = std::any_cast<std::string>(literal);
        }
        else if (type == typeid(double))
        {
            double val = std::any_cast<double>(literal);
            valLiteral = std::to_string(val);
        }
    }

    return lexeme + " [" + valLiteral + "]";
}
