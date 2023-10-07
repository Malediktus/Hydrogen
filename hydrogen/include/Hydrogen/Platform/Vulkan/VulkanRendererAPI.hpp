#pragma once

#include "../../Renderer/RendererAPI.hpp"

#include <vulkan/vulkan.h>

namespace Hydrogen::Vulkan {
class VulkanRendererAPI : public RendererAPI {
 public:
  VulkanRendererAPI(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class Framebuffer>& framebuffer);
  virtual ~VulkanRendererAPI() = default;

  virtual void SetupImGui() override;
  virtual void ImGuiNewFrame() override;
  virtual void DestroyImGui() override;

 private:
  ReferencePointer<class VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<class VulkanFramebuffer> m_Framebuffer;

  VkDescriptorPool m_ImGuiPool;
};
}  // namespace Hydrogen::Vulkan
