#include <utils/Debug.hpp>
#include <tokenizer/Scanner.hpp>
#include <types/throws/SyntaxError.hpp>

Scanner::Scanner(const std::string &source) : source(std::move(source)), line(1) {}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::THIS_EOF, "", std::any(), line);
    return tokens;
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}

char Scanner::advance()
{
    if (isAtEnd())
        return '\0';
    return source[current++];
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c)
    {
    case '/':
        if (match('/'))
        {
            while (peek() != '\n' && !isAtEnd())
                advance();
        }
        else if (match('*'))
        {
            while (!isAtEnd())
            {
                if (peek() == '*' && peekNext() == '/')
                {
                    advance();
                    advance();
                    break;
                }
                if (peek() == '\n')
                    line++;
                advance();
            }
        }
        break;
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case '\'':
    case '\"':
        string();
        break;
    case '\n':
        line++;
        break;
    case ' ':
    case '\t':
    case '\r':
        break;
    default:
        if (isDigit(c))
        {
            number();
        }
        else if (isAlphaNumeric(c))
        {
            identifier();
        }
        else
        {
            throw SyntaxError("Character unexpected: '" + std::string(1, c) + "' to line: " + std::to_string(line));
        }
        break;
    }
}

void Scanner::addToken(TokenType type)
{
    addToken(type, std::any());
}

void Scanner::addToken(TokenType type, std::any literal)
{
    std::string text{source.substr(start, current - start)};
    tokens.emplace_back(type, text, literal, line);
}

bool Scanner::match(char expected)
{
    if (isAtEnd() || peek() != expected)
        return false;
    advance();
    return true;
}

char Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return source[current];
}

char Scanner::peekNext()
{
    if (current + 1 >= source.length())
        return '\0';
    return source[current + 1];
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
    return isDigit(c) || isAlpha(c);
}

void Scanner::number()
{
    while (isDigit(peek()) && !isAtEnd())
        advance();

    if (peek() == '.' && isDigit(peekNext()))
    {
        advance();
        while (isDigit(peek()))
            advance();
    }

    std::string text(source.substr(start, current - start));
    double number = std::stod(text);
    addToken(TokenType::NUMBER, number);
}

void Scanner::string()
{
    std::string value;
    char str_op = source[start];

    while (!isAtEnd() && peek() != str_op)
    {
        if (match('\\'))
        {
            value += escapeSequence();
        }
        else
        {
            if (peek() == '\n')
            {
                line++;
            }
            value += advance();
        }
    };

    if (isAtEnd())
    {
        throw SyntaxError("string not closed.");
    }

    advance();

    addToken(TokenType::STRING, value);
}

char Scanner::escapeSequence()
{
    if (isAtEnd())
        return '\\';

    char c = advance();
    switch (c)
    {
    case 'n':
        return '\n';
    case 't':
        return '\t';
    case 'r':
        return '\r';
    case '\\':
        return '\\';
    case '\"':
        return '\"';
    case '\'':
        return '\'';
    default:
        return c;
    }
}

char Scanner::previous()
{
    if (current == 0)
        return source[0];
    return source[current - 1];
}

void Scanner::identifier()
{
    while(isAlphaNumeric(peek())) advance();

    std::string text{source.substr(start, current - start)};
    auto it = keyWords.find(text);
    TokenType type = it == keyWords.end() ? TokenType::IDENTIFIER : it->second;
    addToken(type);
}