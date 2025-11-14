#include <scanner.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct
{
    const char *name;
    TokenType type;
} KeyWords;

static KeyWords keyWords[] = {
    {"display", TK_DISPLAY},
    {"true", TK_TRUE},
    {"false", TK_FALSE},
    {"nil", TK_NIL},
    {"set", TK_SET},
    {NULL, TK_EOF}};

struct Scanner
{
    Token *tokens;
    size_t token_capacity;
    size_t token_count;
    char *last_error;
    const char *source;
    size_t size;
    size_t current;
    size_t start;
    int line;
};

Scanner *createScanner(const char *source, size_t size)
{
    Scanner *scanner = malloc(sizeof(Scanner));
    if (scanner == NULL)
    {
        printf("ERROR: Failed to allocate scanner\n");
        return NULL;
    }

    scanner->last_error = NULL;

    scanner->source = source;
    scanner->size = size;

    scanner->current = 0;
    scanner->start = 0;
    scanner->line = 1;
    scanner->tokens = NULL;
    scanner->token_count = 0;
    scanner->token_capacity = 0;

    return scanner;
}

void destroyScanner(Scanner *scanner)
{
    if (scanner->last_error)
        free(scanner->last_error);

    size_t i;
    for (i = 0; i < scanner->token_count; i++)
    {
        Token *token = &scanner->tokens[i];
        if (token->lexeme != NULL)
        {
            free((void *)token->lexeme);
            token->lexeme = NULL;
        }

        if (token->type == TK_STRING && token->literal.string != NULL)
        {
            free((void *)token->literal.string);
            token->literal.string = NULL;
        }
    }

    if (scanner->tokens)
        free(scanner->tokens);
    free(scanner);
    scanner = NULL;
}

void addToken(Scanner *scanner, TokenType type, Value literal)
{
    if (scanner->token_count >= scanner->token_capacity)
    {
        scanner->token_capacity = scanner->token_capacity == 0 ? 10 : scanner->token_capacity * 2;
        scanner->tokens = realloc(scanner->tokens, sizeof(Token) * scanner->token_capacity);
    }

    size_t lenght = scanner->current - scanner->start;
    char *lexeme = malloc(lenght + 1);
    strncpy(lexeme, &scanner->source[scanner->start], lenght);
    lexeme[lenght] = '\0';

    scanner->tokens[scanner->token_count].type = type;
    scanner->tokens[scanner->token_count].lexeme = lexeme;
    scanner->tokens[scanner->token_count].line = scanner->line;
    scanner->tokens[scanner->token_count].literal = literal;
    scanner->token_count++;
}

int isAtEnd(const Scanner *scanner)
{
    return scanner->source[scanner->current] == '\0';
}

char advance(Scanner *scanner)
{
    return scanner->source[scanner->current++];
}

int match(Scanner *scanner, char expected)
{
    if (isAtEnd(scanner))
        return 0;
    if (scanner->source[scanner->current] != expected)
        return 0;
    scanner->current++;
    return 1;
}

char peek(Scanner *scanner)
{
    return scanner->source[scanner->current];
}

char peekNext(Scanner *scanner)
{
    if (scanner->current + 1 >= scanner->size)
        return '\0';
    return scanner->source[scanner->current + 1];
}

int string(Scanner *scanner)
{
    // Processa normalmente - lexeme COM aspas
    while (peek(scanner) != '"' && !isAtEnd(scanner))
    {
        if (peek(scanner) == '\n')
            scanner->line++;
        advance(scanner);
    }

    if (isAtEnd(scanner))
    {
        if (scanner->last_error)
            free(scanner->last_error);
        scanner->last_error = strdup("String nunca fechada!");
        return -1;
    }

    advance(scanner);

    addToken(scanner, TK_STRING, (Value){.ptr = NULL});

    Token *last_token = &scanner->tokens[scanner->token_count - 1];

    size_t lexeme_len = strlen(last_token->lexeme);
    if (lexeme_len >= 2)
    {
        char *literal_str = malloc(lexeme_len - 1); // -2 aspas +1 null
        strncpy(literal_str, last_token->lexeme + 1, lexeme_len - 2);
        literal_str[lexeme_len - 2] = '\0';
        last_token->literal.string = literal_str;
    }

    scanner->start = scanner->current;
    return 0;
}

