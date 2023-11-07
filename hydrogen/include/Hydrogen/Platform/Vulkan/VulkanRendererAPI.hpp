#pragma once

#include <vulkan/vulkan.h>

#include "../../Renderer/RendererAPI.hpp"

namespace Hydrogen::Vulkan {
class VulkanRendererAPI : public RendererAPI {
 public:
  VulkanRendererAPI(const ReferencePointer<class RenderWindow>& window);
  virtual ~VulkanRendererAPI();

  virtual void SetupImGui() override;
  virtual void ImGuiNewFrame() override;
  virtual void DestroyImGui() override;

  virtual void BeginFrame() override;
  virtual void EndFrame() override;
  virtual void DrawIndexed(const ReferencePointer<class VertexArray>& vertexArray) override;
  virtual void SetViewport(uint32_t width = UINT32_MAX, uint32_t height = UINT32_MAX) override;
  virtual void SetScissor(int offsetX = 0, int offsetY = 0) override;

  VkCommandBuffer GetCommandBuffer() { return m_CommandBuffer; }

 private:
  ReferencePointer<class RenderWindow> m_Window;

  VkDescriptorPool m_ImGuiPool;
  VkCommandBuffer m_CommandBuffer;
  VkSemaphore m_ImageAvailableSemaphore;
  VkSemaphore m_RenderFinishedSemaphore;
  VkFence m_InFlightFence;
  uint32_t m_ImageIndex;
};
}  // namespace Hydrogen::Vulkan
