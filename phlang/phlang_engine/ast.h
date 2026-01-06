#ifndef PH_AST_H
#define PH_AST_H

#include <stddef.h>

typedef enum {
    AST_PROGRAM,
    AST_PRINT,
    AST_ASSIGN,
    AST_BINARY,
    AST_NUMBER,
    AST_IDENT,
    AST_STRING,
    AST_WHILE
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char *text; /* for number/ident/string values */
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next; /* for statement lists */
} ASTNode;

ASTNode* make_node(ASTNodeType type);
void free_ast(ASTNode* node);
void dump_ast(ASTNode* node, int indent);

#endif /* PH_AST_H */
