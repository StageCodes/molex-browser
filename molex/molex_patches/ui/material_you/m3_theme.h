#ifndef MOLEX_UI_MATERIAL_YOU_M3_THEME_H_
#define MOLEX_UI_MATERIAL_YOU_M3_THEME_H_

#include "molex/molex_patches/ui/material_you/m3_color_scheme.h"
#include "molex/molex_patches/ui/material_you/m3_shape_tokens.h"
#include "molex/molex_patches/ui/material_you/m3_typography.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/widget/widget.h"

namespace molex::m3 {

class M3Theme {
 public:
  static M3Theme* Get();

  void UpdateTheme(ui::NativeTheme* native_theme);

  const M3ColorScheme& color_scheme() const { return color_scheme_; }
  const M3ComponentShapes& shapes() const { return shapes_; }
  const M3TypeScale& typography() const { return typography_; }

  bool dark_mode() const { return dark_mode_; }

  void ApplyToWidget(views::Widget* widget);

 private:
  M3Theme();
  ~M3Theme() = default;

  M3ColorScheme color_scheme_;
  M3ComponentShapes shapes_;
  M3TypeScale typography_;
  bool dark_mode_ = false;
};

}

#endif
