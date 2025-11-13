#ifndef PARSER_H
#define PARSER_H

#include <token.h>

typedef struct Parser Parser;
typedef struct ASTNode ASTNode;

Parser *createParser(Token *tokens, size_t tokensCount);
void destroyParser(Parser *parser);

ASTNode* parse(Parser* parser);
void freeAST(ASTNode *node);

char *getTypeAST(ASTNode *node);
#endif // PARSER_H