#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* make_node(ASTNodeType type) {
    ASTNode *n = (ASTNode*)malloc(sizeof(ASTNode));
    n->type = type;
    n->text = NULL;
    n->left = n->right = n->next = NULL;
    return n;
}

void dump_indent(int indent) {
    for (int i = 0; i < indent; ++i) putchar(' ');
}

void dump_ast(ASTNode* node, int indent) {
    if (!node) return;
    ASTNode *cur = node;
    while (cur) {
        dump_indent(indent);
        switch (cur->type) {
            case AST_PROGRAM:
                printf("PROGRAM\n");
                dump_ast(cur->left, indent + 2);
                break;
            case AST_PRINT:
                printf("PRINT\n");
                dump_ast(cur->left, indent + 2);
                break;
            case AST_ASSIGN:
                printf("ASSIGN %s\n", cur->text ? cur->text : "");
                dump_ast(cur->left, indent + 2);
                break;
            case AST_NUMBER:
                printf("NUMBER %s\n", cur->text ? cur->text : "");
                break;
            case AST_IDENT:
                printf("IDENT %s\n", cur->text ? cur->text : "");
                break;
            case AST_STRING:
                printf("STRING %s\n", cur->text ? cur->text : "");
                break;
            case AST_BINARY:
                printf("BINARY %s\n", cur->text ? cur->text : "");
                dump_ast(cur->left, indent + 2);
                dump_ast(cur->right, indent + 2);
                break;
            case AST_WHILE:
                printf("WHILE\n");
                dump_ast(cur->left, indent + 2);
                dump_ast(cur->right, indent + 2);
                break;
            default:
                printf("<unknown node>\n");
        }
        cur = cur->next;
    }
}

void free_ast(ASTNode* node) {
    if (!node) return;
    if (node->text) free(node->text);
    if (node->left) free_ast(node->left);
    if (node->right) free_ast(node->right);
    if (node->next) free_ast(node->next);
    free(node);
}
