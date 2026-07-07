param(
  [string]$TargetDir = "$PSScriptRoot\..\mol"
)

$ErrorActionPreference = "Stop"
$MOLEX_DIR = Split-Path -Parent $PSScriptRoot

Write-Host "=== Molex Browser — Windows Setup ===" -ForegroundColor Cyan
Write-Host "Target: $TargetDir"

Write-Host "`n[1/7] Checking prerequisites..." -ForegroundColor Yellow

$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" `
  -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
  -property installationPath 2>$null
if (-not $vsPath) {
  $vsPath = & "${env:ProgramFiles}\Microsoft Visual Studio\Installer\vswhere.exe" `
    -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath 2>$null
}
if (-not $vsPath) {
  Write-Warning "Visual Studio 2022+ not found."
}
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
  throw "Git not found."
}

Write-Host "  Visual Studio: $vsPath" -ForegroundColor Green
Write-Host "  Git:           $((git --version))" -ForegroundColor Green

Write-Host "`n[2/7] Installing depot_tools..." -ForegroundColor Yellow
$DEPOT_TOOLS = "$env:USERPROFILE\depot_tools"
if (-not (Test-Path $DEPOT_TOOLS)) {
  git clone --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git $DEPOT_TOOLS
}
$env:Path = "$DEPOT_TOOLS;$env:Path"
[Environment]::SetEnvironmentVariable("Path", "$DEPOT_TOOLS;$env:Path", "User")
$env:DEPOT_TOOLS_WIN_TOOLCHAIN = "0"
$env:Path = "$DEPOT_TOOLS;$env:Path"

Write-Host "`n[3/7] Fetching upstream source (this will take a while)..." -ForegroundColor Yellow
if (-not (Test-Path "$TargetDir")) {
  New-Item -ItemType Directory -Path "$TargetDir" -Force | Out-Null
}
Set-Location $TargetDir
fetch --nohooks chromium
if ($LASTEXITCODE -ne 0) {
  Write-Host "  Retrying with --no-history..." -ForegroundColor Yellow
  fetch --nohooks --no-history chromium
}

Write-Host "`n[4/7] Running gclient hooks..." -ForegroundColor Yellow
Set-Location "$TargetDir\src"
gclient runhooks

Write-Host "`n[5/7] Applying Molex patches..." -ForegroundColor Yellow
$SRC = "$TargetDir\src"
New-Item -ItemType Directory -Path "$SRC\molex" -Force | Out-Null
Copy-Item -Recurse "$MOLEX_DIR\molex_patches" "$SRC\molex\" -Force
New-Item -ItemType Directory -Path "$SRC\chrome\app\theme\molex" -Force | Out-Null
Copy-Item "$MOLEX_DIR\molex_patches\branding\icons\molex_logo.svg" `
  "$SRC\chrome\app\theme\molex\" -Force

if (Test-Path "$SRC\chrome\app") {
  Get-ChildItem "$SRC\chrome\app" -Filter "*exe*main*" -Recurse | ForEach-Object {
  (Get-Content $_.FullName) -replace '"Chromium"', '"Molex"' -replace '"Chrome"', '"Molex"' |
    Set-Content $_.FullName
  }
}

$ccPath = "$SRC\chrome\common\chrome_constants.cc"
if (Test-Path $ccPath) {
  (Get-Content $ccPath) -replace 'CHROMIUM', 'PRODUCT_NAME' -replace '"Chromium"', '"Molex"' |
    Set-Content $ccPath
}

$viPath = "$SRC\components\version_info\version_info.cc"
if (Test-Path $viPath) {
  (Get-Content $viPath) -replace '"Chromium"', '"Molex"' | Set-Content $viPath
}

Add-Content "$SRC\BUILD.gn" @"

group("molex") {
  deps = [
    "//molex/molex_patches/ui/material_you:m3_theme",
    "//molex/molex_patches/features:molex_features",
  ]
}
"@

Write-Host "`n[6/7] Generating GN build directory..." -ForegroundColor Yellow
$GN_ARGS = @'
is_official_build=true
is_debug=false
symbol_level=0
proprietary_codecs=true
ffmpeg_branding="Chrome"
enable_widevine=true
branding="molex"
'@
gn gen out/molex --args="$GN_ARGS"
if ($LASTEXITCODE -ne 0) {
  gn gen out/molex --args='branding="molex"'
}

Write-Host "`n=== Setup complete! ===" -ForegroundColor Green
Write-Host "To build:"
Write-Host "  cd $SRC"
Write-Host "  ninja -C out/molex chrome"
Write-Host "  Binary: out\molex\chrome.exe"
