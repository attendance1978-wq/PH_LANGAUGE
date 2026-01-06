# PHLang — Minimal language project

PHLang is an educational toy language. This repository contains a minimal interpreter scaffold, example PH programs, a simple build script, and documentation to get started.

Project layout:

```
phlang/                    <- project root
├── README.md
├── build.sh
├── phlang_engine/          <- core engine & components
│   ├── ph_engine.c
│   └── ...
├── ph_programs/            <- example .ph files
│   └── hello.ph
├── output/                 <- compiled output (if compiler is implemented)
│   └── output.c
└── docs/                   <- syntax & examples
    └── syntax.md
```

Getting started

- Build (Unix-like): `./build.sh`
- Run an example: `./phlang_engine/ph_engine ph_programs/hello.ph`

Notes

- The engine and other modules are stubs to get you started. Implement lexer, parser, AST, interpreter or a compiler as next steps.

## Contributing — expanding PHLang 🔧

If you'd like to add language features, implement components, or help with tests and CI, follow these steps:

1. Setup & prerequisites
   - Unix-like: install GCC (or use WSL / Git Bash) and make sure `gcc` is in PATH.
   - Windows: install MSYS2/MinGW or Git for Windows (Git Bash) to get `gcc`, or use `build.ps1` and a suitable toolchain.
   - Python 3 is used for a reference interpreter fallback (present in `tools/ref_interpreter.py`).

2. Build
   - Unix/Git Bash: `./build.sh`
   - Windows PowerShell: `./build.ps1`
   - The executable is `phlang_engine/ph_engine`.

3. Running the engine
   - Lex only: `phlang_engine/ph_engine --lex ph_programs/hello.ph`
   - Parse (print AST): `phlang_engine/ph_engine --parse ph_programs/math.ph`
   - Run program: `phlang_engine/ph_engine ph_programs/hello.ph` (or `--run`)
   - Compile to C: `phlang_engine/ph_engine --compile ph_programs/hello.ph` → writes `output/output.c`

4. Tests & running them
   - Unit and integration tests live under `tests/` (lexer, parser, interpreter, compiler).
   - Unix: `bash tests/<component>/test_<component>.sh`
   - Windows PowerShell: `powershell -ExecutionPolicy Bypass -File tests\<component>\test_<component>.ps1`
   - Tests auto-fallback to `tools/ref_interpreter.py` when a native build or `gcc` is not available.
   - Use the VS Code tasks (`.vscode/tasks.json`) to run build & tests from the editor.

5. How to expand the codebase (recommended order)
   - Lexer: add tokens and ability to produce a canonical token stream (`tokenize_file` in `lexer.c`), plus `tests/lexer` cases.
   - Parser & AST: implement grammar, recursive-descent parser (`parser.c`/`parser.h`), and `ast.h`/`ast.c` helpers; add `tests/parser` cases.
   - Interpreter: implement evaluation (`interpreter.c` + `env.c`/`env.h`), add integration tests in `tests/interpreter`.
   - Compiler: codegen to C (`compiler.c`/`compiler.h`) and tests under `tests/compiler`.

6. Tests & CI
   - Add deterministic unit tests for every new feature (edge cases, error behavior).
   - Keep tests small and additive. If you add a grammar feature, add both lexer and parser tests.
   - Consider adding GitHub Actions workflow to run tests on push/PR (we can add a starter workflow in `.github/workflows/`).

7. Style & contribution etiquette
   - Keep changes focused and well-documented.
   - Add tests with your change; make them pass locally before opening a PR.
   - Write clear commit messages and describe the design rationale in PR descriptions.

If you'd like, I can add a starter GitHub Actions workflow and example unit tests for a component you want to expand — which component should I scaffold tests for next? 🔁

---

## Quickstart ✅

1. Clone the repository and open it in VS Code.
2. Build:
   - Unix-like: `./build.sh`
   - Windows PowerShell: `./build.ps1`
3. Run an example:
   - `./phlang_engine/ph_engine ph_programs/hello.ph`
4. Run tests (example):
   - Bash: `bash tests/interpreter/test_interpreter.sh`
   - PowerShell: `powershell -ExecutionPolicy Bypass -File tests\interpreter\test_interpreter.ps1`

---

## Language Overview ✨

PHLang is intentionally minimal to make it easy to experiment with language features. The current supported features in the scaffold are:

- Comments: start with `#`
- Print: `print "text";` or `print expression;`
- Assignment: `x = 1 + 2;`
- Arithmetic: `+ - * /` and parentheses
- Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- While loops: `while (cond) { ... }`
- Blocks: `{ ... }`
- Functions: placeholder syntax `func name() { ... }` (not implemented yet)

Example `ph_programs/hello.ph`:

```
# hello.ph
print "Hello, PHLang!";
```

Example `ph_programs/math.ph`:

```
x = 1 + 2 * (3 + 4);
print x;
```

Example `ph_programs/loops.ph`:

```
i = 0;
while (i < 5) {
  print i;
  i = i + 1;
}
```

---

## Project Architecture 🏗️

