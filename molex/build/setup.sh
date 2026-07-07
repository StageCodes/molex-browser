set -euo pipefail

_readlinkf() {
  if command -v realpath &>/dev/null; then realpath "$1"
  elif command -v greadlink &>/dev/null; then greadlink -f "$1"
  else readlink -f "$1" 2>/dev/null || (cd "$(dirname "$1")" && pwd -P)
  fi
}
SCRIPT_DIR="$(dirname "$(_readlinkf "$0")")"
MOLEX_DIR="$(dirname "$SCRIPT_DIR")"
SRC_DIR="${1:-"$MOLEX_DIR/mol"}"

echo "==> Molex Browser — Setup"
echo "    Target: $SRC_DIR"

echo "==> Installing build dependencies..."
if command -v pacman &>/dev/null; then
  sudo pacman -Syu --noconfirm \
    base-devel git python3 python-pip clang lld curl wget file tar xz \
    glib2 gtk3 pango cairo gdk-pixbuf2 dbus libx11 libxcb libxcomposite \
    libxdamage libxext libxfixes libxi libxrandr libxrender libxscrnsaver \
    libxtst libdrm mesa alsa-lib libpulse nss nspr atk at-spi2-atk \
    at-spi2-core gtk-update-icon-cache rsvg-convert nodejs npm librsvg \
    || echo "Some packages may have failed (check individually)"
elif command -v apt &>/dev/null; then
  sudo apt update && sudo apt install -y \
    git python3 python3-pip clang lld curl wget nodejs npm \
    libglib2.0-dev libgtk-3-dev libpango1.0-dev libcairo2-dev \
    libdbus-1-dev libx11-xcb-dev libxcomposite-dev libxdamage-dev \
    libxext-dev libxfixes-dev libxi-dev libxrandr-dev libxrender-dev \
    libxscrnsaver-dev libxtst-dev libdrm-dev libpulse-dev libnss3-dev \
    libnspr4-dev libasound2-dev libatk1.0-dev libatspi2.0-dev librsvg2-bin
fi

echo "==> Installing depot_tools..."
DEPOT_TOOLS_DIR="$HOME/depot_tools"
if [ ! -d "$DEPOT_TOOLS_DIR" ]; then
  git clone --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git "$DEPOT_TOOLS_DIR"
fi
export PATH="$DEPOT_TOOLS_DIR:$PATH"
if ! grep -q "depot_tools" ~/.bashrc 2>/dev/null; then
  echo "export PATH=\"\$HOME/depot_tools:\$PATH\"" >> ~/.bashrc
fi

echo "==> Fetching upstream source (this will take a while)..."
mkdir -p "$SRC_DIR"
cd "$SRC_DIR"
fetch --nohooks chromium

echo "==> Running install-build-deps..."
cd "$SRC_DIR/src"
./build/install-build-deps.sh --no-prompt --no-arm --no-chromeos-fonts \
  || echo "install-build-deps may have warnings (safe to ignore)"

echo "==> Running gclient hooks..."
gclient runhooks

echo "==> Applying Molex patches..."
bash "$SCRIPT_DIR/apply_patches.sh" "$SRC_DIR/src"

echo "==> Generating build directory..."
gn gen out/molex --args='
  is_official_build=true
  is_debug=false
  symbol_level=0
  use_system_xcode=false
  proprietary_codecs=true
  ffmpeg_branding="Chrome"
  enable_widevine=true
  branding="molex"
'

echo ""
echo "============================================"
echo "  Setup complete!"
echo "  To build: cd $SRC_DIR/src && ninja -C out/molex chrome"
echo "  Binary: out/molex/chrome"
echo "============================================"
