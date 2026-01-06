# build.ps1 — Cross-platform build helper for Windows PowerShell
$ErrorActionPreference = 'Stop'

# If gcc is available, compile directly
if (Get-Command gcc -ErrorAction SilentlyContinue) {
    gcc -std=c99 -Wall phlang_engine\*.c -o phlang_engine\ph_engine
    Write-Output "Built phlang_engine\ph_engine"
    if (Test-Path output\output.c) {
        gcc output\output.c -o output\output
        Write-Output "Built output\output"
    }
    exit 0
}

# If bash is available, test whether it's usable (avoid WSL shim with no distro)
$bashCmd = Get-Command bash -ErrorAction SilentlyContinue
if ($bashCmd) {
    try {
        $test = & bash -c "echo OK" 2>&1
        if ($test -eq "OK") {
            bash build.sh
            exit $LASTEXITCODE
        } else {
            Write-Error "Found 'bash' but it's not usable (WSL may have no distributions). Install Git Bash or configure a working bash, or install GCC directly."
            exit 1
        }
    } catch {
        Write-Error "Found 'bash' but unable to run commands: $_. Install Git Bash or configure a working bash, or install GCC directly."
        exit 1
    }
}

Write-Error "Could not find 'gcc' or usable 'bash'. Please install GCC (MinGW/MSYS2) or Git Bash and ensure it's on PATH."
exit 1