- `phlang_engine/` — core implementation split into components:
  - `lexer.*` — tokenization and token API
  - `parser.*` — parsing tokens into AST
  - `ast.*` — AST types and helpers
  - `interpreter.*` — evaluate AST
  - `compiler.*` — emit C from AST (basic)
  - `env.*` — environment / symbol table
- `ph_programs/` — example `.ph` programs
- `tests/` — component tests & integration tests
- `tools/ref_interpreter.py` — Python reference interpreter used as a test fallback

---

## Development workflow 🔁

- Work in feature branches and open PRs against `main`.
- Keep commits small and include tests for every new feature.
- Run tests locally before pushing (Bash or PowerShell scripts are provided in `tests/`).
- Use VS Code tasks (see `.vscode/tasks.json`) to run build and test tasks quickly.

---

## Testing & CI ✅

- Tests currently include lexer, parser, interpreter (with Python fallback), and compiler smoke tests.
- I can add a GitHub Actions workflow to run tests on every push/PR — specify the matrix you want (Linux/Windows, Python versions, etc.).

---

## Roadmap & ideas 🚀

- Improve the grammar and extend the parser to support functions and scopes.
- Add more types (strings, booleans, arrays) and static checks.
- Improve the compiler to emit more robust C and support more language features.
- Add a proper REPL for quick experimentation.
- Expand test coverage and add more formalized unit tests.

---

## Credits 🇵🇭

**Owner:** The Philippines (community-maintained project)

**Developer:** [attendance1978-wq](https://github.com/attendance1978-wq) (GitHub)

**Free to use & distribute:** This project is free to use and distribute under the terms of the MIT License.

**Distributor:** The Philippines (community)

We gratefully acknowledge the contributions, feedback, and support of community members and contributors in the **Philippines**. Special thanks to local developers, testers, and community groups whose ideas and testing helped shape this project. If you'd like your name or organization listed explicitly, please open an issue or submit a PR and we'll add proper attribution.

---

## License & contact

This project uses the MIT license for source code — see `LICENSE`.

Documentation, examples, and non-code content are available under the Creative Commons Attribution 4.0 International (CC BY 4.0) license — see `CREATIVE_COMMONS.md` for details. If you prefer a different Creative Commons variant (for example CC0 or CC BY-SA), tell me and I will update the files.

If you'd like me to scaffold CI or add unit tests for a component (lexer, parser, interpreter, compiler), tell me which one and I'll add a starter workflow and test cases.

---

## Contributor ideas & Good First Issues 🧭

Here are concrete ideas, grouped by complexity, to help contributors pick useful tasks quickly.

### Good first issues (low effort)
- Add more example programs under `ph_programs/` (string handling, edge-case expressions, comments). ✅
- Fix typos and expand documentation in `docs/` and `README.md`. ✅
- Add small unit tests for the lexer: tokenization of identifiers, numbers, strings, and operators. ✅
- Add a `Makefile` or simple `ps1` wrappers to simplify common workflows. ✅

### Intermediate (small feature & tests)
- Add unary `+` and `-` support and tests for expressions. ➕
- Add boolean literals and logical operators (`and`, `or`, `not`) including lexer+parser+interpreter tests. 🧪
- Implement `break` and `continue` for loops. 🔁
- Improve string literal escaping and add test cases for special characters. "\n", "\t", etc. ✍️
- Add argument parsing to `func` calls and small function support in the AST/interpreter. 🧩

### Advanced (larger tasks)
- Add lexical scoping and block-level variable lifetimes (introduce stack frames). 🎯
- Add a proper type system (static/dynamic) and type-checker with tests. 🧠
- Implement compiler improvements (emit readable C code, support strings or runtime library). ⚙️
- Add an optimizing pass (constant folding, dead code elimination). ⚡
- Add a REPL and interactive debugging tools (step/continue/inspect). 🐞

### Testing & CI improvements
- Add unit tests for each component using a test framework or simple shell scripts (`tests/`). 🧪
- Add a GitHub Actions workflow to run tests on Linux and Windows, with a matrix for Python versions. 🔁
- Add fuzz testing or property-based tests for the lexer/parser to find edge cases. 🔍

### Documentation & UX
- Expand `docs/` with detailed grammar (EBNF), examples, and a language reference. 📚
- Add a `CONTRIBUTING.md` with PR checklist and coding conventions (line lengths, formatting). ✅
- Add code examples showing how to extend the AST and write a small pass. ✏️

### Performance & tooling
- Add micro-benchmarks for interpreter and compiler output and track regressions. 📊
- Add optional support for sanitizers (ASAN/UBSAN) in CI for detecting memory issues. 🛡️
- Create a small test harness to compare the C-compiled output vs the reference interpreter output for correctness. ✅

---

## How to propose & work on an idea
1. Open an issue describing the feature or bug and add the `good first issue` label if it's beginner-friendly. 🏷️
2. Shortlist the required changes (files to edit, tests to add, expected behavior). ✏️
3. Create a small focused branch and include tests that demonstrate the expected behavior before implementing the fix. ✅
4. Open a PR and request a review; mention `@maintainers` or add a note if you'd like a walk-through. 👥

---

If you want, I can create a `Good First Issues` checklist and add a handful of labeled issues in the repository, or scaffold a starter GitHub Actions workflow (Linux + Windows) to run tests. Which would you prefer I do next?