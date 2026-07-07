param(
  [string]$TargetDir = "$PSScriptRoot\..\mol\src",
  [string]$GNExtra = ""
)

$ErrorActionPreference = "Stop"
$MOLEX_DIR = Split-Path -Parent $PSScriptRoot

$DEPOT_TOOLS = "$env:USERPROFILE\depot_tools"
if (Test-Path $DEPOT_TOOLS) {
  $env:Path = "$DEPOT_TOOLS;$env:Path"
}
$env:DEPOT_TOOLS_WIN_TOOLCHAIN = "0"

if (-not (Test-Path "$TargetDir\BUILD.gn")) {
  Write-Error "Source not found at $TargetDir. Run setup_windows.ps1 first."
  exit 1
}

Set-Location $TargetDir

$GN_BASE = @'
is_official_build=true
is_debug=false
symbol_level=0
proprietary_codecs=true
ffmpeg_branding="Chrome"
enable_widevine=true
branding="molex"
enable_platform_hevc=true
enable_hangout_services_extension=true
enable_remoting=true
enable_extensions=true
'@
if ($GNExtra) { $GN_BASE += "`n$GNExtra" }

Write-Host "==> Configuring GN..." -ForegroundColor Yellow
gn gen out/molex --args="$GN_BASE"

Write-Host "==> Building Molex browser (1-6 hours)..." -ForegroundColor Yellow
Write-Host "  Use: ninja -C out/molex chrome -j N" -ForegroundColor Gray

ninja -C out/molex chrome

if ($LASTEXITCODE -eq 0) {
  Write-Host "`n=== Build complete! ===" -ForegroundColor Green
  Write-Host "Binary: $TargetDir\out\molex\chrome.exe"
  Write-Host "Size:   $((Get-Item "$TargetDir\out\molex\chrome.exe").Length / 1MB) MB"
} else {
  Write-Error "Build failed."
}
