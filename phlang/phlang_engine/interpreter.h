#ifndef PH_INTERPRETER_H
#define PH_INTERPRETER_H

/* Interpret a PH source file; returns 0 on success, non-zero on error */
int interpret_file(const char *path);

#endif /* PH_INTERPRETER_H */