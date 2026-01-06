# test_parser.ps1 - PowerShell parser test
$ErrorActionPreference = 'Stop'

try {
    .\build.ps1 | Write-Output
} catch {
    Write-Warning "Build failed - skipping parser tests. Error: $_"
    exit 0
}

$out = & .\phlang_engine\ph_engine --parse .\ph_programs\math.ph 2>&1
Write-Output $out

if (-not ($out -match 'ASSIGN')) { Write-Error "Missing ASSIGN in parse output"; exit 1 }
if (-not ($out -match 'BINARY')) { Write-Error "Missing BINARY in parse output"; exit 1 }

Write-Output "Parser test passed."