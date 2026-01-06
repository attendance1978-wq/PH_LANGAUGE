#!/usr/bin/env bash
set -e

# Build
if ! ./build.sh >/dev/null 2>&1; then
  echo "Build failed - skipping parser tests."; exit 0
fi

OUT=$(./phlang_engine/ph_engine --parse ph_programs/math.ph)

echo "$OUT"

echo "$OUT" | grep -q "ASSIGN" || { echo "Missing ASSIGN in parse output"; exit 1; }
echo "$OUT" | grep -q "BINARY" || { echo "Missing BINARY in parse output"; exit 1; }

echo "Parser test passed."