// Copyright 2025 TubbyAI Browser
// Data Storage Module - Implementation

#include "src/data-storage/data_storage.h"

#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "sql/statement.h"

namespace tubbyai {

DataStorage::DataStorage(const base::FilePath& db_path) : db_path_(db_path) {
  database_ = std::make_unique<sql::Database>();
}

DataStorage::~DataStorage() {
  if (database_ && database_->is_open()) {
    database_->Close();
  }
}

bool DataStorage::Initialize() {
  if (!database_->Open(db_path_)) {
    LOG(ERROR) << "DataStorage: Failed to open database at " << db_path_.AsUTF8Unsafe();
    return false;
  }

  if (!CreateTables()) {
    LOG(ERROR) << "DataStorage: Failed to create tables";
    return false;
  }

  VLOG(1) << "DataStorage: Initialized successfully at " << db_path_.AsUTF8Unsafe();
  return true;
}

bool DataStorage::StoreInteractionData(const InteractionData& data) {
  if (!database_ || !database_->is_open()) {
    LOG(ERROR) << "DataStorage: Database not initialized";
    return false;
  }

  return InsertInteractionData(data);
}

std::vector<InteractionData> DataStorage::GetDataForUrl(const std::string& url) const {
  return QueryData("WHERE website_url = '" + url + "'");
}

std::vector<InteractionData> DataStorage::GetDataForElement(const std::string& selector) const {
  return QueryData("WHERE element_selector = '" + selector + "'");
}

bool DataStorage::ClearAllData() {
  if (!database_ || !database_->is_open()) {
    return false;
  }

  sql::Statement statement(database_->GetCachedStatement(
      SQL_FROM_HERE, "DELETE FROM interactions"));
  
  return statement.Run();
}

bool DataStorage::ClearDataForUrl(const std::string& url) {
  if (!database_ || !database_->is_open()) {
    return false;
  }

  sql::Statement statement(database_->GetCachedStatement(
      SQL_FROM_HERE, "DELETE FROM interactions WHERE website_url = ?"));
  statement.BindString(0, url);
  
  return statement.Run();
}

DataStorage::StorageStats DataStorage::GetStorageStats() const {
  StorageStats stats = {0, 0, 0};
  
  if (!database_ || !database_->is_open()) {
    return stats;
  }

  // Get total interactions
  sql::Statement count_statement(database_->GetCachedStatement(
      SQL_FROM_HERE, "SELECT COUNT(*) FROM interactions"));
  if (count_statement.Step()) {
    stats.total_interactions = count_statement.ColumnInt(0);
  }

  // Get total unique URLs
  sql::Statement url_statement(database_->GetCachedStatement(
      SQL_FROM_HERE, "SELECT COUNT(DISTINCT website_url) FROM interactions"));
  if (url_statement.Step()) {
    stats.total_urls = url_statement.ColumnInt(0);
  }

  // Get total size (approximate)
  if (base::PathExists(db_path_)) {
    int64_t file_size;
    if (base::GetFileSize(db_path_, &file_size)) {
      stats.total_size_bytes = static_cast<size_t>(file_size);
    }
  }

  return stats;
}

bool DataStorage::CreateTables() {
  const char kCreateTableSql[] = R"(
    CREATE TABLE IF NOT EXISTS interactions (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      website_url TEXT NOT NULL,
      element_selector TEXT NOT NULL,
      element_text TEXT,
      page_url TEXT NOT NULL,
      pre_interaction_thumbnail TEXT,
      post_interaction_thumbnail TEXT,
      interaction_gif_or_video TEXT,
      timestamp INTEGER NOT NULL
    )
  )";

  sql::Statement statement(database_->GetCachedStatement(SQL_FROM_HERE, kCreateTableSql));
  return statement.Run();
}

bool DataStorage::InsertInteractionData(const InteractionData& data) {
  const char kInsertSql[] = R"(
    INSERT INTO interactions (
      website_url, element_selector, element_text, page_url,
      pre_interaction_thumbnail, post_interaction_thumbnail,
      interaction_gif_or_video, timestamp
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
  )";

  sql::Statement statement(database_->GetCachedStatement(SQL_FROM_HERE, kInsertSql));
  statement.BindString(0, data.website_url);
  statement.BindString(1, data.element_selector);
  statement.BindString(2, data.element_text);
  statement.BindString(3, data.page_url);
  statement.BindString(4, data.pre_interaction_thumbnail);
  statement.BindString(5, data.post_interaction_thumbnail);
  statement.BindString(6, data.interaction_gif_or_video);
  statement.BindInt64(7, data.timestamp.ToInternalValue());

  return statement.Run();
}

std::vector<InteractionData> DataStorage::QueryData(const std::string& where_clause) const {
  std::vector<InteractionData> results;
  
  if (!database_ || !database_->is_open()) {
    return results;
  }

  std::string query = "SELECT * FROM interactions " + where_clause;
  sql::Statement statement(database_->GetCachedStatement(SQL_FROM_HERE, query.c_str()));

  while (statement.Step()) {
    InteractionData data;
    data.website_url = statement.ColumnString(1);
    data.element_selector = statement.ColumnString(2);
    data.element_text = statement.ColumnString(3);
    data.page_url = statement.ColumnString(4);
    data.pre_interaction_thumbnail = statement.ColumnString(5);
    data.post_interaction_thumbnail = statement.ColumnString(6);
    data.interaction_gif_or_video = statement.ColumnString(7);
    data.timestamp = base::Time::FromInternalValue(statement.ColumnInt64(8));
    
    results.push_back(data);
  }

  return results;
}

}  // namespace tubbyai
