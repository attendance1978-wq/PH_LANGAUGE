#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

static Token *cur;
static void advance() { if (cur) cur = cur->next; }
static int is_punct(const char *s) { return cur && cur->type == TOK_PUNCT && strcmp(cur->lexeme, s) == 0; }
static int is_op(const char *s) { return cur && cur->type == TOK_OP && strcmp(cur->lexeme, s) == 0; }
static int is_keyword(const char *s) { return cur && cur->type == TOK_KEYWORD && strcmp(cur->lexeme, s) == 0; }

/* Forward declarations */
static ASTNode* parse_statement();
static ASTNode* parse_expression();

ASTNode* parse_tokens(Token* tokens) {
    cur = tokens;
    ASTNode *prog = make_node(AST_PROGRAM);
    ASTNode *stmts = NULL;
    ASTNode *tail = NULL;

    while (cur && cur->type != TOK_EOF) {
        ASTNode *st = parse_statement();
        if (st) {
            if (!stmts) stmts = tail = st; else { tail->next = st; tail = st; }
        } else {
            // skip unexpected token to avoid infinite loop
            advance();
        }
    }

    prog->left = stmts;
    return prog;
}

static ASTNode* parse_number() {
    ASTNode *n = make_node(AST_NUMBER);
    n->text = strdup(cur->lexeme);
    advance();
    return n;
}

static ASTNode* parse_primary() {
    if (!cur) return NULL;
    if (cur->type == TOK_NUMBER) return parse_number();
    if (cur->type == TOK_STRING) {
        ASTNode *n = make_node(AST_STRING);
        n->text = strdup(cur->lexeme);
        advance();
        return n;
    }
    if (cur->type == TOK_IDENT) {
        ASTNode *n = make_node(AST_IDENT);
        n->text = strdup(cur->lexeme);
        advance();
        return n;
    }
    if (is_punct("(")) {
        advance();
        ASTNode *e = parse_expression();
        if (is_punct(")")) advance();
        return e;
    }
    return NULL;
}

static ASTNode* parse_unary() {
    if (is_op("-")) {
        char op[2] = "-";
        advance();
        ASTNode *right = parse_unary();
        ASTNode *n = make_node(AST_BINARY);
        n->text = strdup(op);
        n->left = make_node(AST_NUMBER); n->left->text = strdup("0");
        n->right = right;
        return n;
    }
    return parse_primary();
}

static ASTNode* parse_binary_rhs(int expr_prec, ASTNode *lhs) {
    while (1) {
        if (!cur || cur->type != TOK_OP) break;
        int prec;
        const char *op = cur->lexeme;
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) prec = 10;
        else if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) prec = 20;
        else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) prec = 5;
        else if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0) prec = 7;
        else prec = 1;

        if (prec < expr_prec) break;

        char opbuf[8]; strncpy(opbuf, op, sizeof(opbuf)-1); opbuf[sizeof(opbuf)-1] = '\0';
        advance();
        ASTNode *rhs = parse_unary();
        if (!rhs) break;

        while (cur && cur->type == TOK_OP) {
            int next_prec;
            const char *op2 = cur->lexeme;
            if (strcmp(op2, "+") == 0 || strcmp(op2, "-") == 0) next_prec = 10;
            else if (strcmp(op2, "*") == 0 || strcmp(op2, "/") == 0) next_prec = 20;
            else if (strcmp(op2, "==") == 0 || strcmp(op2, "!=") == 0) next_prec = 5;
            else if (strcmp(op2, "<") == 0 || strcmp(op2, ">") == 0 || strcmp(op2, "<=") == 0 || strcmp(op2, ">=") == 0) next_prec = 7;
            else next_prec = 1;
            if (next_prec > prec) {
                rhs = parse_binary_rhs(prec + 1, rhs);
            } else break;
        }

        ASTNode *n = make_node(AST_BINARY);
        n->text = strdup(opbuf);
        n->left = lhs;
        n->right = rhs;
        lhs = n;
    }
    return lhs;
}

static ASTNode* parse_expression() {
    ASTNode *lhs = parse_unary();
    if (!lhs) return NULL;
    return parse_binary_rhs(0, lhs);
}

static ASTNode* parse_statement() {
    if (is_keyword("print")) {
        advance();
        ASTNode *expr = parse_expression();
        if (is_punct(";")) advance();
        ASTNode *n = make_node(AST_PRINT);
        n->left = expr;
        return n;
    }

    if (is_keyword("while")) {
        advance();
        if (is_punct("(")) advance();
        ASTNode *cond = parse_expression();
        if (is_punct(")")) advance();
        ASTNode *body = NULL;
        if (is_punct("{")) {
            advance();
            ASTNode *first = NULL, *tail = NULL;
            while (cur && !is_punct("}")) {
                ASTNode *s = parse_statement();
                if (s) {
                    if (!first) first = tail = s; else { tail->next = s; tail = s; }
                } else break;
            }
            if (is_punct("}")) advance();
            body = make_node(AST_PROGRAM);
            body->left = first;
        }
        ASTNode *n = make_node(AST_WHILE);
        n->left = cond;
        n->right = body;
        return n;
    }

    if (cur && cur->type == TOK_IDENT) {
        char *name = strdup(cur->lexeme);
        advance();
        if (is_op("=")) {
            advance();
            ASTNode *expr = parse_expression();
            if (is_punct(";")) advance();
            ASTNode *n = make_node(AST_ASSIGN);
            n->text = name;
            n->left = expr;
            return n;
        } else {
            ASTNode *id = make_node(AST_IDENT); id->text = name;
            ASTNode *n = make_node(AST_PRINT);
            n->left = id;
            if (is_punct(";")) advance();
            return n;
        }
    }

    ASTNode *e = parse_expression();
    if (e && is_punct(";")) advance();
    return e;
}