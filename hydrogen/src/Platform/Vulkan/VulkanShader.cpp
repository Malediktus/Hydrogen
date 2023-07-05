#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Core/Memory.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanShader.hpp>
#include <array>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <tracy/Tracy.hpp>
#include <vector>

using namespace Hydrogen::Vulkan;
using namespace Hydrogen;

VulkanShader::VulkanShader(const String& name,
                           const DynamicArray<uint32_t>& vertexSrc,
                           const DynamicArray<uint32_t>& fragmentSrc,
                           const DynamicArray<uint32_t>& geometrySrc)
    : m_Name(name) {
  ZoneScoped;

  DynamicArray<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(3);

  if (vertexSrc.size() > 0) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = vertexSrc.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(vertexSrc.data());

    VK_CHECK_ERROR(
        vkCreateShaderModule(
            Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(),
            &createInfo, nullptr, &m_VertexShaderModule),
        "Failed to create vulkan shader module!");

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = m_VertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    shaderStageCreateInfos.push_back(vertexShaderStageInfo);
  }

  if (fragmentSrc.size() > 0) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = fragmentSrc.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentSrc.data());

    VK_CHECK_ERROR(
        vkCreateShaderModule(
            Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(),
            &createInfo, nullptr, &m_FragmentShaderModule),
        "Failed to create vulkan shader module!");

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = m_FragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    shaderStageCreateInfos.push_back(fragmentShaderStageInfo);
  }

  if (geometrySrc.size() > 0) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = geometrySrc.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(geometrySrc.data());

    VK_CHECK_ERROR(
        vkCreateShaderModule(
            Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(),
            &createInfo, nullptr, &m_GeometryShaderModule),
        "Failed to create vulkan shader module!");

    VkPipelineShaderStageCreateInfo geometryShaderStageInfo{};
    geometryShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    geometryShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
    geometryShaderStageInfo.module = m_GeometryShaderModule;
    geometryShaderStageInfo.pName = "main";

    shaderStageCreateInfos.push_back(geometryShaderStageInfo);
  }
}

VulkanShader::~VulkanShader() {
  ZoneScoped;
  if (m_VertexShaderModule != VK_NULL_HANDLE)
    vkDestroyShaderModule(
        Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(),
        m_VertexShaderModule, nullptr);
  if (m_FragmentShaderModule != VK_NULL_HANDLE)
    vkDestroyShaderModule(
        Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(),
        m_FragmentShaderModule, nullptr);
  if (m_GeometryShaderModule != VK_NULL_HANDLE)
    vkDestroyShaderModule(
        Renderer::GetRenderDevice<VulkanRenderDevice>()->GetDevice(),
        m_GeometryShaderModule, nullptr);
}

void VulkanShader::Bind() const { ZoneScoped; }

void VulkanShader::Unbind() const { ZoneScoped; }
