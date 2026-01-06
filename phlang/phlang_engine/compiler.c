#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"
#include "parser.h"

/* Very small C code generator for a subset of PHLang. Emits to output/output.c */

typedef struct VarNode { char *name; struct VarNode *next; } VarNode;

static int var_declared(VarNode *head, const char *name) {
    for (VarNode *v = head; v; v = v->next) if (strcmp(v->name, name) == 0) return 1;
    return 0;
}
static void add_var(VarNode **head, const char *name) {
    if (var_declared(*head, name)) return;
    VarNode *v = (VarNode*)malloc(sizeof(VarNode)); v->name = strdup(name); v->next = *head; *head = v;
}
static void free_vars(VarNode *head) { while (head) { VarNode *n=head->next; free(head->name); free(head); head=n; } }

static void emit_expr(FILE *out, ASTNode *node) {
    if (!node) fprintf(out, "0");
    switch (node->type) {
        case AST_NUMBER: fprintf(out, "%s", node->text ? node->text : "0"); break;
        case AST_STRING: fprintf(out, "%s", node->text ? node->text : "\"\""); break;
        case AST_IDENT: fprintf(out, "%s", node->text ? node->text : ""); break;
        case AST_BINARY:
            fprintf(out, "(");
            emit_expr(out, node->left);
            fprintf(out, " %s ", node->text ? node->text : "?");
            emit_expr(out, node->right);
            fprintf(out, ")");
            break;
        default: fprintf(out, "0"); break;
    }
}

static void collect_vars(VarNode **vars, ASTNode *node) {
    if (!node) return;
    if (node->type == AST_ASSIGN && node->text) add_var(vars, node->text);
    collect_vars(vars, node->left);
    collect_vars(vars, node->right);
    collect_vars(vars, node->next);
}

static void emit_statement(FILE *out, VarNode **vars, ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case AST_PRINT:
            if (node->left && node->left->type == AST_STRING && node->left->text) {
                // string literal includes quotes
                fprintf(out, "printf(\"%s\\n\");\n", node->left->text + 1); // crude: drop leading quote
            } else {
                fprintf(out, "printf(\"%%g\\n\", (double) (");
                emit_expr(out, node->left);
                fprintf(out, "));\n");
            }
            break;
        case AST_ASSIGN:
            if (!var_declared(*vars, node->text)) add_var(vars, node->text);
            fprintf(out, "%s = ", node->text);
            emit_expr(out, node->left);
            fprintf(out, ";\n");
            break;
        case AST_WHILE:
            fprintf(out, "while ("); emit_expr(out, node->left); fprintf(out, ") {\n");
            if (node->right && node->right->type == AST_PROGRAM) {
                ASTNode *s = node->right->left;
                while (s) { emit_statement(out, vars, s); s = s->next; }
            }
            fprintf(out, "}\n");
            break;
        case AST_PROGRAM:
            {
                ASTNode *s = node->left;
                while (s) { emit_statement(out, vars, s); s = s->next; }
            }
            break;
        default:
            // expression statement
            emit_expr(out, node);
            fprintf(out, ";\n");
            break;
    }
}

int compile_file_to_c(const char *path) {
    Token *toks = tokenize_file(path);
    if (!toks) return 1;
    ASTNode *root = parse_tokens(toks);
    if (!root) { free_tokens(toks); return 1; }

    VarNode *vars = NULL;
    collect_vars(&vars, root);

    FILE *out = fopen("output/output.c", "w");
    if (!out) { free_ast(root); free_tokens(toks); free_vars(vars); return 1; }

    fprintf(out, "#include <stdio.h>\n#include <stdlib.h>\n\nint main(void) {\n");

    // declare vars as doubles
    for (VarNode *v = vars; v; v = v->next) {
        fprintf(out, "double %s = 0;\n", v->name);
    }
    fprintf(out, "\n");

    // emit body
    emit_statement(out, &vars, root);

    fprintf(out, "return 0;\n}\n");
    fclose(out);

    free_ast(root);
    free_tokens(toks);
    free_vars(vars);
    return 0;
}