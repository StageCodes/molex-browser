set -euo pipefail

_readlinkf() {
  if command -v realpath &>/dev/null; then realpath "$1"
  elif command -v greadlink &>/dev/null; then greadlink -f "$1"
  else readlink -f "$1" 2>/dev/null || (cd "$(dirname "$1")" && pwd -P)
  fi
}
MOLEX_DIR="$(dirname "$(dirname "$(_readlinkf "$0")")")"
SRC_DIR="${1:?Usage: $0 /path/to/upstream/src}"

if [ ! -d "$SRC_DIR" ]; then
  echo "Error: $SRC_DIR does not exist"
  exit 1
fi

echo "==> Applying Molex patches at $SRC_DIR"

mkdir -p "$SRC_DIR/molex"
cp -r "$MOLEX_DIR/molex_patches" "$SRC_DIR/molex/"

mkdir -p "$SRC_DIR/chrome/app/theme/molex"
cp "$MOLEX_DIR/molex_patches/branding/icons/molex_logo.svg" \
   "$SRC_DIR/chrome/app/theme/molex/"

for size in 16 32 48 64 128 256; do
  if command -v rsvg-convert &>/dev/null; then
    rsvg-convert -w "$size" -h "$size" \
      "$MOLEX_DIR/molex_patches/branding/icons/molex_logo.svg" \
      > "$SRC_DIR/chrome/app/theme/molex/logo_${size}.png"
    rsvg-convert -w "$((size*2))" -h "$((size*2))" \
      "$MOLEX_DIR/molex_patches/branding/icons/molex_logo.svg" \
      > "$SRC_DIR/chrome/app/theme/molex/logo_${size}@2x.png"
  fi
done

echo "==> Patching product name constants..."
if [[ "$OSTYPE" == "darwin"* ]]; then
  SED_INPLACE=(sed -i '')
else
  SED_INPLACE=(sed -i)
fi

if [ -d "$SRC_DIR/chrome/app" ]; then
  find "$SRC_DIR/chrome/app" -name "*exe*main*" -exec "${SED_INPLACE[@]}" \
    's/"Chromium"/"Molex"/g; s/"Chrome"/"Molex"/g' {} +
fi

"${SED_INPLACE[@]}" \
  's/CHROMIUM/PRODUCT_NAME/g; s/"Chromium"/"Molex"/g' \
  "$SRC_DIR/chrome/common/chrome_constants.cc" 2>/dev/null || true

"${SED_INPLACE[@]}" \
  's/"Chromium"/"Molex"/g' \
  "$SRC_DIR/components/version_info/version_info.cc" 2>/dev/null || true

echo "==> Adding Molex build targets..."
cat >> "$SRC_DIR/BUILD.gn" << EOF

group("molex") {
  deps = [
    "//molex/molex_patches/ui/material_you:m3_theme",
    "//molex/molex_patches/features:molex_features",
  ]
}
EOF

echo "==> Applying UI patches..."
PATCH_DIR="$MOLEX_DIR/molex_patches/patches"
if [ -d "$PATCH_DIR" ]; then
  for patch in "$PATCH_DIR"/*.patch; do
    if [ -f "$patch" ]; then
      echo "  Applying $(basename "$patch")"
      patch -p1 -d "$SRC_DIR" < "$patch" || echo "  Warning: patch may have conflicts"
    fi
  done
fi

echo "==> Installing Material Symbols font..."
mkdir -p "$SRC_DIR/third_party/material_symbols"
cp -r "$MOLEX_DIR/molex_patches/third_party/material_symbols/"* \
   "$SRC_DIR/third_party/material_symbols/" 2>/dev/null || true


echo "==> Done! Molex patches applied."
echo "    Next steps:"
echo "      1. cd $SRC_DIR && gn gen out/molex --args='branding=\"molex\"'"
echo "      2. ninja -C out/molex chrome"
