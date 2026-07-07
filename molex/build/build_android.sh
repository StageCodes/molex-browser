set -euo pipefail

_readlinkf() {
  if command -v realpath &>/dev/null; then realpath "$1"
  elif command -v greadlink &>/dev/null; then greadlink -f "$1"
  else readlink -f "$1" 2>/dev/null || (cd "$(dirname "$1")" && pwd -P)
  fi
}
SCRIPT_DIR="$(dirname "$(_readlinkf "$0")")"
MOLEX_DIR="$(dirname "$SCRIPT_DIR")"
SRC_DIR="${1:-"$MOLEX_DIR/mol/src"}"

export PATH="$HOME/depot_tools:$PATH"

if [ ! -d "$SRC_DIR" ]; then
  echo "Error: Source not found at $SRC_DIR. Run ./setup.sh first."
  exit 1
fi

cd "$SRC_DIR"

echo "==> Configuring GN for Android (arm64)..."
gn gen out/molex-android --args='
  is_official_build=true
  is_debug=false
  symbol_level=0
  target_os="android"
  target_cpu="arm64"
  proprietary_codecs=true
  ffmpeg_branding="Chrome"
  enable_widevine=true
  branding="molex"
  use_errorprone=false
'

echo "==> Building Molex for Android..."
ninja -C out/molex-android chrome_public_apk

echo "==> Build complete!"
echo "    APK: out/molex-android/apks/MolexBrowser.apk"
