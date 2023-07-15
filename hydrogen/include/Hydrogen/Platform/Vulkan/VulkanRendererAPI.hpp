#pragma once

#include "../../Renderer/RendererAPI.hpp"

namespace Hydrogen::Vulkan {
class VulkanRendererAPI : public RendererAPI {
 public:
  VulkanRendererAPI() = default;
  virtual ~VulkanRendererAPI() = default;

  virtual void SetClearColor(const Vector4& color) override;
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

  virtual void SetupImGui() override;
  virtual void ImGuiNewFrame() override;
  virtual void ImGuiRenderDrawData(ImDrawData* drawData) override;

  virtual void DestroyImGui() override;

  virtual void DrawIndexed(const ReferencePointer<VertexArray>& vertexArray, uint32_t indexCount) override;
};
}  // namespace Hydrogen::Vulkan
