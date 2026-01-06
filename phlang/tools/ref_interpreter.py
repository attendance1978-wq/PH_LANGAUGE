#!/usr/bin/env python3
"""Reference PH interpreter (Python fallback) - supports minimal syntax for tests:
- print STRING_OR_EXPR;
- assignment: IDENT = EXPR;
- while (COND) { ... }
Expressions: uses Python evaluation with restricted globals to support numbers, + - * / and comparisons.
"""
import sys
import re

RE_PRINT = re.compile(r'^\s*print\s+(.*);\s*$')
RE_ASSIGN = re.compile(r'^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(.*);\s*$')
RE_WHILE = re.compile(r'^\s*while\s*\((.*)\)\s*{\s*$')
RE_CLOSE = re.compile(r'^\s*}\s*$')

SAFE_NAMES = {}

class Env(dict):
    pass


def safe_eval(expr, env):
    # Allow only a subset of builtins/operators by restricting globals
    # Convert 'print' usage not allowed here. Strings keep quotes.
    # Replace identifier names with env[...] lookups by using eval with env as locals.
    try:
        # use Python's eval with env as locals; no builtins
        return eval(expr, {"__builtins__": None}, env)
    except Exception:
        return None


def run_file(path):
    with open(path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    env = Env()
    i = 0
    out_lines = []

    def eval_expr(s):
        s = s.strip()
        # If string literal in quotes, return it without quotes
        if (s.startswith('"') and s.endswith('"')) or (s.startswith("'") and s.endswith("'")):
            return s[1:-1]
        val = safe_eval(s, env)
        return val

    def exec_block(start_idx):
        nonlocal i
        # start_idx points after the while( ... ) { line
        body = []
        depth = 1
        j = start_idx
        while j < len(lines):
            line = lines[j]
            if RE_WHILE.match(line):
                depth += 1
            if RE_CLOSE.match(line):
                depth -= 1
                if depth == 0:
                    return body, j+1
            body.append(line)
            j += 1
        return body, j

    while i < len(lines):
        line = lines[i].rstrip('\n')
        if not line.strip() or line.strip().startswith('#'):
            i += 1
            continue
        m = RE_PRINT.match(line)
        if m:
            expr = m.group(1).strip()
            v = eval_expr(expr)
            # print integers without decimal when possible
            if isinstance(v, float):
                if v.is_integer():
                    out_lines.append(str(int(v)))
                else:
                    out_lines.append(str(v))
            else:
                out_lines.append(str(v))
            i += 1
            continue
        m = RE_ASSIGN.match(line)
        if m:
            name = m.group(1)
            expr = m.group(2).strip()
            v = eval_expr(expr)
            if isinstance(v, str):
                env[name] = v
            else:
                try:
                    env[name] = float(v)
                except Exception:
                    env[name] = v
            i += 1
            continue
        m = RE_WHILE.match(line)
        if m:
            cond_expr = m.group(1).strip()
            body, after = exec_block(i+1)
            # execute loop
            while True:
                cond_val = eval_expr(cond_expr)
                cond_truth = False
                if isinstance(cond_val, (int, float)):
                    cond_truth = (cond_val != 0)
                else:
                    cond_truth = bool(cond_val)
                if not cond_truth:
                    break
                # execute body lines
                k = 0
                while k < len(body):
                    l = body[k]
                    # recursively process body lines using a small dispatcher
                    m2 = RE_PRINT.match(l)
                    if m2:
                            expr = m2.group(1).strip()
                            v = eval_expr(expr)
                            # print integers without decimal when possible
                            if isinstance(v, float):
                                if v.is_integer():
                                    out_lines.append(str(int(v)))
                                else:
                                    out_lines.append(str(v))
                            else:
                                out_lines.append(str(v))
                            k += 1
                            continue
                    m2 = RE_ASSIGN.match(l)
                    if m2:
                        name = m2.group(1)
                        expr = m2.group(2).strip()
                        v = eval_expr(expr)
                        if isinstance(v, str): env[name] = v
                        else:
                            try: env[name] = float(v)
                            except: env[name] = v
                        k += 1
                        continue
                    # nested while not supported in body for simplicity
                    k += 1
            i = after
            continue
        # fallback: ignore
        i += 1
    return '\n'.join(out_lines)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: ref_interpreter.py file.ph')
        sys.exit(1)
    out = run_file(sys.argv[1])
    if out:
        print(out)
