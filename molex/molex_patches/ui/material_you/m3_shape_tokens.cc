#include "molex/molex_patches/ui/material_you/m3_shape_tokens.h"

namespace molex::m3 {

const M3Shape& GetM3Shape() {
  static const M3Shape kShape;
  return kShape;
}

M3ComponentShapes GetM3ComponentShapes() {
  const auto& s = GetM3Shape();
  return M3ComponentShapes{
    .navigation_bar = s.extra_large,
    .navigation_rail = s.extra_large,
    .tab = s.full,
    .card_elevated = s.medium,
    .card_filled = s.medium,
    .card_outlined = s.medium,
    .button_text = s.full,
    .button_filled = s.full,
    .button_tonal = s.full,
    .button_outlined = s.full,
    .fab = s.large,
    .text_field = s.small,
    .search_bar = s.full,
    .dialog = s.extra_large,
    .bottom_sheet = s.extra_large,
    .menu = s.small,
    .tooltip = s.small,
    .chip_assist = s.small,
    .chip_filter = s.small,
    .chip_input = s.small,
    .chip_suggestion = s.small,
  };
}

gfx::RoundedCornersF ApplyM3Corners(const gfx::RoundedCornersF& base,
                                     float radius) {
  return gfx::RoundedCornersF(
      std::max(base.upper_left(), radius),
      std::max(base.upper_right(), radius),
      std::max(base.lower_right(), radius),
      std::max(base.lower_left(), radius));
}

}
