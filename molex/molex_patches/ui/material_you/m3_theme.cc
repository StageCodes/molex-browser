#include "molex/molex_patches/ui/material_you/m3_theme.h"

#include "base/no_destructor.h"
#include "ui/color/color_id.h"
#include "ui/color/color_provider.h"
#include "ui/color/color_recipe.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/widget/widget.h"

namespace molex::m3 {

M3Theme* M3Theme::Get() {
  static base::NoDestructor<M3Theme> instance;
  return instance.get();
}

M3Theme::M3Theme() {
  UpdateTheme(ui::NativeTheme::GetInstanceForNativeUi());
}

void M3Theme::UpdateTheme(ui::NativeTheme* native_theme) {
  if (!native_theme) return;
  dark_mode_ = native_theme->ShouldUseDarkColors();

  SkColor seed = SkColorSetRGB(0x67, 0x50, 0xC4);
  color_scheme_ = GenerateM3ColorScheme(seed, dark_mode_);
  shapes_ = GetM3ComponentShapes();
  typography_ = GetM3TypeScale();
}

void M3Theme::ApplyToWidget(views::Widget* widget) {
  if (!widget) return;
  auto* cp = widget->GetColorProvider();
  if (!cp) return;

}

}
