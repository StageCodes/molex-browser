param([string]$OutDir = "out/molex")
$null = New-Item -ItemType Directory -Path $OutDir -Force
$lines = @(
  "is_official_build = false",
  "is_debug = false",
  "symbol_level = 0",
  "blink_symbol_level = 0",
  "optimize_for_size = true",
  "proprietary_codecs = true",
  "chrome_pgo_phase = 0",
  'ffmpeg_branding = "Chrome"',
  "enable_widevine = true",
  'branding = "molex"'
)
$lines | Set-Content -Path "$OutDir\args.gn" -Encoding utf8
