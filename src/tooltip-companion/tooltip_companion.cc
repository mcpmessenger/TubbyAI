// Copyright 2025 TubbyAI Browser
// Tooltip Companion Module - Implementation

#include "src/tooltip-companion/tooltip_companion.h"

#include "base/bind.h"
#include "base/logging.h"
#include "content/public/browser/web_contents.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/widget/widget.h"

namespace tubbyai {

TooltipCompanion::TooltipCompanion(content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents) {
  DCHECK(web_contents);
}

TooltipCompanion::~TooltipCompanion() = default;

void TooltipCompanion::DidFinishLoad(content::RenderFrameHost* render_frame_host,
                                     const GURL& validated_url) {
  // Initialize tooltip functionality when page loads
  VLOG(1) << "TooltipCompanion: Page loaded - " << validated_url.spec();
}

void TooltipCompanion::ShowTooltip(const std::string& element_selector,
                                   const std::string& interaction_data) {
  if (IsTooltipVisible()) {
    HideTooltip();
  }

  // Create tooltip widget
  tooltip_widget_ = std::make_unique<views::Widget>();
  views::Widget::InitParams params;
  params.type = views::Widget::InitParams::TYPE_TOOLTIP;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.context = web_contents()->GetTopLevelNativeWindow();
  
  tooltip_widget_->Init(std::move(params));
  
  // Create content view with interaction preview
  auto* content_view = new views::View();
  content_view->SetLayoutManager(std::make_unique<views::FillLayout>());
  
  // TODO: Add image view for interaction preview
  // This will be implemented when we integrate with the crawler module
  
  tooltip_widget_->SetContentsView(content_view);
  tooltip_widget_->Show();
  
  VLOG(1) << "TooltipCompanion: Showing tooltip for element: " << element_selector;
}

void TooltipCompanion::HideTooltip() {
  if (tooltip_widget_) {
    tooltip_widget_->Close();
    tooltip_widget_.reset();
  }
}

bool TooltipCompanion::IsTooltipVisible() const {
  return tooltip_widget_ && tooltip_widget_->IsVisible();
}

}  // namespace tubbyai
