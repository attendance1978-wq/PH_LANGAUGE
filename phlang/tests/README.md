# Tests folder

This folder contains tests for components of the PHLang project.

Run lexer tests:

- On Unix-like systems (macOS / WSL / Git Bash):

    bash tests/lexer/test_lexer.sh

- On Windows PowerShell (native):

    powershell -ExecutionPolicy Bypass -File .\tests\lexer\test_lexer.ps1

Tips for Windows users: install Git for Windows (Git Bash) or MSYS2/MinGW to get a working `bash` and `gcc` in PATH. WSL without an installed distribution may show a `bash` shim that's not usable — install Git Bash or GCC if you want to run the Unix-style build/test flow.

Add more tests under `tests/` for parser, interpreter, compiler as those components are implemented. Parser tests are in `tests/parser/` and can be run via the tasks added to `.vscode/tasks.json`.

Interpreter tests now use the Python fallback `tools/ref_interpreter.py` when the C build is not available, so you can run interpreter tests without a native compiler installed.
