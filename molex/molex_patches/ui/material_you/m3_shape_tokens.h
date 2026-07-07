#ifndef MOLEX_UI_MATERIAL_YOU_M3_SHAPE_TOKENS_H_
#define MOLEX_UI_MATERIAL_YOU_M3_SHAPE_TOKENS_H_

#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/rounded_corners_f.h"

namespace molex::m3 {


struct M3Shape {
  float none = 0.0f;
  float extra_small = 4.0f;
  float small = 8.0f;
  float medium = 12.0f;
  float large = 16.0f;
  float extra_large = 28.0f;
  float full = 1000.0f;
};

struct M3ComponentShapes {
  float navigation_bar;
  float navigation_rail;
  float tab;

  float card_elevated;
  float card_filled;
  float card_outlined;

  float button_text;
  float button_filled;
  float button_tonal;
  float button_outlined;
  float fab;

  float text_field;
  float search_bar;

  float dialog;
  float bottom_sheet;
  float menu;
  float tooltip;

  float chip_assist;
  float chip_filter;
  float chip_input;
  float chip_suggestion;
};

const M3Shape& GetM3Shape();

M3ComponentShapes GetM3ComponentShapes();

gfx::RoundedCornersF ApplyM3Corners(const gfx::RoundedCornersF& base,
                                     float radius);

}

#endif
