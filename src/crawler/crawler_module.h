// Copyright 2025 TubbyAI Browser
// Crawler Module - Header File

#ifndef TUBBYAI_CRAWLER_MODULE_H_
#define TUBBYAI_CRAWLER_MODULE_H_

#include <string>
#include <vector>
#include <memory>
#include "base/callback.h"
#include "base/memory/weak_ptr.h"

namespace tubbyai {

// Structure to hold interaction data
struct InteractionData {
  std::string website_url;
  std::string element_selector;
  std::string element_text;
  std::string page_url;
  std::string pre_interaction_thumbnail;
  std::string post_interaction_thumbnail;
  std::string interaction_gif_or_video;
  base::Time timestamp;
};

// Callback for crawler completion
using CrawlerCallback = base::OnceCallback<void(bool success, 
                                               const std::vector<InteractionData>& data)>;

// Crawler module that uses Playwright to capture interactions
class CrawlerModule {
 public:
  CrawlerModule();
  ~CrawlerModule();

  // Start crawling a website
  void StartCrawl(const std::string& url, 
                  int max_depth = 3,
                  CrawlerCallback callback = base::NullCallback());

  // Stop current crawling operation
  void StopCrawl();

  // Check if currently crawling
  bool IsCrawling() const;

  // Get crawled data for a specific URL
  std::vector<InteractionData> GetDataForUrl(const std::string& url) const;

 private:
  // Internal crawling logic
  void CrawlPage(const std::string& url, int depth);
  void ProcessElement(const std::string& selector, const std::string& page_url);
  void SaveInteractionData(const InteractionData& data);

  // State management
  bool is_crawling_;
  std::string current_url_;
  std::vector<InteractionData> crawled_data_;
  base::WeakPtrFactory<CrawlerModule> weak_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(CrawlerModule);
};

}  // namespace tubbyai

#endif  // TUBBYAI_CRAWLER_MODULE_H_
