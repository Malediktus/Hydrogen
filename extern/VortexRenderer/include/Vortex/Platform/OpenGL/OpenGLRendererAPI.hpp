#pragma once

#include "../../RendererAPI.hpp"

namespace Vortex::OpenGL {
class OpenGLRendererAPI : public RendererAPI {
public:
    OpenGLRendererAPI() = default;
    virtual ~OpenGLRendererAPI() = default;

    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear(const ClearBuffer clearBuffer) override;

    virtual void ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) override;
    virtual void ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                         const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                         const StencilTestAction stencilPassDepthPassAction) override;
    virtual void ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                   const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) override;
    virtual void ConfigureCulling(const bool enable, const CullingType type) override;
    virtual void ConfigureWireframeView(const bool enable) override;
    virtual void ConfigureAntiAliasing(const bool enable) override;

    virtual void SetViewport(const int width, const int height) override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) override;
};

void CheckOpenGLError(const char* file, int line);
} // namespace Vortex::OpenGL

#define glCheckError() Vortex::OpenGL::CheckOpenGLError(__FILE__, __LINE__)
