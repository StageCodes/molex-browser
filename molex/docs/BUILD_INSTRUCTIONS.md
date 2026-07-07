# Building Molex Browser

## System Requirements

| Component   | Minimum              | Recommended         |
|------------|----------------------|---------------------|
| RAM        | 16 GB                | 32 GB               |
| CPU        | 8 cores              | 16+ cores           |
| Disk       | 80 GB free           | 150 GB free (SSD)   |
| OS         | Linux, Windows 10+,  | Linux, Windows 11,  |
|            | macOS 13+            | macOS 14+           |

## Quick Start (Linux)

```bash
git clone https://github.com/your-org/molex-browser.git
cd molex-browser
./build/setup.sh
cd mol/src
ninja -C out/molex chrome
./out/molex/chrome
```

## Step-by-Step

### 1. Install depot_tools

```bash
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git ~/depot_tools
export PATH="$HOME/depot_tools:$PATH"
```

### 2. Fetch upstream source

```bash
mkdir ~/mol && cd ~/mol
fetch --nohooks chromium
cd src
./build/install-build-deps.sh
gclient runhooks
```

### 3. Apply Molex patches

```bash
cd ~/mol/src
bash /path/to/molex/build/apply_patches.sh "$PWD"
```

### 4. Configure GN

```bash
gn gen out/molex --args='
  is_official_build=true
  is_debug=false
  symbol_level=0
  proprietary_codecs=true
  ffmpeg_branding="Chrome"
  enable_widevine=true
  branding="molex"
  use_sysroot=false
'
```

### 5. Build

```bash
ninja -C out/molex chrome
```

Binary: `out/molex/chrome`

## Build Flags

| Flag                | Description                          |
|---------------------|--------------------------------------|
| `is_official_build` | Optimized release build              |
| `proprietary_codecs`| H.264, AAC, MP3, HEVC, etc.         |
| `enable_widevine`   | DRM support (Netflix, Spotify, etc.) |
| `branding`          | "molex" for custom branding          |

## Android Build

```bash
fetch --nohooks android
cd src
./build/install-build-deps-android.sh
gn gen out/molex-android --args='
  target_os="android"
  target_cpu="arm64"
  is_official_build=true
  branding="molex"
'
ninja -C out/molex-android chrome_public_apk
```

## macOS Build

```bash
gn gen out/molex --args='
  is_official_build=true
  proprietary_codecs=true
  ffmpeg_branding="Chrome"
  enable_widevine=true
  branding="molex"
'
ninja -C out/molex chrome
```

## Windows Build (10/11)

### Requirements
- Visual Studio 2022+ — "Desktop C++" workload
- Windows 10 SDK
- Git for Windows
- Python 3.10+

### Quick Start (PowerShell 7+)

```powershell
git clone https://github.com/your-org/molex-browser.git
cd molex-browser
.\build\setup_windows.ps1
cd mol\src
ninja -C out\molex chrome
.\out\molex\chrome.exe
```

### Step-by-Step

1. **Install depot_tools:**
   ```powershell
   git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git $env:USERPROFILE\depot_tools
   $env:Path = "$env:USERPROFILE\depot_tools;$env:Path"
   $env:DEPOT_TOOLS_WIN_TOOLCHAIN = 0
   ```

2. **Fetch upstream:**
   ```powershell
   mkdir C:\mol -Force; cd C:\mol
   fetch --nohooks chromium
   ```

3. **Apply Molex patches:**
   ```powershell
   Copy-Item -Recurse .\molex_patches C:\mol\src\molex\
   ```

4. **Configure and build:**
   ```powershell
   cd C:\mol\src
   gn gen out/molex --args="is_official_build=true proprietary_codecs=true ffmpeg_branding=`"Chrome`" enable_widevine=true branding=`"molex`""
   ninja -C out/molex chrome
   ```

Binary: `out\molex\chrome.exe`

### Troubleshooting (Windows)

- **`gn` not found**: Ensure depot_tools is in PATH. Reopen PowerShell.
- **VS not detected**: Run from "Developer Command Prompt for VS 2022".
- **Out of memory**: `ninja -j2 -C out/molex chrome`
- **Path too long**: `git config --system core.longpaths true`

## Updating

```bash
cd molex-browser
./build/update.sh
ninja -C mol/src/out/molex chrome
```

## Troubleshooting

- **Out of memory**: `ninja -j2` to limit parallel jobs.
- **Missing dependencies**: Run `./build/install-build-deps.sh --no-prompt`.
- **Patch conflicts**: Check `build/apply_patches.sh` output.
