/* ph_engine.c — Minimal PH engine stub
 * This is a starting point for the interpreter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ph_engine.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

void print_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "Could not open %s\n", path); exit(1); }
    int c;
    while ((c = fgetc(f)) != EOF) putchar(c);
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("PHLang engine\n");
        printf("Usage: %s <file.ph>\n", argv[0]);
        printf("       %s --lex <file.ph>\n", argv[0]);
        printf("       %s --parse <file.ph>\n", argv[0]);
        printf("       %s --run <file.ph>\n", argv[0]);
        return 0;
    }

    if (argc >= 3 && strcmp(argv[1], "--lex") == 0) {
        lex_file(argv[2]);
        return 0;
    }

    if (argc >= 3 && strcmp(argv[1], "--parse") == 0) {
        Token *toks = tokenize_file(argv[2]);
        if (!toks) { fprintf(stderr, "Failed to tokenize %s\n", argv[2]); return 1; }
        ASTNode *root = parse_tokens(toks);
        if (!root) { fprintf(stderr, "Parse failed for %s\n", argv[2]); free_tokens(toks); return 1; }
        dump_ast(root, 0);
        free_ast(root);
        free_tokens(toks);
        return 0;
    }

    if (argc >= 3 && strcmp(argv[1], "--compile") == 0) {
        int r = compile_file_to_c(argv[2]);
        if (r != 0) { fprintf(stderr, "Compile failed for %s\n", argv[2]); return 1; }
        printf("Wrote output/output.c\n");
        return 0;
    }

    if (argc >= 3 && strcmp(argv[1], "--print") == 0) {
        print_file(argv[2]);
        return 0;
    }

    // if first argument looks like a flag but unrecognized - print help
    if (argv[1][0] == '-') {
        printf("Unknown option: %s\n", argv[1]);
        return 1;
    }

    // default: run the file
    return interpret_file(argv[1]);
}  
