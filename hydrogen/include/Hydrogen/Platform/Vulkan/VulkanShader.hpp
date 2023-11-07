#pragma once

#include <vulkan/vulkan.h>

#include <unordered_map>

#include "../../Core/Memory.hpp"
#include "../../Renderer/Shader.hpp"

namespace Hydrogen::Vulkan {
class VulkanShader : public Shader {
 public:
  VulkanShader(const ReferencePointer<RenderWindow>& window, const BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph, const String& name,
               const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc);
  virtual ~VulkanShader();

  virtual void SetBuffer(const ReferencePointer<class UniformBuffer>& buffer, uint32_t location) override;
  virtual void SetTexture(const ReferencePointer<class Texture2D>& texture, uint32_t location) override;
  virtual void Bind() const override;

  virtual const String& GetName() const override { return m_Name; }
  VkPipeline GetPipeline() { return m_Pipeline; }

 private:
  String m_Name;
  bool m_HasDependencies;
  ReferencePointer<RenderWindow> m_Window;
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
