#ifndef PH_PARSER_H
#define PH_PARSER_H

#include "lexer.h"
#include "ast.h"

/* Parse tokens into an AST program node. Returns allocated ASTNode (PROGRAM); caller frees with free_ast. */
ASTNode* parse_tokens(Token* tokens);

#endif /* PH_PARSER_H */