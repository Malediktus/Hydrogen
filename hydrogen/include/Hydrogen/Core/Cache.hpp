#pragma once

#include <optional>
#include "Memory.hpp"

namespace Hydrogen {
class CacheFile {
 public:
  CacheFile(const String& path, const String& content);
  ~CacheFile() = default;
  bool CacheValid();
  void UpdateCacheChecksum();

 private:
  size_t m_ContentChecksum;
  std::optional<size_t> m_CacheCecksum;
  String m_CacheFilepath;
};
}  // namespace Hydrogen
