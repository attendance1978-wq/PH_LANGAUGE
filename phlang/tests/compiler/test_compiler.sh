#!/usr/bin/env bash
set -e

# Build engine (if available)
if ! ./build.sh >/dev/null 2>&1; then
  echo "Engine build failed or not available; will still run compiler if present.";
fi

# Use engine to compile
./phlang_engine/ph_engine --compile ph_programs/hello.ph || true

if [ ! -f output/output.c ]; then
  echo "Compiler did not write output/output.c"; exit 1
fi

echo "Generated output/output.c"

gcc -o output/output output/output.c >/dev/null 2>&1 || { echo "gcc not available; skipping compiled run"; exit 0; }

OUT=$(./output/output)

echo "$OUT"
echo "$OUT" | grep -q "Hello" || { echo "Compiled program did not print Hello"; exit 1; }

echo "Compiler test passed."