#include <memory>

#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "molex/molex_patches/ui/material_you/m3_theme.h"
#include "ui/gfx/canvas.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/textfield/textfield.h"

void LocationBarView::SetM3PillShape(bool enabled) {
  m3_pill_shape_ = enabled;
  if (enabled) {
    auto* m3 = molex::m3::M3Theme::Get();
    m3_fill_color_ = m3->color_scheme().surface_container_high;
    m3_outline_color_ = m3->color_scheme().outline;

    SetBackground(nullptr);

    if (location_entry_) {
      location_entry_->SetFontList(m3->typography().body_large);
    }
  }
  SchedulePaint();
}
