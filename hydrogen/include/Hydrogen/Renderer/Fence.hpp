#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderDevice.hpp"

namespace Hydrogen {
class Fence {
 public:
  virtual ~Fence() = default;

  virtual void Wait() = 0;
  virtual void Reset() = 0;

  static ReferencePointer<Fence> Create(const ReferencePointer<RenderDevice>& renderDevice);
};
}  // namespace Hydrogen
