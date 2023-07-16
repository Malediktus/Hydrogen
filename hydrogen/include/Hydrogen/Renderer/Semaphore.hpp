#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderDevice.hpp"

namespace Hydrogen {
class Semaphore {
 public:
  virtual ~Semaphore() = default;

  static ReferencePointer<Semaphore> Create(const ReferencePointer<RenderDevice>& renderDevice);
};
}  // namespace Hydrogen
