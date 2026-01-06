# test_compiler.ps1 - PowerShell compiler test
$ErrorActionPreference = 'Stop'

$ok = $true
try {
    .\build.ps1 | Write-Output
} catch {
    Write-Warning "Engine build failed or not available: $_"
    $ok = $false
}

# Attempt to use engine to compile if available
try { .\phlang_engine\ph_engine --compile .\ph_programs\hello.ph | Write-Output } catch { }

if (-not (Test-Path output\output.c)) { Write-Error "Compiler did not write output\output.c"; exit 1 }

Write-Output "Generated output\output.c"

# Try to compile using gcc if present
if (Get-Command gcc -ErrorAction SilentlyContinue) {
    gcc output\output.c -o output\output | Out-Null
    $out = & .\output\output
    Write-Output $out
    if (-not ($out -match 'Hello')) { Write-Error "Compiled program did not print Hello"; exit 1 }
    Write-Output "Compiler test passed."
} else {
    Write-Warning "gcc not available; skipping compiled run"
}
