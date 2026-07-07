#include "molex/molex_patches/ui/material_you/m3_typography.h"

#include "ui/gfx/font.h"
#include "ui/gfx/font_list.h"

namespace molex::m3 {

namespace {

gfx::FontList MakeFont(int size_dp, gfx::Font::Weight weight) {
  std::string typeface = GetM3Typeface();
  gfx::Font font(typeface, size_dp);
  font.SetWeight(weight);
  return gfx::FontList(font);
}

}

std::string GetM3Typeface() {
  return "Google Sans Text, Roboto, sans-serif";
}

const M3TypeScale& GetM3TypeScale() {
  static const M3TypeScale kScale = {
    .display_large   = MakeFont(57, gfx::Font::Weight::NORMAL),
    .display_medium  = MakeFont(45, gfx::Font::Weight::NORMAL),
    .display_small   = MakeFont(36, gfx::Font::Weight::NORMAL),

    .headline_large  = MakeFont(32, gfx::Font::Weight::NORMAL),
    .headline_medium = MakeFont(28, gfx::Font::Weight::NORMAL),
    .headline_small  = MakeFont(24, gfx::Font::Weight::NORMAL),

    .title_large     = MakeFont(22, gfx::Font::Weight::MEDIUM),
    .title_medium    = MakeFont(16, gfx::Font::Weight::MEDIUM),
    .title_small     = MakeFont(14, gfx::Font::Weight::MEDIUM),

    .label_large     = MakeFont(14, gfx::Font::Weight::MEDIUM),
    .label_medium    = MakeFont(12, gfx::Font::Weight::MEDIUM),
    .label_small     = MakeFont(11, gfx::Font::Weight::MEDIUM),

    .body_large      = MakeFont(16, gfx::Font::Weight::NORMAL),
    .body_medium     = MakeFont(14, gfx::Font::Weight::NORMAL),
    .body_small      = MakeFont(12, gfx::Font::Weight::NORMAL),
  };
  return kScale;
}

}
