/* lexer.c — tokenizer that builds a Token linked list */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

static Token* make_token(TokenType type, const char *lexeme) {
    Token *t = (Token*)malloc(sizeof(Token));
    t->type = type;
    t->lexeme = strdup(lexeme);
    t->next = NULL;
    return t;
}

static void append_token(Token **head, Token **tail, Token *t) {
    if (*head == NULL) {
        *head = *tail = t;
    } else {
        (*tail)->next = t;
        *tail = t;
    }
}

Token* tokenize_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "Could not open %s\n", path); return NULL; }

    Token *head = NULL, *tail = NULL;
    int c;
    char buf[1024];

    while ((c = fgetc(f)) != EOF) {
        if (isspace(c)) continue;

        if (isalpha(c) || c == '_') {
            int i = 0;
            buf[i++] = (char)c;
            while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) buf[i++] = (char)c;
            buf[i] = '\0';
            if (c != EOF) ungetc(c, f);
            TokenType type = TOK_IDENT;
            if (strcmp(buf, "print") == 0 || strcmp(buf, "while") == 0 || strcmp(buf, "func") == 0)
                type = TOK_KEYWORD;
            append_token(&head, &tail, make_token(type, buf));
            continue;
        }

        if (isdigit(c)) {
            int i = 0;
            buf[i++] = (char)c;
            while ((c = fgetc(f)) != EOF && isdigit(c)) buf[i++] = (char)c;
            buf[i] = '\0';
            if (c != EOF) ungetc(c, f);
            append_token(&head, &tail, make_token(TOK_NUMBER, buf));
            continue;
        }

        if (c == '"') {
            int i = 0;
            buf[i++] = '"';
            while ((c = fgetc(f)) != EOF && c != '"') {
                if (c == '\\') { // escape
                    int nxt = fgetc(f);
                    if (nxt == EOF) break;
                    buf[i++] = (char)'\\';
                    buf[i++] = (char)nxt;
                } else {
                    buf[i++] = (char)c;
                }
            }
            buf[i++] = '"';
            buf[i] = '\0';
            append_token(&head, &tail, make_token(TOK_STRING, buf));
            continue;
        }

        // punctuation
        if (strchr(";,(){}", c)) {
            buf[0] = (char)c; buf[1] = '\0';
            append_token(&head, &tail, make_token(TOK_PUNCT, buf));
            continue;
        }

        // operators (one or two char)
        int next = fgetc(f);
        if (next != EOF) {
            char opbuf[4] = {0};
            opbuf[0] = (char)c; opbuf[1] = (char)next; opbuf[2] = '\0';
            if (strcmp(opbuf, "==") == 0 || strcmp(opbuf, "!=") == 0 || strcmp(opbuf, "<=") == 0 || strcmp(opbuf, ">=") == 0) {
                append_token(&head, &tail, make_token(TOK_OP, opbuf));
            } else {
                ungetc(next, f);
                char op1[2] = { (char)c, '\0' };
                append_token(&head, &tail, make_token(TOK_OP, op1));
            }
        } else {
            char op1[2] = { (char)c, '\0' };
            append_token(&head, &tail, make_token(TOK_OP, op1));
        }
    }

    append_token(&head, &tail, make_token(TOK_EOF, ""));
    fclose(f);
    return head;
}

void free_tokens(Token* head) {
    Token *t = head;
    while (t) {
        Token *n = t->next;
        free(t->lexeme);
        free(t);
        t = n;
    }
}

void lex_file(const char *path) {
    Token *head = tokenize_file(path);
    for (Token *t = head; t != NULL; t = t->next) {
        const char *typ = "UNKNOWN";
        switch (t->type) {
            case TOK_IDENT: typ = "IDENT"; break;
            case TOK_NUMBER: typ = "NUMBER"; break;
            case TOK_STRING: typ = "STRING"; break;
            case TOK_KEYWORD: typ = "KEYWORD"; break;
            case TOK_PUNCT: typ = "PUNCT"; break;
            case TOK_OP: typ = "OP"; break;
            case TOK_EOF: typ = "EOF"; break;
        }
        printf("%s\t%s\n", typ, t->lexeme);
    }
    free_tokens(head);
}
