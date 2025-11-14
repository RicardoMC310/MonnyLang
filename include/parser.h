#ifndef PARSER_H
#define PARSER_H

#include <token.h>

typedef struct Parser Parser;
typedef struct ASTNode ASTNode;

typedef enum 
{
    ND_NULL,
    ND_PRINT,
    ND_STRING,
    ND_NUMBER,
    ND_IDENTIFIER,
} NodeType;

Parser *createParser(Token *tokens, size_t tokensCount);
void destroyParser(Parser *parser);

ASTNode* parse(Parser* parser);
int isAtEndParser(Parser *parser);

void freeAST(ASTNode *node);
NodeType getTypeAST(ASTNode *node);
char *getValueAST(ASTNode *node);
ASTNode *getLeftExpr(ASTNode *node);
ASTNode *getRightExpr(ASTNode *node);
#endif // PARSER_H