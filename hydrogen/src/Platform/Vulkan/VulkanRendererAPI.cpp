#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>

#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Math/Math.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;

void VulkanRendererAPI::SetupImGui() {
  // const auto& context = Renderer::GetContext<VulkanContext>();
  // const auto& renderDevice = Renderer::GetRenderDevice<VulkanRenderDevice>();

  // VkDescriptorPoolSize poolSizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
  //                                     {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

  // VkDescriptorPoolCreateInfo createInfo = {};
  // createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  // createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  // createInfo.maxSets = 1000;
  // createInfo.poolSizeCount = std::size(poolSizes);
  // createInfo.pPoolSizes = poolSizes;

  // VkDescriptorPool imguiPool;
  // VK_CHECK_ERROR(vkCreateDescriptorPool(renderDevice->GetDevice(),
  // &createInfo,
  //                                       nullptr, &imguiPool),
  //                "Failed to create imgui descriptor pool");

  // ImGui_ImplVulkan_InitInfo initInfo = {};
  // initInfo.Instance = context->GetInstance();
  //  initInfo.PhysicalDevice = renderDevice->GetPhysicalDevice();
  // initInfo.Device = renderDevice->GetDevice();
  //  initInfo.Queue = renderDevice->GetGraphicsQueue();
  // initInfo.DescriptorPool = imguiPool;
  // initInfo.MinImageCount = 3;
  // initInfo.ImageCount = 3;
  // initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  // ImGui_ImplVulkan_Init(&initInfo, renderDevice->GetRenderPass());
  // HY_INVOKE_ERROR("Unimplemented");
}

void VulkanRendererAPI::ImGuiNewFrame() {}

void VulkanRendererAPI::ImGuiRenderDrawData(ImDrawData* drawData) {}

void VulkanRendererAPI::DestroyImGui() {}
