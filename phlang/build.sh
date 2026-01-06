#!/usr/bin/env bash
set -e

# Build the PH engine
gcc -std=c99 -Wall phlang_engine/*.c -o phlang_engine/ph_engine
printf "Built phlang_engine/ph_engine\n"

# Compile generated output if present
if [ -f output/output.c ]; then
  gcc output/output.c -o output/output
  printf "Built output/output\n"
fi

printf "Done. Run: phlang_engine/ph_engine ph_programs/hello.ph\n"