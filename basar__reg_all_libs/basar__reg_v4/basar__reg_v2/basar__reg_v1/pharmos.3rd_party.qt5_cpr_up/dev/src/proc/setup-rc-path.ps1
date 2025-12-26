# setup-rc-path.ps1

$kitsBinRoot = "C:\Program Files (x86)\Windows Kits\10\bin"

# 1. Check if rc.exe already in PATH
$rcExists = Get-Command rc.exe -ErrorAction SilentlyContinue

if ($rcExists) {
    # Already found in PATH, just return the current PATH line
    Write-Output "REM rc.exe already in PATH"
    Write-Output "set PATH=$env:PATH"
    exit 0
}

# 2. Try non-versioned path first
$rcPathFallback = Join-Path $kitsBinRoot "x64\rc.exe"
if (Test-Path $rcPathFallback) {
    $newPath = Join-Path $kitsBinRoot "x64"
    Write-Output "set PATH=$newPath;$env:PATH"
    exit 0
}

# 3. Try versioned paths
$versionedPaths = Get-ChildItem -Directory "$kitsBinRoot" | Where-Object {
    $_.Name -match '^\d+\.\d+'
} | Sort-Object Name -Descending

foreach ($dir in $versionedPaths) {
    $candidate = Join-Path $dir.FullName "x64\rc.exe"
    if (Test-Path $candidate) {
        $newPath = Join-Path $dir.FullName "x64"
        Write-Output "set PATH=$newPath;$env:PATH"
        exit 0
    }
}

Write-Output "echo rc.exe not found in any known path."
exit 1
