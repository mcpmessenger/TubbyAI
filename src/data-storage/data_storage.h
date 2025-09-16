// Copyright 2025 TubbyAI Browser
// Data Storage Module - Header File

#ifndef TUBBYAI_DATA_STORAGE_H_
#define TUBBYAI_DATA_STORAGE_H_

#include <string>
#include <vector>
#include <memory>
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "sql/database.h"

namespace tubbyai {

// Forward declaration
struct InteractionData;

// Data storage interface for interaction data
class DataStorage {
 public:
  explicit DataStorage(const base::FilePath& db_path);
  ~DataStorage();

  // Initialize the database
  bool Initialize();

  // Store interaction data
  bool StoreInteractionData(const InteractionData& data);

  // Retrieve interaction data for a specific URL
  std::vector<InteractionData> GetDataForUrl(const std::string& url) const;

  // Retrieve interaction data for a specific element
  std::vector<InteractionData> GetDataForElement(const std::string& selector) const;

  // Clear all data
  bool ClearAllData();

  // Clear data for specific URL
  bool ClearDataForUrl(const std::string& url);

  // Get storage statistics
  struct StorageStats {
    int total_interactions;
    int total_urls;
    size_t total_size_bytes;
  };
  StorageStats GetStorageStats() const;

 private:
  // Database operations
  bool CreateTables();
  bool InsertInteractionData(const InteractionData& data);
  std::vector<InteractionData> QueryData(const std::string& where_clause) const;

  // Database connection
  std::unique_ptr<sql::Database> database_;
  base::FilePath db_path_;

  DISALLOW_COPY_AND_ASSIGN(DataStorage);
};

}  // namespace tubbyai

#endif  // TUBBYAI_DATA_STORAGE_H_
