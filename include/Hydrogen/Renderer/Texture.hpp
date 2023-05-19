#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include <memory>
#include <string>

namespace Hydrogen {
class Texture {
public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual void* GetNative() const = 0;
};

enum class Texture2DUsageType { Color = 0, Depth = 1, Stencil = 2, DepthStencil = 3 };

class Texture2D : public Texture {
public:
    virtual ~Texture2D() = default;

    virtual uint32_t GetWidth() const override {
        HY_ASSERT_CHECK(false, "Texture2D is an abstract class and should not be called, (Please use TextureCreate)");
        return 0;
    }

    virtual uint32_t GetHeight() const override {
        HY_ASSERT_CHECK(false, "Texture2D is an abstract class and should not be called, (Please use TextureCreate)");
        return 0;
    }

    virtual void Bind(uint32_t) const override {
        HY_ASSERT_CHECK(false, "Texture2D is an abstract class and should not be called, (Please use TextureCreate)");
    }

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual void* GetNative() const override {
        HY_ASSERT_CHECK(false, "Texture2D is an abstract class and should not be called, (Please use TextureCreate)");
        return nullptr;
    }

    static Reference<Texture2D> Create(const int width, const int height, Texture2DUsageType usageType = Texture2DUsageType::Color);
    static Reference<Texture2D> Create(const int width, const int height, const void* data, Texture2DUsageType usageType = Texture2DUsageType::Color);
};
} // namespace Hydrogen
