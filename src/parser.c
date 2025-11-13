#include <parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ASTNode
{
    char *type;
    char *value;
    struct ASTNode *left;
    struct ASTNode *right;
};

struct Parser
{
    Token *tokens;
    size_t tokensCount;
    size_t current;
    int line;
    char *last_error;
};

Parser *createParser(Token *tokens, size_t tokensCount)
{
    Parser *parser = malloc(sizeof(Parser));
    if (parser == NULL)
        return NULL;

    parser->tokens = tokens;
    parser->tokensCount = tokensCount;
    parser->line = 1;
    parser->current = 0;
    parser->last_error = NULL;

    return parser;
}

void destroyParser(Parser *parser)
{
    if (parser->last_error)
        free(parser->last_error);
    free(parser);
    parser = NULL;
}

Token advanceToken(Parser *parser)
{
    return parser->tokens[parser->current++];
}

int expected(Parser *parser, TokenType expected)
{
    return parser->tokens[parser->current].type == expected;
}

void consume(Parser *parser, TokenType type, const char *errorMsg)
{
    if (!expected(parser, type))
    {
        printf("Erro: %s\nlinha: %d\n", errorMsg, parser->tokens[parser->current].line);
        exit(1);
    }

    advanceToken(parser);
}

ASTNode *parserExpr(Parser *parser)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL;

    if (expected(parser, TK_NUMBER))
    {
        node->type = "number";
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%g", parser->tokens[parser->current].literal.number);
        node->value = strdup(buffer);
        node->left = NULL;
        node->right = NULL;
        advanceToken(parser);
    }
    else if (expected(parser, TK_STRING))
    {
        node->type = "string";
        node->value = strdup(parser->tokens[parser->current].literal.string);
        node->left = NULL;
        node->right = NULL;
        advanceToken(parser);
    }
    else if (expected(parser, TK_IDENTIFIER))
    {
        node->type = "identifier";
        node->value = strdup("NULL");
        node->left = NULL;
        node->right = NULL;
        advanceToken(parser);
    }
    else
    {
        printf("Erro: Expressão esperada\n");
        free(node);
        return NULL;
    }

    return node;
}

ASTNode *printSTMT(Parser *parser)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL;

    node->type = "printSTMT";
    node->value = NULL;

    consume(parser, TK_DISPLAY, "expected 'display'");
    consume(parser, TK_LEFT_PAREN, "expected '(' after display");

    node->left = parserExpr(parser);

    consume(parser, TK_RIGHT_PAREN, "expected ')' after expressions");
    consume(parser, TK_SEMICOLON, "expected ';' after ')'");

    node->right = NULL;

    return node;
}

void freeAST(ASTNode *node)
{
    if (node == NULL)
        return;

    if (node->value != NULL)
    {
        free(node->value);
    }

    freeAST(node->left);
    freeAST(node->right);
    free(node);
    node = NULL;
}

ASTNode *parse(Parser *parser)
{
    if (expected(parser, TK_DISPLAY))
    {
        return printSTMT(parser);
    }
    else
    {
        printf("Expressão inválida!\n");
        return NULL;
    }
}

char *getTypeAST(ASTNode *node)
{
    if (node == NULL)
    {
        return "NULL";
    }

    return node->type;
}