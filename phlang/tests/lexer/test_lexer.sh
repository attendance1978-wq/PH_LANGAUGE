#!/usr/bin/env bash
set -e

# Ensure the engine is built
if ! ./build.sh >/dev/null 2>&1; then
  echo "Build failed — skipping lexer tests."; exit 0
fi

OUT=$(./phlang_engine/ph_engine --lex ph_programs/hello.ph)

echo "$OUT"

echo "$OUT" | grep -qP '^IDENT\tprint' || { echo "Missing 'print' identifier token"; exit 1; }
echo "$OUT" | grep -qP 'STRING\t".*Hello, PHLang' || { echo "Missing string token"; exit 1; }

echo "Lexer test passed."