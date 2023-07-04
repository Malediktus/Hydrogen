#pragma once

#include "../Core/Memory.hpp"

namespace Hydrogen {
class Asset {
 public:
  struct AssetInfo {
    bool Preload;
  };

  virtual ~Asset() = default;
  virtual void Load(const String& filepath) = 0;

  AssetInfo GetInfo() { return m_AssetInfo; }

 protected:
  AssetInfo m_AssetInfo;
};
}  // namespace Hydrogen
