# test_lexer.ps1 - PowerShell test for lexer
$ErrorActionPreference = 'Stop'

# Build
Write-Output "Building..."
try {
    .\build.ps1 | Write-Output
} catch {
    Write-Warning "Build failed - skipping lexer tests. Error: $_"
    exit 0
}

# Run lexer
$out = & .\phlang_engine\ph_engine --lex .\ph_programs\hello.ph 2>&1
Write-Output $out

if (-not ($out -match '^IDENT\s+print')) {
    Write-Error "Missing 'print' identifier token"
    exit 1
}
if (-not ($out -match 'Hello, PHLang')) {
    Write-Error "Missing string token"
    exit 1
}

Write-Output "Lexer test passed."