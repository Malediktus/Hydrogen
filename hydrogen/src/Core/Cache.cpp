#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <Hydrogen/Core/Cache.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace Hydrogen;

CacheFile::CacheFile(const String& path, const String& content) {
  m_CacheFilepath = path + ".hycache";

  m_ContentChecksum = std::hash<String>{}(content);
  if (!std::filesystem::exists(m_CacheFilepath)) {
    m_CacheCecksum.reset();
    return;
  }

  std::ofstream cachefile;
  cachefile.open(m_CacheFilepath, std::ios::in);
  HY_ASSERT(cachefile.is_open(), "Failed to open file {}!", m_CacheFilepath);
  std::stringstream cachefileStream;
  cachefileStream << cachefile.rdbuf();
  cachefile.close();
  size_t val;
  cachefileStream >> val;
  m_CacheCecksum = val;
}

bool CacheFile::CacheValid() {
  return m_CacheCecksum.has_value() &&
         m_ContentChecksum == m_CacheCecksum.value();
}

void CacheFile::UpdateCacheChecksum() {
  String directory = DIRECTORY_FROM_PATH(m_CacheFilepath);
  if (!std::filesystem::exists(directory)) {
    std::filesystem::create_directories(directory);
  }

  std::ofstream cachefile;
  cachefile.open(m_CacheFilepath, std::ios::out);
  HY_ASSERT(cachefile.is_open(), "Failed to open file {}!", m_CacheFilepath);
  cachefile << m_ContentChecksum;
  cachefile.close();
}
