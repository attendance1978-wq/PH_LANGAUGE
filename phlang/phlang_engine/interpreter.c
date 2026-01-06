/* interpreter.c — evaluate AST nodes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "ast.h"
#include "env.h"
#include "lexer.h"
#include "parser.h"

static int is_truthy_value(const Value *v) {
    if (!v) return 0;
    if (v->type == VAL_NUMBER) return v->number != 0.0;
    if (v->type == VAL_STRING) return v->string && v->string[0] != '\0';
    return 0;
}

static Value* eval_expression(ASTNode *node, Env *env);
static void exec_statement(ASTNode *node, Env *env);

static Value* eval_expression(ASTNode *node, Env *env) {
    if (!node) return NULL;
    switch (node->type) {
        case AST_NUMBER: {
            double n = atof(node->text ? node->text : "0");
            return value_number(n);
        }
        case AST_STRING: {
            const char *s = node->text ? node->text : "";
            // strip surrounding quotes if present
            size_t len = strlen(s);
            if (len >= 2 && s[0] == '"' && s[len-1] == '"') {
                char *t = strndup(s+1, len-2);
                Value *v = value_string(t);
                free(t);
                return v;
            }
            return value_string(s);
        }
        case AST_IDENT: {
            Value *v = env_get(env, node->text);
            if (!v) return value_number(0);
            // return a copy
            if (v->type == VAL_NUMBER) return value_number(v->number);
            return value_string(v->string ? v->string : "");
        }
        case AST_BINARY: {
            Value *L = eval_expression(node->left, env);
            Value *R = eval_expression(node->right, env);
            if (!L || !R) { free_value(L); free_value(R); return value_number(0); }
            const char *op = node->text ? node->text : "";
            Value *res = NULL;
            if (strcmp(op, "+") == 0) {
                if (L->type == VAL_STRING || R->type == VAL_STRING) {
                    // concat
                    const char *ls = (L->type == VAL_STRING && L->string) ? L->string : (char[1]){0};
                    const char *rs = (R->type == VAL_STRING && R->string) ? R->string : (char[1]){0};
                    size_t n = strlen(ls) + strlen(rs) + 1;
                    char *buf = malloc(n);
                    strcpy(buf, ls);
                    strcat(buf, rs);
                    res = value_string(buf);
                    free(buf);
                } else {
                    res = value_number(L->number + R->number);
                }
            } else if (strcmp(op, "-") == 0) {
                res = value_number(L->number - R->number);
            } else if (strcmp(op, "*") == 0) {
                res = value_number(L->number * R->number);
            } else if (strcmp(op, "/") == 0) {
                res = value_number(L->number / R->number);
            } else if (strcmp(op, "==") == 0) {
                if (L->type == VAL_STRING || R->type == VAL_STRING) res = value_number(strcmp(L->string?L->string:"", R->string?R->string:"") == 0);
                else res = value_number(L->number == R->number);
            } else if (strcmp(op, "!=") == 0) {
                if (L->type == VAL_STRING || R->type == VAL_STRING) res = value_number(strcmp(L->string?L->string:"", R->string?R->string:"") != 0);
                else res = value_number(L->number != R->number);
            } else if (strcmp(op, "<") == 0) {
                res = value_number(L->number < R->number);
            } else if (strcmp(op, ">") == 0) {
                res = value_number(L->number > R->number);
            } else if (strcmp(op, "<=") == 0) {
                res = value_number(L->number <= R->number);
            } else if (strcmp(op, ">=") == 0) {
                res = value_number(L->number >= R->number);
            } else {
                res = value_number(0);
            }
            free_value(L); free_value(R);
            return res;
        }
        default:
            return value_number(0);
    }
}

static void exec_statement(ASTNode *node, Env *env) {
    if (!node) return;
    switch (node->type) {
        case AST_PRINT: {
            Value *v = eval_expression(node->left, env);
            if (v) {
                if (v->type == VAL_STRING) printf("%s\n", v->string ? v->string : "");
                else printf("%g\n", v->number);
                free_value(v);
            }
            break;
        }
        case AST_ASSIGN: {
            Value *v = eval_expression(node->left, env);
            env_set(env, node->text, v);
            free_value(v);
            break;
        }
        case AST_WHILE: {
            while (1) {
                Value *cond = eval_expression(node->left, env);
                int truth = is_truthy_value(cond);
                free_value(cond);
                if (!truth) break;
                if (node->right && node->right->type == AST_PROGRAM) {
                    ASTNode *s = node->right->left;
                    while (s) { exec_statement(s, env); s = s->next; }
                }
            }
            break;
        }
        case AST_PROGRAM: {
            ASTNode *s = node->left;
            while (s) { exec_statement(s, env); s = s->next; }
            break;
        }
        default:
            // evaluate expression statements
            if (node->type == AST_BINARY || node->type == AST_NUMBER || node->type == AST_IDENT || node->type == AST_STRING) {
                Value *v = eval_expression(node, env);
                free_value(v);
            }
            break;
    }
}

int interpret_file(const char *path) {
    Token *toks = tokenize_file(path);
    if (!toks) return 1;
    ASTNode *root = parse_tokens(toks);
    if (!root) { free_tokens(toks); return 1; }

    Env *env = env_create();
    exec_statement(root, env);
    free_ast(root);
    env_free(env);
    free_tokens(toks);
    return 0;
}
