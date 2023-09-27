#pragma once

#include <unordered_map>

#include "../../Math/Math.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Renderer/Shader.hpp"
#include "VulkanContext.hpp"
#include "VulkanRenderDevice.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanSwapChain.hpp"

typedef unsigned int GLenum;

namespace Hydrogen::Vulkan {
class VulkanShader : public Shader {
 public:
  VulkanShader(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
               const BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph, const String& name, const DynamicArray<uint32_t>& vertexSrc,
               const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc);
  virtual ~VulkanShader();

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const override;

  virtual const String& GetName() const override { return m_Name; }
  VkPipeline GetPipeline() { return m_Pipeline; }

 private:
  String m_Name;
  bool m_HasDependencies;
  ReferencePointer<VulkanRenderDevice> m_RenderDevice;
  ReferencePointer<VulkanSwapChain> m_SwapChain;
  ReferencePointer<VulkanFramebuffer> m_Framebuffer;
  VkShaderModule m_VertexShaderModule;
  VkShaderModule m_FragmentShaderModule;
  VkShaderModule m_GeometryShaderModule;
  VkPipelineLayout m_PipelineLayout;
  VkPipeline m_Pipeline;
  VkDescriptorSetLayout m_DescriptorSetLayout;
  VkDescriptorPool m_DescriptorPool;
  VkDescriptorSet m_DescriptorSet;
};
}  // namespace Hydrogen::Vulkan
