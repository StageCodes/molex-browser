#ifndef MOLEX_UI_MATERIAL_YOU_M3_TYPOGRAPHY_H_
#define MOLEX_UI_MATERIAL_YOU_M3_TYPOGRAPHY_H_

#include <string>

#include "ui/gfx/font_list.h"

namespace molex::m3 {


struct M3TypeScale {
  gfx::FontList display_large;
  gfx::FontList display_medium;
  gfx::FontList display_small;

  gfx::FontList headline_large;
  gfx::FontList headline_medium;
  gfx::FontList headline_small;

  gfx::FontList title_large;
  gfx::FontList title_medium;
  gfx::FontList title_small;

  gfx::FontList label_large;
  gfx::FontList label_medium;
  gfx::FontList label_small;

  gfx::FontList body_large;
  gfx::FontList body_medium;
  gfx::FontList body_small;
};

const M3TypeScale& GetM3TypeScale();

std::string GetM3Typeface();

}

#endif
