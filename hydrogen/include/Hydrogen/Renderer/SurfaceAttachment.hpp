#pragma once

#include "../Core/Memory.hpp"

namespace Hydrogen {
class SurfaceAttachment {
 public:
  virtual ~SurfaceAttachment() = default;

  static ReferencePointer<SurfaceAttachment> Create(class RenderWindow* window, bool verticalSync);
};
}  // namespace Hydrogen
