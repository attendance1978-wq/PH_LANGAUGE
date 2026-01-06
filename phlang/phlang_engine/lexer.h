#ifndef PH_LEXER_H
#define PH_LEXER_H

#include <stddef.h>

typedef enum {
    TOK_EOF,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_STRING,
    TOK_KEYWORD,
    TOK_PUNCT,
    TOK_OP
} TokenType;

typedef struct Token {
    TokenType type;
    char *lexeme;
    struct Token *next;
} Token;

/* Tokenize file: returns head of linked list of tokens. Caller must free with free_tokens */
Token* tokenize_file(const char *path);
void free_tokens(Token* head);

/* Convenience: print tokens to stdout (backwards compatible) */
void lex_file(const char *path);

#endif /* PH_LEXER_H */
