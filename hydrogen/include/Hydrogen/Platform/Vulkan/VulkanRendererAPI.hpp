#pragma once

#include "../../Renderer/RendererAPI.hpp"

namespace Hydrogen::Vulkan {
class VulkanRendererAPI : public RendererAPI {
 public:
  VulkanRendererAPI() = default;
  virtual ~VulkanRendererAPI() = default;

  virtual void SetupImGui() override;
  virtual void ImGuiNewFrame() override;
  virtual void ImGuiRenderDrawData(ImDrawData* drawData) override;
  virtual void DestroyImGui() override;
};
}  // namespace Hydrogen::Vulkan
