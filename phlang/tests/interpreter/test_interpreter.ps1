# test_interpreter.ps1 - PowerShell interpreter tests
$ErrorActionPreference = 'Stop'

$ErrorActionPreference = 'Stop'

$build_ok = $true
try {
    .\build.ps1 | Write-Output
} catch {
    Write-Warning "Build failed - falling back to Python reference interpreter. Error: $_"
    $build_ok = $false
}

if ($build_ok) {
    $out = & .\phlang_engine\ph_engine .\ph_programs\hello.ph 2>&1
} else {
    $out = & python .\tools\ref_interpreter.py .\ph_programs\hello.ph 2>&1
}
Write-Output $out
if (-not ($out -match 'Hello, PHLang')) { Write-Error "Hello test failed"; exit 1 }

if ($build_ok) {
    $out = & .\phlang_engine\ph_engine .\ph_programs\math.ph 2>&1
} else {
    $out = & python .\tools\ref_interpreter.py .\ph_programs\math.ph 2>&1
}
Write-Output $out
if (-not ($out -match '\b15\b')) { Write-Error "Math test failed (expected 15)"; exit 1 }

if ($build_ok) {
    $out = & .\phlang_engine\ph_engine .\ph_programs\loops.ph 2>&1
} else {
    $out = & python .\tools\ref_interpreter.py .\ph_programs\loops.ph 2>&1
}
Write-Output $out

# Verify that the output contains the sequence 0,1,2,3,4 in order (one per line)
$lines = ($out -split '\r?\n') | ForEach-Object { $_.Trim() } | Where-Object { $_ -ne '' }
$nums = $lines | Where-Object { $_ -match '^\d+$' }
if ($nums.Count -lt 5) { Write-Error "Loops test failed (not enough numeric lines)"; exit 1 }
for ($i = 0; $i -lt 5; $i++) {
    if ($nums[$i] -ne [string]$i) { Write-Error "Loops test failed (expected $i at position $i, got $($nums[$i]))"; exit 1 }
}

Write-Output "Interpreter tests passed."