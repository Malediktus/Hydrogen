#pragma once

#include "../Core/Memory.hpp"
#include <filesystem>

namespace Hydrogen {
class Asset {
 public:
  struct AssetInfo {
    bool Preload;
  };

  virtual ~Asset() = default;
  virtual void Load(const std::filesystem::path& filepath) = 0;

  AssetInfo GetInfo() { return m_AssetInfo; }

 protected:
  AssetInfo m_AssetInfo;
};
}  // namespace Hydrogen
