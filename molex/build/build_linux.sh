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
GN_EXTRA="${2:-}"

export PATH="$HOME/depot_tools:$PATH"

if [ ! -d "$SRC_DIR" ]; then
  echo "Error: Source not found at $SRC_DIR. Run ./setup.sh first."
  exit 1
fi

cd "$SRC_DIR"

GN_ARGS='
  is_official_build=true
  is_debug=false
  symbol_level=0
  use_system_xcode=false
  proprietary_codecs=true
  ffmpeg_branding="Chrome"
  enable_widevine=true
  branding="molex"
  use_sysroot=false
  use_udev=true
  use_vaapi=true
  enable_platform_hevc=true
  enable_platform_ac3_eac3=true
  enable_platform_mpeg_h_audio=true
  enable_platform_dolby_vision=true
  enable_hangout_services_extension=true
  enable_remoting=true
  enable_reading_list=true
  enable_service_discovery=true
  enable_extensions=true
'
if [ -n "$GN_EXTRA" ]; then
  GN_ARGS="$GN_ARGS $GN_EXTRA"
fi

echo "==> Configuring GN for Linux..."
gn gen out/molex --args="$GN_ARGS"

if [ ! -f "molex/.patches_applied" ]; then
  bash "$SCRIPT_DIR/apply_patches.sh" "$SRC_DIR"
  touch "molex/.patches_applied"
fi

echo "==> Building Molex browser..."
ninja -C out/molex chrome

echo "==> Build complete!"
echo "    Binary: out/molex/chrome"

if command -v dpkg-deb &>/dev/null; then
  echo "==> Creating .deb package..."
  mkdir -p out/molex/pkg/DEBIAN
  mkdir -p out/molex/pkg/usr/bin
  mkdir -p out/molex/pkg/usr/share/applications
  mkdir -p out/molex/pkg/usr/share/icons/hicolor/256x256/apps

  cp out/molex/chrome out/molex/pkg/usr/bin/molex-browser

  cat > out/molex/pkg/DEBIAN/control << EOF
Package: molex-browser
Version: 1.0.0
Section: web
Priority: optional
Architecture: amd64
Maintainer: Molex Team
Description: Molex Browser - Material You 3 browser
EOF

  cat > out/molex/pkg/usr/share/applications/molex-browser.desktop << EOF
[Desktop Entry]
Version=1.0
Name=Molex Browser
Comment=Material You 3 browser
Exec=molex-browser %U
Icon=molex-browser
Terminal=false
Type=Application
Categories=Network;WebBrowser;
MimeType=text/html;text/xml;application/xhtml+xml;
EOF

  cp "$MOLEX_DIR/molex_patches/branding/icons/molex_logo.svg" \
     out/molex/pkg/usr/share/icons/hicolor/256x256/apps/molex-browser.svg

  dpkg-deb --build out/molex/pkg out/molex/molex-browser-amd64.deb
  echo "    Package: out/molex/molex-browser-amd64.deb"
fi
