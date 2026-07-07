#include "molex/molex_patches/ui/material_you/m3_color_scheme.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>

#include "third_party/skia/include/core/SkColor.h"

namespace molex::m3 {

namespace {

float SrgbToLinear(float v) {
  if (v <= 0.04045f) return v / 12.92f;
  return std::pow((v + 0.055f) / 1.055f, 2.4f);
}

float LinearToSrgb(float v) {
  if (v <= 0.0031308f) return 12.92f * v;
  return 1.055f * std::pow(v, 1.0f / 2.4f) - 0.055f;
}

struct HctColor {
  float hue;
  float chroma;

  float tone;
};

struct XyzColor {
  float x, y, z;
};

XyzColor LinearRgbToXyz(float r, float g, float b) {
  return {
    .x = 0.4124564f * r + 0.3575761f * g + 0.1804375f * b,
    .y = 0.2126729f * r + 0.7151522f * g + 0.0721750f * b,
    .z = 0.0193339f * r + 0.1191920f * g + 0.9503041f * b,
  };
}

void XyzToLinearRgb(const XyzColor& xyz, float& r, float& g, float& b) {
  r =  3.2404542f * xyz.x - 1.5371385f * xyz.y - 0.4985314f * xyz.z;
  g = -0.9692660f * xyz.x + 1.8760108f * xyz.y + 0.0415560f * xyz.z;
  b =  0.0556434f * xyz.x - 0.2040259f * xyz.y + 1.0572252f * xyz.z;
}

struct LabColor {
  float l, a, b;
};

LabColor XyzToLab(const XyzColor& xyz) {
  auto f = [](float t) {
    if (t > 0.008856f) return std::pow(t, 1.0f / 3.0f);
    return 7.787f * t + 16.0f / 116.0f;
  };
  return {
    .l = 116.0f * f(xyz.y / 1.0f) - 16.0f,
    .a = 500.0f * (f(xyz.x / 0.95047f) - f(xyz.y / 1.0f)),
    .b = 200.0f * (f(xyz.y / 1.0f) - f(xyz.z / 1.08883f)),
  };
}

XyzColor LabToXyz(const LabColor& lab) {
  auto f_inv = [](float t) {
    if (t > 0.2069f) return t * t * t;
    return 0.12842f * (t - 16.0f / 116.0f);
  };
  return {
    .x = 0.95047f * f_inv((lab.l + 16.0f) / 116.0f + lab.a / 500.0f),
    .y = 1.0f * f_inv((lab.l + 16.0f) / 116.0f),
    .z = 1.08883f * f_inv((lab.l + 16.0f) / 116.0f - lab.b / 200.0f),
  };
}

HctColor ArgbToHct(SkColor argb) {
  float r = SrgbToLinear(SkColorGetR(argb) / 255.0f);
  float g = SrgbToLinear(SkColorGetG(argb) / 255.0f);
  float b = SrgbToLinear(SkColorGetB(argb) / 255.0f);
  auto xyz = LinearRgbToXyz(r, g, b);
  auto lab = XyzToLab(xyz);
  float hue = std::atan2(lab.b, lab.a) * 180.0f / 3.14159265f;
  if (hue < 0) hue += 360.0f;
  float chroma = std::sqrt(lab.a * lab.a + lab.b * lab.b);
  return {.hue = hue, .chroma = chroma, .tone = lab.l};
}

SkColor HctToArgb(const HctColor& hct) {
  float hue_rad = hct.hue * 3.14159265f / 180.0f;
  LabColor lab = {
    .l = hct.tone,
    .a = hct.chroma * std::cos(hue_rad),
    .b = hct.chroma * std::sin(hue_rad),
  };
  auto xyz = LabToXyz(lab);
  float r, g, b;
  XyzToLinearRgb(xyz, r, g, b);
  auto clamp = [](float v) { return std::clamp(v, 0.0f, 1.0f); };
  return SkColorSetARGB(
      255,
      static_cast<uint8_t>(std::round(LinearToSrgb(clamp(r)) * 255.0f)),
      static_cast<uint8_t>(std::round(LinearToSrgb(clamp(g)) * 255.0f)),
      static_cast<uint8_t>(std::round(LinearToSrgb(clamp(b)) * 255.0f)));
}


struct TonalPalette {
  float hue;
  float chroma;

