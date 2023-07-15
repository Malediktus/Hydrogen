#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RendererAPI.hpp"

namespace Hydrogen {
class RenderCommand {
 public:
  static void Init() { s_RendererAPI = RendererAPI::Create(); }

  inline static void SetClearColor(const Vector4& color) { s_RendererAPI->SetClearColor(color); }

  inline static void SetViewport(const int width, const int height) { s_RendererAPI->SetViewport(width, height); }

  inline static void Clear(const RendererAPI::ClearBuffer clearBuffer) { s_RendererAPI->Clear(clearBuffer); }

  inline static void ConfigureDepthTesting(const bool enable, const bool depthMask, const RendererAPI::DepthTestFunc func) {
    s_RendererAPI->ConfigureDepthTesting(enable, depthMask, func);
  }

  inline static void ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const RendererAPI::StencilTestFunc func, const int ref,
                                             const RendererAPI::StencilTestAction stencilFailAction, const RendererAPI::StencilTestAction stencilPassDepthFailAction,
                                             const RendererAPI::StencilTestAction stencilPassDepthPassAction) {
    s_RendererAPI->ConfigureStencilTesting(enable, writeMask, readMask, func, ref, stencilFailAction, stencilPassDepthFailAction, stencilPassDepthPassAction);
  }

  inline static void ConfigureBlending(const bool enable, const RendererAPI::BlendingFunc blendingFunc1, const RendererAPI::BlendingFunc blendingFunc2,
                                       const RendererAPI::BlendingFunc blendingFuncR, const RendererAPI::BlendingFunc blendingFuncG, const RendererAPI::BlendingFunc blendingFuncB,
                                       const RendererAPI::BlendingFunc blendingFuncA) {
    s_RendererAPI->ConfigureBlending(enable, blendingFunc1, blendingFunc2, blendingFuncR, blendingFuncG, blendingFuncB, blendingFuncA);
  }

  inline static void ConfigureCulling(const bool enable, const RendererAPI::CullingType type) { s_RendererAPI->ConfigureCulling(enable, type); }

  inline static void ConfigureWireframeView(const bool enable) { s_RendererAPI->ConfigureWireframeView(enable); }

  inline static void ConfigureAntiAliasing(const bool enable) { s_RendererAPI->ConfigureAntiAliasing(enable); }

  inline static void DrawIndexed(const ReferencePointer<VertexArray>& vertexArray, uint32_t count = 0) { s_RendererAPI->DrawIndexed(vertexArray, count); }

  static ReferencePointer<RendererAPI> GetRendererAPI() { return s_RendererAPI; }

 private:
  static ReferencePointer<RendererAPI> s_RendererAPI;
};
}  // namespace Hydrogen
