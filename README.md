# PHLang - A Simple Educational Programming Language

PHLang (PH Language) is a tiny educational programming language implemented in C.

## Features (Current)
- Print statements: `print "Hello World";`
- Basic arithmetic expressions: `1 + 2 * 3`
- Variable assignment: `x = 5;`
- Simple while loops: `while (x > 0) { ... }`
- Blocks with `{ }`

## Syntax Rules
- Statements end with `;`
- Strings in double quotes
- Numbers are integers
- Operators: `+ - * / ( )`
- Keywords: `print`, `while`

## Building & Running
```bash
gcc phlang_engine/ph_engine.c -o phlang
./phlang ph_programs/hello.ph