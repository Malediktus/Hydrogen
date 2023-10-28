#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/RendererAPI.hpp"

namespace Hydrogen::Vulkan {
class VulkanRendererAPI : public RendererAPI {
 public:
  VulkanRendererAPI(const ReferencePointer<class RenderWindow>& window);
  virtual ~VulkanRendererAPI() = default;

  virtual void SetupImGui() override;
  virtual void ImGuiNewFrame() override;
  virtual void DestroyImGui() override;

 private:
  ReferencePointer<class RenderWindow> m_Window;

  VkDescriptorPool m_ImGuiPool;
};
}  // namespace Hydrogen::Vulkan
