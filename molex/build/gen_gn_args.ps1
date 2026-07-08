param([string]$OutDir = "out/molex")
$null = New-Item -ItemType Directory -Path $OutDir -Force
$lines = @(
  "is_official_build = true",
  "is_debug = false",
  "symbol_level = 0",
  "proprietary_codecs = true",
  'ffmpeg_branding = "Chrome"',
  "enable_widevine = true",
  'branding = "molex"'
)
$lines | Set-Content -Path "$OutDir\args.gn" -Encoding utf8
