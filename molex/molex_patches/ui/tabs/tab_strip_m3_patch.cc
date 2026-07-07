#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "chrome/browser/ui/views/tabs/tab_style_views.h"
#include "cc/paint/paint_flags.h"
#include "molex/molex_patches/ui/material_you/m3_theme.h"
#include "third_party/skia/include/core/SkPath.h"
#include "third_party/skia/include/core/SkRect.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/rect_f.h"

namespace {

class TabStyleViewsM3 : public TabStyleViews {
 public:
  explicit TabStyleViewsM3(Tab* tab) : TabStyleViews(tab), tab_(tab) {}
  ~TabStyleViewsM3() override = default;

  SkColor GetTabBackgroundColor(
      TabActive active,
      BrowserFrameActiveState active_state) const override {
    auto* m3 = molex::m3::M3Theme::Get();
    const bool is_active = active == TabActive::kActive;

    if (tab_->IsIncognito()) {
      return is_active
                 ? m3->color_scheme().surface_container_highest
                 : m3->color_scheme().surface_container_low;
    }
    return is_active ? m3->color_scheme().surface_container_high
                     : m3->color_scheme().surface_container_low;
  }

  SkColor GetTabForegroundColor(
      TabActive active,
      BrowserFrameActiveState active_state) const override {
    auto* m3 = molex::m3::M3Theme::Get();
    return (active == TabActive::kActive) ? m3->color_scheme().on_surface
                                          : m3->color_scheme().on_surface_variant;
  }

  void PaintTab(gfx::Canvas* canvas) const override {
    PaintTabBackground(canvas, tab_->IsActive(), 0);
  }

  void PaintTabBackground(gfx::Canvas* canvas, bool active,
                           int fill_id) const override {
    const gfx::Rect& bounds = GetBounds();
    if (bounds.IsEmpty()) return;

    cc::PaintFlags flags;
    flags.setAntiAlias(true);
    flags.setColor(GetTabBackgroundColor(
        active ? TabActive::kActive : TabActive::kInactive,
        BrowserFrameActiveState::kActive));

    const float radius = GetCornerRadius();

    canvas->DrawRoundRect(bounds, radius, flags);

    if (active) {
      auto* m3 = molex::m3::M3Theme::Get();
      flags.setColor(m3->color_scheme().primary);
      flags.setStyle(cc::PaintFlags::kFill_Style);

      const int indicator_h = 3;
      const int indicator_w = std::max(std::min(bounds.width() / 3, 40), 12);
      const int ind_y = bounds.bottom() - 4;
      const int ind_x = bounds.x() + (bounds.width() - indicator_w) / 2;

      canvas->DrawRoundRect(
          gfx::Rect(ind_x, ind_y, indicator_w, indicator_h),
          indicator_h / 2.0f, flags);
    }
  }

  SkPath GetPath(TabActive active) const override {
    SkPath path;
    const gfx::Rect& bounds = GetBounds();
    const float radius = GetCornerRadius();
    path.addRoundRect(SkRect::MakeXYWH(bounds.x(), bounds.y(),
                                        bounds.width(), bounds.height()),
                      radius, radius);
    return path;
  }

  gfx::Insets GetContentsInsets() const override {
    return gfx::Insets::VH(6, 16);
  }

  int GetStrokeThickness(bool active) const override {
    return 0;
  }

  float GetCornerRadius() const override {
    return std::max(static_cast<float>(GetBounds().height()) / 2.0f, 16.0f);
  }

 private:
  raw_ptr<Tab> tab_;
};

}
