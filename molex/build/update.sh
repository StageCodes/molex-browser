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
  echo "Error: Upstream source not found at $SRC_DIR"
  echo "Run ./setup.sh first."
  exit 1
fi

cd "$SRC_DIR"

echo "==> Stashing Molex changes..."
git stash push -m "molex-patches" 2>/dev/null || true

echo "==> Fetching latest upstream..."
git checkout main
git pull origin main
gclient sync --with_branch_heads

echo "==> Re-applying Molex patches..."
git stash pop 2>/dev/null || true
bash "$SCRIPT_DIR/apply_patches.sh" "$SRC_DIR"

echo "==> Update complete! Current revision:"
git rev-parse --short HEAD

echo "==> Build with: ninja -C out/molex chrome"
