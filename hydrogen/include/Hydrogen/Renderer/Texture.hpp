#pragma once

#include "../Core/Memory.hpp"

namespace Hydrogen {
class Texture2D {
 public:
  virtual ~Texture2D() = default;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  static ReferencePointer<Texture2D> Create(const ReferencePointer<class RenderWindow>& window, const uint32_t width, const uint32_t height, const void* data);
};
}  // namespace Hydrogen
