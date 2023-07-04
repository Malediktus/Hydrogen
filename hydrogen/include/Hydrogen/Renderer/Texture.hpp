#pragma once

#include <memory>
#include <string>

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
class Texture {
 public:
  virtual ~Texture() = default;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  virtual void Bind(uint32_t slot = 0) const = 0;

  virtual void* GetNative() const = 0;
};

enum class Texture2DStorageType {
  RGBA8F = 0,
  RGBA8I = 1,
  RGBA16F = 2,
  RGBA16I = 3,
  RGBA32F = 4,
  RGBA32I = 5,
  SRGBA8F = 6,
  RGB8F = 7,
  RGB8I = 8,
  RGB16F = 9,
  RGB16I = 10,
  RGB32F = 11,
  RGB32I = 12,
  SRGB8F = 13,
  Depth = 14,
  Stencil = 15,
  DepthStencil = 16
};

class Texture2D : public Texture {
 public:
  virtual ~Texture2D() = default;

  virtual uint32_t GetWidth() const override {
    HY_ASSERT_CHECK(false,
                    "Texture2D is an abstract class and should not be called, "
                    "(Please use TextureCreate)");
    return 0;
  }

  virtual uint32_t GetHeight() const override {
    HY_ASSERT_CHECK(false,
                    "Texture2D is an abstract class and should not be called, "
                    "(Please use TextureCreate)");
    return 0;
  }

  virtual void Bind(uint32_t) const override {
    HY_ASSERT_CHECK(false,
                    "Texture2D is an abstract class and should not be called, "
                    "(Please use TextureCreate)");
  }

  virtual void Resize(uint32_t width, uint32_t height) = 0;

  virtual void* GetNative() const override {
    HY_ASSERT_CHECK(false,
                    "Texture2D is an abstract class and should not be called, "
                    "(Please use TextureCreate)");
    return nullptr;
  }

  static ReferencePointer<Texture2D> Create(
      const int width, const int height,
      Texture2DStorageType storageType = Texture2DStorageType::RGBA8F);
  static ReferencePointer<Texture2D> Create(
      const int width, const int height, const void* data,
      Texture2DStorageType storageType = Texture2DStorageType::RGBA8F);
};
}  // namespace Hydrogen
