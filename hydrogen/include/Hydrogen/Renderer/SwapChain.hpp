#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
class SwapChain {
 public:
  virtual ~SwapChain() = default;

  static ReferencePointer<SwapChain> Create(bool verticalSync);
};
}  // namespace Hydrogen
