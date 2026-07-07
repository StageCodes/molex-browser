#ifndef MOLEX_UI_MATERIAL_YOU_M3_COLOR_SCHEME_H_
#define MOLEX_UI_MATERIAL_YOU_M3_COLOR_SCHEME_H_

#include <string>

#include "third_party/skia/include/core/SkColor.h"

namespace molex::m3 {

struct M3ColorScheme {
  SkColor primary;
  SkColor on_primary;
  SkColor primary_container;
  SkColor on_primary_container;
  SkColor inverse_primary;

  SkColor secondary;
  SkColor on_secondary;
  SkColor secondary_container;
  SkColor on_secondary_container;

  SkColor tertiary;
  SkColor on_tertiary;
  SkColor tertiary_container;
  SkColor on_tertiary_container;

  SkColor error;
  SkColor on_error;
  SkColor error_container;
  SkColor on_error_container;

  SkColor background;
  SkColor on_background;
  SkColor surface;
  SkColor on_surface;
  SkColor surface_variant;
  SkColor on_surface_variant;
  SkColor inverse_surface;
  SkColor inverse_on_surface;

  SkColor outline;
  SkColor outline_variant;

  SkColor shadow;
  SkColor scrim;

  SkColor surface_tint;
  SkColor surface_container_lowest;
  SkColor surface_container_low;
  SkColor surface_container;
  SkColor surface_container_high;
  SkColor surface_container_highest;
  SkColor surface_dim;
  SkColor surface_bright;
};

M3ColorScheme GenerateM3ColorScheme(SkColor seed_color, bool dark_mode);

M3ColorScheme GenerateDynamicColorScheme(bool dark_mode);

SkColor GetSurfaceColorAtElevation(const M3ColorScheme& scheme,
                                    int elevation_dp);

SkColor ResolveM3Role(const M3ColorScheme& scheme, const std::string& role);

}

#endif
