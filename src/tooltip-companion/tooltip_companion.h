// Copyright 2025 TubbyAI Browser
// Tooltip Companion Module - Header File

#ifndef TUBBYAI_TOOLTIP_COMPANION_H_
#define TUBBYAI_TOOLTIP_COMPANION_H_

#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace tubbyai {

// Tooltip Companion class that handles displaying interaction previews
class TooltipCompanion : public content::WebContentsObserver {
 public:
  explicit TooltipCompanion(content::WebContents* web_contents);
  ~TooltipCompanion() override;

  // WebContentsObserver implementation
  void DidFinishLoad(content::RenderFrameHost* render_frame_host,
                     const GURL& validated_url) override;

  // Tooltip management
  void ShowTooltip(const std::string& element_selector,
                   const std::string& interaction_data);
  void HideTooltip();
  bool IsTooltipVisible() const;

 private:
  // UI components
  std::unique_ptr<views::Widget> tooltip_widget_;
  base::WeakPtrFactory<TooltipCompanion> weak_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(TooltipCompanion);
};

}  // namespace tubbyai

#endif  // TUBBYAI_TOOLTIP_COMPANION_H_
