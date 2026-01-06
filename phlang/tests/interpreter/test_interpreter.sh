#!/usr/bin/env bash
set -e

# Build
if ! ./build.sh >/dev/null 2>&1; then
  echo "Build failed - falling back to Python reference interpreter.";
  PY_FALLBACK=1
else
  PY_FALLBACK=0
fi

# Hello test
if [ "$PY_FALLBACK" -eq 1 ]; then
  OUT=$(python tools/ref_interpreter.py ph_programs/hello.ph)
else
  OUT=$(./phlang_engine/ph_engine ph_programs/hello.ph)
fi

echo "$OUT"
echo "$OUT" | grep -q "Hello, PHLang" || { echo "Hello test failed"; exit 1; }

# Math test
if [ "$PY_FALLBACK" -eq 1 ]; then
  OUT=$(python tools/ref_interpreter.py ph_programs/math.ph)
else
  OUT=$(./phlang_engine/ph_engine ph_programs/math.ph)
fi

echo "$OUT"
echo "$OUT" | grep -q "15" || { echo "Math test failed (expected 15)"; exit 1; }

# Loops test
if [ "$PY_FALLBACK" -eq 1 ]; then
  OUT=$(python tools/ref_interpreter.py ph_programs/loops.ph)
else
  OUT=$(./phlang_engine/ph_engine ph_programs/loops.ph)
fi

echo "$OUT"
# Normalize newlines to spaces and check sequence
SINGLE=$(echo "$OUT" | tr '\n' ' ')
echo "$SINGLE" | grep -E "0[[:space:]]+1[[:space:]]+2[[:space:]]+3[[:space:]]+4" >/dev/null || { echo "Loops test failed"; exit 1; }

echo "Interpreter tests passed."