int isDigit(char c)
{
    return c >= '0' && c <= '9';
}

int isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= '|') || c == '_';
}

int isAlphaNumeric(char c)
{
    return isDigit(c) || isAlpha(c);
}

int number(Scanner *scanner)
{
    while (isDigit(peek(scanner)) && !isAtEnd(scanner))
        advance(scanner);

    if (peek(scanner) == '.' && isDigit(peekNext(scanner)))
    {
        advance(scanner);
        while (isDigit(peek(scanner)))
            advance(scanner);
    }

    size_t lenght = scanner->current - scanner->start;
    char *text = malloc(lenght + 1);
    strncpy(text, &scanner->source[scanner->start], lenght);
    text[lenght] = '\0';

    double number = strtold(text, NULL);
    addToken(scanner, TK_NUMBER, (Value){.number = number});
    free(text);
    return 0;
}

void identifier(Scanner *scanner)
{
    while (isAlphaNumeric(peek(scanner)))
        advance(scanner);

    size_t length = scanner->current - scanner->start;
    char *text = malloc(length + 1);
    strncpy(text, &scanner->source[scanner->start], length);
    text[length] = '\0';

    TokenType type = TK_IDENTIFIER;
    for (int i = 0; keyWords[i].name != NULL; i++) // ← CORREÇÃO
    {
        if (strcmp(text, keyWords[i].name) == 0)
        {
            type = keyWords[i].type;
            break;
        }
    }

    free(text);
    addToken(scanner, type, (Value){.ptr = NULL});
}

int scanToken(Scanner *scanner)
{
    char c = advance(scanner);

    switch (c)
    {
    case '(':
        addToken(scanner, TK_LEFT_PAREN, (Value){.ptr = NULL});
        break;
    case ')':
        addToken(scanner, TK_RIGHT_PAREN, (Value){.ptr = NULL});
        break;
    case ';':
        addToken(scanner, TK_SEMICOLON, (Value){.ptr = NULL});
        break;
    case '*':
    case '/':
        if (match(scanner, '/'))
        {
            // Comentário - ignora até o fim da linha
            while (peek(scanner) != '\n' && !isAtEnd(scanner))
                advance(scanner);
        }
        break;
    
    case '=':
    {
        addToken(scanner, TK_EQUAL, (Value){.ptr = NULL});
        break;
    }

    case ' ':
    case '\r':
    case '\t':
        // Ignora whitespace
        break;

    case '\n':
        scanner->line++;
        break;

    case '"':
        int result = string(scanner);
        if (result < 0)
        {
            return 1;
        }
        break;

    default:
        if (isDigit(c))
        {
            number(scanner);
        }
        else if (isAlpha(c))
        {
            identifier(scanner);
        }
        else
        {
            if (scanner->last_error)
                free(scanner->last_error);
        }
        break;
    }

    return 0;
}

int scanTokens(Scanner *scanner)
{
    while (scanner->current < scanner->size)
    {
        scanner->start = scanner->current;
        int scannerRet = scanToken(scanner);
        if (scannerRet < 0)
        {
            return scannerRet;
        }
    }
    addToken(scanner, TK_EOF, (Value){.string = ""});
    return 0;
}

char *getLastErrorScanner(Scanner *scanner)
{
    if (scanner->last_error)
        return scanner->last_error;
    return "\0";
}

Token *getTokens(Scanner *scanner)
{
    return scanner->tokens;
}

size_t getTokensCount(Scanner *scanner)
{
    return scanner->token_count;
}