  SkColor GetTone(float tone) const {
    return HctToArgb({.hue = hue, .chroma = chroma, .tone = tone});
  }
};

TonalPalette PaletteFromColor(SkColor color) {
  auto hct = ArgbToHct(color);
  return {.hue = hct.hue, .chroma = hct.chroma};
}

constexpr float kSurfaceLight = 98.0f;
constexpr float kSurfaceDark = 6.0f;
constexpr float kSurfaceDimLight = 87.0f;
constexpr float kSurfaceDimDark = 4.0f;
constexpr float kSurfaceBrightLight = 98.0f;
constexpr float kSurfaceBrightDark = 24.0f;
constexpr float kSurfaceContainerLowestLight = 100.0f;
constexpr float kSurfaceContainerLowestDark = 0.0f;
constexpr float kSurfaceContainerLowLight = 96.0f;
constexpr float kSurfaceContainerLowDark = 10.0f;
constexpr float kSurfaceContainerLight = 94.0f;
constexpr float kSurfaceContainerDark = 12.0f;
constexpr float kSurfaceContainerHighLight = 92.0f;
constexpr float kSurfaceContainerHighDark = 17.0f;
constexpr float kSurfaceContainerHighestLight = 90.0f;
constexpr float kSurfaceContainerHighestDark = 22.0f;

}

M3ColorScheme GenerateM3ColorScheme(SkColor seed_color, bool dark_mode) {
  auto primary = PaletteFromColor(seed_color);

  TonalPalette secondary = {primary.hue, primary.chroma * 0.5f};

  TonalPalette tertiary = {
      .hue = std::fmod(primary.hue + 60.0f, 360.0f),
      .chroma = primary.chroma * 0.7f,
  };

  TonalPalette neutral = {primary.hue, primary.chroma * 0.03f};
  TonalPalette neutral_variant = {primary.hue, primary.chroma * 0.08f};
  TonalPalette error_palette = {25.0f, 84.0f};

  M3ColorScheme scheme;

  scheme.primary = primary.GetTone(dark_mode ? 80.0f : 40.0f);
  scheme.on_primary = primary.GetTone(dark_mode ? 20.0f : 100.0f);
  scheme.primary_container = primary.GetTone(dark_mode ? 30.0f : 90.0f);
  scheme.on_primary_container = primary.GetTone(dark_mode ? 90.0f : 10.0f);
  scheme.inverse_primary = primary.GetTone(dark_mode ? 40.0f : 80.0f);

  scheme.secondary = secondary.GetTone(dark_mode ? 80.0f : 40.0f);
  scheme.on_secondary = secondary.GetTone(dark_mode ? 20.0f : 100.0f);
  scheme.secondary_container = secondary.GetTone(dark_mode ? 30.0f : 90.0f);
  scheme.on_secondary_container = secondary.GetTone(dark_mode ? 90.0f : 10.0f);

  scheme.tertiary = tertiary.GetTone(dark_mode ? 80.0f : 40.0f);
  scheme.on_tertiary = tertiary.GetTone(dark_mode ? 20.0f : 100.0f);
  scheme.tertiary_container = tertiary.GetTone(dark_mode ? 30.0f : 90.0f);
  scheme.on_tertiary_container = tertiary.GetTone(dark_mode ? 90.0f : 10.0f);

  scheme.error = error_palette.GetTone(dark_mode ? 80.0f : 40.0f);
  scheme.on_error = error_palette.GetTone(dark_mode ? 20.0f : 100.0f);
  scheme.error_container = error_palette.GetTone(dark_mode ? 30.0f : 90.0f);
  scheme.on_error_container = error_palette.GetTone(dark_mode ? 90.0f : 10.0f);

  scheme.background = neutral.GetTone(dark_mode ? kSurfaceDark : kSurfaceLight);
  scheme.on_background = neutral.GetTone(dark_mode ? 90.0f : 10.0f);
  scheme.surface = neutral.GetTone(dark_mode ? kSurfaceDark : kSurfaceLight);
  scheme.on_surface = neutral.GetTone(dark_mode ? 90.0f : 10.0f);
  scheme.surface_variant = neutral_variant.GetTone(dark_mode ? 30.0f : 90.0f);
  scheme.on_surface_variant =
      neutral_variant.GetTone(dark_mode ? 80.0f : 30.0f);
  scheme.inverse_surface = neutral.GetTone(dark_mode ? 90.0f : 20.0f);
  scheme.inverse_on_surface = neutral.GetTone(dark_mode ? 20.0f : 95.0f);

  scheme.outline = neutral_variant.GetTone(dark_mode ? 60.0f : 50.0f);
  scheme.outline_variant =
      neutral_variant.GetTone(dark_mode ? 30.0f : 80.0f);

  scheme.shadow = SK_ColorBLACK;
  scheme.scrim = SK_ColorBLACK;

  scheme.surface_tint = scheme.primary;
  scheme.surface_dim = neutral.GetTone(dark_mode ? kSurfaceDimDark
                                                  : kSurfaceDimLight);
  scheme.surface_bright = neutral.GetTone(dark_mode ? kSurfaceBrightDark
                                                     : kSurfaceBrightLight);
  scheme.surface_container_lowest =
      neutral.GetTone(dark_mode ? kSurfaceContainerLowestDark
                                : kSurfaceContainerLowestLight);
  scheme.surface_container_low =
      neutral.GetTone(dark_mode ? kSurfaceContainerLowDark
                                : kSurfaceContainerLowLight);
  scheme.surface_container =
      neutral.GetTone(dark_mode ? kSurfaceContainerDark
                                : kSurfaceContainerLight);
  scheme.surface_container_high =
      neutral.GetTone(dark_mode ? kSurfaceContainerHighDark
                                : kSurfaceContainerHighLight);
  scheme.surface_container_highest =
      neutral.GetTone(dark_mode ? kSurfaceContainerHighestDark
                                : kSurfaceContainerHighestLight);

  return scheme;
}

M3ColorScheme GenerateDynamicColorScheme(bool dark_mode) {
  const SkColor kDefaultSeed = SkColorSetRGB(0x67, 0x50, 0xC4);
  return GenerateM3ColorScheme(kDefaultSeed, dark_mode);
}

SkColor GetSurfaceColorAtElevation(const M3ColorScheme& scheme,
                                    int elevation_dp) {
  if (elevation_dp <= 0) return scheme.surface_container_lowest;
  if (elevation_dp <= 3) return scheme.surface_container_low;
  if (elevation_dp <= 8) return scheme.surface_container;
  if (elevation_dp <= 12) return scheme.surface_container_high;
  return scheme.surface_container_highest;
}

SkColor ResolveM3Role(const M3ColorScheme& scheme, const std::string& role) {
  static const std::unordered_map<std::string, SkColor M3ColorScheme::*> kRoles = {
    {"primary", &M3ColorScheme::primary},
    {"on_primary", &M3ColorScheme::on_primary},
    {"primary_container", &M3ColorScheme::primary_container},
    {"on_primary_container", &M3ColorScheme::on_primary_container},
    {"secondary", &M3ColorScheme::secondary},
    {"on_secondary", &M3ColorScheme::on_secondary},
    {"secondary_container", &M3ColorScheme::secondary_container},
    {"on_secondary_container", &M3ColorScheme::on_secondary_container},
    {"tertiary", &M3ColorScheme::tertiary},
    {"on_tertiary", &M3ColorScheme::on_tertiary},
    {"tertiary_container", &M3ColorScheme::tertiary_container},
    {"on_tertiary_container", &M3ColorScheme::on_tertiary_container},
    {"error", &M3ColorScheme::error},
    {"on_error", &M3ColorScheme::on_error},
    {"error_container", &M3ColorScheme::error_container},
    {"on_error_container", &M3ColorScheme::on_error_container},
    {"background", &M3ColorScheme::background},
    {"on_background", &M3ColorScheme::on_background},
    {"surface", &M3ColorScheme::surface},
    {"on_surface", &M3ColorScheme::on_surface},
    {"surface_variant", &M3ColorScheme::surface_variant},
    {"on_surface_variant", &M3ColorScheme::on_surface_variant},
    {"outline", &M3ColorScheme::outline},
    {"outline_variant", &M3ColorScheme::outline_variant},
    {"shadow", &M3ColorScheme::shadow},
    {"scrim", &M3ColorScheme::scrim},
  };
  auto it = kRoles.find(role);
  if (it != kRoles.end()) {
    return scheme.*(it->second);
  }
  return scheme.primary;
}

}
