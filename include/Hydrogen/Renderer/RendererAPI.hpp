#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"
#include "VertexArray.hpp"
#include <glm/glm.hpp>
#include <cstdint>

namespace Hydrogen {
class RendererAPI {
public:
    enum class API { OpenGL = 0 };
    enum class ClearBuffer { COLOR = 0, DEPTH = 1, STENCIL = 2 };
    enum class DepthTestFunc { ALWAYS = 0, NEVER = 1, LESS = 2, EQUAL = 3, LEQUAL = 4, GREATER = 5, NOTEQUAL = 6, GEQUAL = 7 };
    enum class StencilTestFunc { ALWAYS = 0, NEVER = 1, LESS = 2, EQUAL = 3, LEQUAL = 4, GREATER = 5, NOTEQUAL = 6, GEQUAL = 7 };
    enum class StencilTestAction { KEEP = 0, ZERO = 1, REPLACE = 2, INCR = 3, INCR_WRAP = 4, DECR = 5, DECR_WRAP = 6, INVERT = 7 };
    enum class BlendingFunc {
        ZERO = 0,
        ONE = 1,
        SRC_COLOR = 2,
        ONE_MINUS_SRC_COLOR = 3,
        DST_COLOR = 4,
        ONE_MINUS_DST_COLOR = 5,
        SRC_ALPHA = 6,
        ONE_MINUS_SRC_ALPHA = 7,
        DST_ALPHA = 8,
        ONE_MINUS_DST_ALPHA = 9,
        CONSTANT_COLOR = 10,
        ONE_MINUS_CONSTANT_COLOR = 11,
        CONSTANT_ALPHA = 12,
        ONE_MINUS_CONSTANT_ALPHA = 13
    };
    enum class CullingType { BACK = 0, FRONT = 1, FRONT_AND_BACK = 2 };
    enum class CullingDirection { CLOCKWISE = 0, COUNTERCLOCKWISE = 1 };

    virtual void SetClearColor(const Vector4& color) = 0;
    virtual void Clear(const ClearBuffer clearBuffer) = 0;
    virtual void SetViewport(const int width, const int height) = 0;

    virtual void ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) = 0;
    virtual void ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                         const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                         const StencilTestAction stencilPassDepthPassAction) = 0;
    virtual void ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                   const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) = 0;
    virtual void ConfigureCulling(const bool enable, const CullingType type) = 0;
    virtual void ConfigureWireframeView(const bool enable) = 0;
    virtual void ConfigureAntiAliasing(const bool enable) = 0;

    virtual void DrawIndexed(const ReferencePointer<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

    inline static RendererAPI::API GetAPI() {
        return s_API;
    }

    inline static void SetAPI(RendererAPI::API api) {
        s_API = api;
    }

    static ReferencePointer<RendererAPI> Create();

private:
    static API s_API;
};
} // namespace Hydrogen
