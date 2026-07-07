# Molex Browser Architecture

## Overview

Molex is a fork of the upstream web engine with a full Material You 3 (M3)
replacement of the user interface. All engine features are retained.

```
┌──────────────────────────────────────────┐
│            Molex UI (M3)                 │
│  ┌──────┐ ┌────────────────────────┐    │
│  │ Tabs  │ │  Omnibox (pill)      │    │
│  │ (pill)│ └────────────────────────┘    │
│  └──────┘                                │
│  ┌──────────────────────────────────┐    │
│  │         Web Content              │    │
│  │    (Blink / V8 engine)           │    │
│  └──────────────────────────────────┘    │
│  ┌──────────────────────────────────┐    │
│  │   Status Bar / M3 Bottom Sheet   │    │
│  └──────────────────────────────────┘    │
└──────────────────────────────────────────┘
```

## Directory Structure

```
molex/
├── build/                   # Build scripts
│   ├── setup.sh             # Full environment setup (Linux)
│   ├── setup_windows.ps1    # Full environment setup (Windows)
│   ├── build_linux.sh       # Linux build
│   ├── build_windows.ps1    # Windows build
│   ├── build_android.sh     # Android build
│   ├── apply_patches.sh     # Patch applier
│   └── update.sh            # Upstream update
│
├── molex_patches/           # All Molex modifications
│   ├── branding/            # Product name, logos, icons
│   ├── ui/
│   │   ├── material_you/    # Core M3 system (C++)
│   │   │   ├── m3_color_scheme.*   # Monet dynamic color
│   │   │   ├── m3_shape_tokens.*   # Corner radii
│   │   │   ├── m3_typography.*     # Type scale
│   │   │   └── m3_theme.*          # Central theme manager
│   │   ├── omnibox/         # M3 pill-shaped address bar
│   │   ├── tabs/            # M3 pill-shaped floating tabs
│   │   ├── menu/            # M3 bottom sheet / drawer menu
│   │   ├── settings/        # M3 settings page (WebUI)
│   │   ├── bookmarks/       # M3 auto-hide bookmarks bar
│   │   ├── incognito/       # M3 dark theme + mole mascot
│   │   └── downloads/       # M3 download sheet
│   ├── features/            # Feature flags
│   └── third_party/         # Material Symbols font, etc.
│
├── .github/workflows/       # CI/CD
└── docs/                    # Documentation
```

## Material You 3 Implementation

### Color System (Monet)

Uses the HCT (Hue-Chroma-Tone) color space:

1. Extract seed color from system wallpaper
2. Generate 5 tonal palettes (primary, secondary, tertiary, neutral, neutral_variant)
3. Map tones to color roles based on light/dark mode
4. Apply to all UI components

Key files:
- `m3_color_scheme.h/cc` — HCT color math, tonal palette generation
- `m3_theme.h/cc` — Central theme singleton, platform integration

### Shape System

M3 defines corner radius tokens:
- `none` (0dp), `extra_small` (4dp), `small` (8dp), `medium` (12dp),
  `large` (16dp), `extra_large` (28dp), `full` (pill)

Each component uses appropriate tokens per the M3 spec.

### Typography

Uses the M3 type scale with 15 text styles across 5 categories:
- Display, Headline, Title, Label, Body
- Font: Google Sans Text / Roboto / system sans-serif

## UI Component Patches

Each UI component has a header patch file explaining the exact changes
needed in the corresponding source files. The patches cover:

- `LocationBarView` → M3 pill-shaped omnibox
- `TabStyleViews` → M3 pill-shaped floating tabs
- `AppMenu` → M3 bottom sheet menu
- `BookmarkBarView` → M3 auto-hide bar
- `SettingsUI` → M3 settings page
- Incognito NTP → M3 dark + mole mascot

## Feature Flags

All M3 features are gated behind feature flags defined in
`molex_patches/features/molex_features.h`:
- `MolexM3Omnibox`, `MolexM3Tabs`, `MolexM3Menu`, etc.
- All enabled by default.
- All enabled by default. Can be toggled via the flags page.

## Build Integration

Patches are applied via `build/apply_patches.sh` which:
1. Copies Molex source files into the tree
2. Replaces branding strings
3. Registers GN build targets
4. Applies `.patch` files to existing sources

## Updating

When a new upstream version is released:
1. `update.sh` pulls the latest commits
2. Re-applies all Molex patches
3. Resolve any conflicts manually
