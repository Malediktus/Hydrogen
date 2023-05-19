#pragma once

#include "RendererAPI.hpp"
#include "Core.hpp"

namespace Vortex {
/**
 * @brief This is an abstraction layer for rendering. The relevant functions are SetClearColor() and Clear(). Init() is handled
 * automaticly by Renderer::Init().
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class RenderCommand {
public:
    static VT_API void Init() {
        s_RendererAPI = RendererAPICreate();
    }

    inline static VT_API void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    inline static VT_API void SetViewport(const int width, const int height) {
        s_RendererAPI->SetViewport(width, height);
    }

    inline static VT_API void Clear(const RendererAPI::ClearBuffer clearBuffer) {
        s_RendererAPI->Clear(clearBuffer);
    }

    inline static VT_API void ConfigureDepthTesting(const bool enable, const bool depthMask, const RendererAPI::DepthTestFunc func) {
        s_RendererAPI->ConfigureDepthTesting(enable, depthMask, func);
    }

    inline static VT_API void ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const RendererAPI::StencilTestFunc func, const int ref,
                                                      const RendererAPI::StencilTestAction stencilFailAction, const RendererAPI::StencilTestAction stencilPassDepthFailAction,
                                                      const RendererAPI::StencilTestAction stencilPassDepthPassAction) {
        s_RendererAPI->ConfigureStencilTesting(enable, writeMask, readMask, func, ref, stencilFailAction, stencilPassDepthFailAction, stencilPassDepthPassAction);
    }

    inline static VT_API void ConfigureBlending(const bool enable, const RendererAPI::BlendingFunc blendingFunc1, const RendererAPI::BlendingFunc blendingFunc2,
                                                const RendererAPI::BlendingFunc blendingFuncR, const RendererAPI::BlendingFunc blendingFuncG,
                                                const RendererAPI::BlendingFunc blendingFuncB, const RendererAPI::BlendingFunc blendingFuncA) {
        s_RendererAPI->ConfigureBlending(enable, blendingFunc1, blendingFunc2, blendingFuncR, blendingFuncG, blendingFuncB, blendingFuncA);
    }

    inline static VT_API void ConfigureCulling(const bool enable, const RendererAPI::CullingType type) {
        s_RendererAPI->ConfigureCulling(enable, type);
    }

    inline static VT_API void ConfigureWireframeView(const bool enable) {
        s_RendererAPI->ConfigureWireframeView(enable);
    }

    inline static VT_API void ConfigureAntiAliasing(const bool enable) {
        s_RendererAPI->ConfigureAntiAliasing(enable);
    }

    inline static VT_API void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) {
        s_RendererAPI->DrawIndexed(vertexArray, count);
    }

private:
    static VT_API std::shared_ptr<RendererAPI> s_RendererAPI;
};
} // namespace Vortex
