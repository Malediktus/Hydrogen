#pragma once

#include "VertexArray.hpp"
#include "Core.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <cstdint>

namespace Vortex {
/**
 * @brief This is a rendering abstraction and should not be used by the client because its realy low level.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
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

    virtual VT_API void SetClearColor(const glm::vec4& color) = 0;
    virtual VT_API void Clear(const ClearBuffer clearBuffer) = 0;
    virtual VT_API void SetViewport(const int width, const int height) = 0;

    virtual VT_API void ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) = 0;
    virtual VT_API void ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                                const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                                const StencilTestAction stencilPassDepthPassAction) = 0;
    virtual VT_API void ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                          const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) = 0;
    virtual VT_API void ConfigureCulling(const bool enable, const CullingType type) = 0;
    virtual VT_API void ConfigureWireframeView(const bool enable) = 0;
    virtual VT_API void ConfigureAntiAliasing(const bool enable) = 0;

    virtual VT_API void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

    inline static VT_API RendererAPI::API GetAPI() {
        return s_API;
    }

    inline static VT_API void SetAPI(RendererAPI::API api) {
        s_API = api;
    }

private:
    static API s_API;
};

/**
 * @brief This function is the only way that should be used for creating a RendererAPI.
 *
 * @return std::shared_ptr<RendererAPI>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<RendererAPI> RendererAPICreate();
} // namespace Vortex
