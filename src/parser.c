#include <parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ASTNode
{
    NodeType type;
    const char *value;
    struct ASTNode *left;
    struct ASTNode *right;

    union
    {
        struct
        {
            const char *varName;
            struct ASTNode *value;
        } setVar;
    } data;
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

ASTNode *createNode()
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL;

    node->type = ND_NULL;
    node->left = NULL;
    node->right = NULL;
    node->value = NULL;

    node->data.setVar.varName = NULL;
    node->data.setVar.value = NULL;

    return node;
}

ASTNode *parserExpr(Parser *parser)
{
    ASTNode *node = createNode();

    if (expected(parser, TK_NUMBER))
    {
        node->type = ND_NUMBER;
        // ALOCA no heap em vez de usar stack
        char *buffer = malloc(64);
        snprintf(buffer, 64, "%g", parser->tokens[parser->current].literal.number);
        node->value = buffer;
        node->left = NULL;
        node->right = NULL;
        advanceToken(parser);
    }
    else if (expected(parser, TK_STRING))
    {
        node->type = ND_STRING;
        node->value = parser->tokens[parser->current].literal.string; // Scanner é dono
        node->left = NULL;
        node->right = NULL;
        advanceToken(parser);
    }
    else if (expected(parser, TK_IDENTIFIER))
    {
        node->type = ND_IDENTIFIER;
        node->value = parser->tokens[parser->current].lexeme; // Scanner é dono
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
    ASTNode *node = createNode();

    node->type = ND_PRINT;
    node->value = NULL;

    consume(parser, TK_DISPLAY, "expected 'display'");
    consume(parser, TK_LEFT_PAREN, "expected '(' after display");

    node->left = parserExpr(parser);

    consume(parser, TK_RIGHT_PAREN, "expected ')' after expressions");
    consume(parser, TK_SEMICOLON, "expected ';' after ')'");

    node->right = NULL;

    return node;
}

ASTNode *setSTMT(Parser *parser)
{
    consume(parser, TK_SET, "Expected 'SET'");
    ASTNode *node = createNode();
    node->type = ND_SET_VAR;

    if (!expected(parser, TK_IDENTIFIER))
    {
        printf("Expected variable name after 'set'\n");
        freeAST(node);
        return NULL;
    }

    const char *varName = parser->tokens[parser->current].lexeme; // Scanner é dono
    advanceToken(parser);

    if (!expected(parser, TK_EQUAL))
    {
        printf("Expected '=' after variable name\n");
        freeAST(node);
        return NULL;
    }

    advanceToken(parser);

    ASTNode *value = parserExpr(parser);
    if (value == NULL)
    {
        freeAST(node);
        // NÃO free varName - Scanner é dono
        return NULL;
    }

    if (!expected(parser, TK_SEMICOLON))
    {
        printf("Expected ';' after expression\n");
        freeAST(node);
        freeAST(value);
        return NULL;
    }
    advanceToken(parser);

    node->data.setVar.varName = varName;
    node->data.setVar.value = value;

    return node;
}

void freeAST(ASTNode *node)
{
    if (node == NULL)
        return;

    freeAST(node->left);
    freeAST(node->right);

    if (node->type == ND_SET_VAR)
    {
        if (node->data.setVar.value != NULL)
        {
            freeAST(node->data.setVar.value);
        }
    }

    if (node->type == ND_NUMBER && node->value != NULL)
    {
        free((void *)node->value);
    }

    free(node);
}

ASTNode *identifierSTMT(Parser *parser)
{
    ASTNode *node = createNode();
    if (node == NULL)
        return NULL;
    node->type = ND_IDENTIFIER;

    Token token = advanceToken(parser);

    node->value = token.lexeme;

    return node;
}

ASTNode *assignmentSTMT(Parser *parser, const char *varName)
{
    consume(parser, TK_EQUAL, "Expected '=' after variable name.");
    ASTNode *node = createNode();

    node->type = ND_SET_VAR;

    ASTNode *value = parserExpr(parser);
    if (value == NULL)
    {
        freeAST(node);
        return NULL;
    }

    if (!expected(parser, TK_SEMICOLON)) {
        printf("Expected ';' after expression\n");
        freeAST(node);
        freeAST(value);
        return NULL;
    }
    advanceToken(parser);

    node->data.setVar.varName = varName;
    node->data.setVar.value = value;

    return node;
}

ASTNode *parse(Parser *parser)
{
    if (expected(parser, TK_DISPLAY))
    {
        return printSTMT(parser);
    }
    else if (expected(parser, TK_SET))
    {
        return setSTMT(parser);
    }
    else if (expected(parser, TK_IDENTIFIER))
    {
        Token identifier = parser->tokens[parser->current];
        advanceToken(parser);

        if (expected(parser, TK_EQUAL))
        {
            return assignmentSTMT(parser, identifier.lexeme);
        }
        else
        {
            return setSTMT(parser);
        }
    }
    else
    {
        printf("Expressão inválida!\n");
        return NULL;
    }
}

NodeType getTypeAST(ASTNode *node)
{
    if (node == NULL)
    {
        return ND_NULL;
    }

    return node->type;
}

const char *getValueAST(ASTNode *node)
{
    if (node == NULL)
    {
        return "NULL";
    }

    return node->value;
}

ASTNode *getLeftExpr(ASTNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    return node->left;
}

ASTNode *getRightExpr(ASTNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    return node->right;
}

int isAtEndParser(Parser *parser)
{
    if (parser == NULL)
        return 1;

    return parser->current >= parser->tokensCount ||
           parser->tokens[parser->current].type == TK_EOF;
}

ASTNode *getVarValue(ASTNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    return node->data.setVar.value;
}

const char *getVarName(ASTNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    return node->data.setVar.varName;
}