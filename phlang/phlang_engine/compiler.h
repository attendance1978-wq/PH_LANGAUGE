#ifndef PH_COMPILER_H
#define PH_COMPILER_H

#include "ast.h"

/* Compile a PH source file into C at output/output.c. Returns 0 on success. */
int compile_file_to_c(const char *path);

#endif /* PH_COMPILER_H */