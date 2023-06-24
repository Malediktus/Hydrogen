#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Math/Math.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <backends/imgui_impl_vulkan.h>
#include <tracy/Tracy.hpp>
#include <vulkan/vulkan.h>

using namespace Hydrogen::Vulkan;

void VulkanRendererAPI::SetClearColor(const Vector4& color) {
    ZoneScoped;
    HY_LOG_TRACE("Set Vulkan clear color to ({}, {}, {}, {})", color.r, color.g, color.b, color.a);
}

void VulkanRendererAPI::Clear(const ClearBuffer clearBuffer) {
    ZoneScoped;
}

void VulkanRendererAPI::SetViewport(const int width, const int height) {
    ZoneScoped;
    HY_LOG_TRACE("Set Vulkan viewport size to ({}, {})", width, height);
}

void VulkanRendererAPI::ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) {
    ZoneScoped;
    HY_LOG_TRACE("Configured Vulkan depth testing: (enable: {}, mask: {}, func: {})", enable, depthMask, (int) func);
}

void VulkanRendererAPI::ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                                const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                                const StencilTestAction stencilPassDepthPassAction) {
    ZoneScoped;
    HY_LOG_TRACE(
        "Configured Vulkan stencil testing: (enable: {}, writeMask: {}, readMask: {}, ref: {}, stencilFailAction: {}, stencilPassDepthFailAction: {}, stencilPassDepthPassAction: {})",
        enable, writeMask, readMask, ref, (int) stencilFailAction, (int) stencilPassDepthFailAction, (int) stencilPassDepthPassAction);
}

void VulkanRendererAPI::ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                          const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) {
    ZoneScoped;
    HY_LOG_TRACE(
        "Configured Vulkan blending testing: (enable: {}, blendingFunc1: {}, blendingFunc2: {}, blendingFuncR: {}, blendingFuncG: {}, blendingFuncB: {}, blendingFuncA: {})",
        enable, (int) blendingFunc1, (int) blendingFunc2, (int) blendingFuncR, (int) blendingFuncG, (int) blendingFuncB, (int) blendingFuncA);
}

void VulkanRendererAPI::ConfigureCulling(const bool enable, const CullingType type) {
    ZoneScoped;
    HY_LOG_TRACE("Configured Vulkan culling: (enable: {}, type: {})", enable, (int) type);
}

void VulkanRendererAPI::ConfigureWireframeView(const bool enable) {
}

void VulkanRendererAPI::ConfigureAntiAliasing(const bool enable) {
}

void VulkanRendererAPI::SetupImGui() {
    const auto& context = Renderer::GetContext<VulkanContext>();

    VkDescriptorPoolSize poolSizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                                        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                                        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                                        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                                        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                                        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                                        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    VkDescriptorPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    createInfo.maxSets = 1000;
    createInfo.poolSizeCount = std::size(poolSizes);
    createInfo.pPoolSizes = poolSizes;

    VkDescriptorPool imguiPool;
    VK_CHECK_ERROR(vkCreateDescriptorPool(context->GetDevice(), &createInfo, nullptr, &imguiPool), "Failed to create imgui descriptor pool");

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = context->GetInstance();
    initInfo.PhysicalDevice = context->GetPhysicalDevice();
    initInfo.Device = context->GetDevice();
    initInfo.Queue = context->GetGraphicsQueue();
    initInfo.DescriptorPool = imguiPool;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    // ImGui_ImplVulkan_Init(&initInfo, context->GetRenderPass());
    HY_INVOKE_ERROR("Unimplemented");
}

void VulkanRendererAPI::ImGuiNewFrame() {
}

void VulkanRendererAPI::ImGuiRenderDrawData(ImDrawData* drawData) {
}

void VulkanRendererAPI::DestroyImGui() {
}

void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
    ZoneScoped;
    HY_LOG_TRACE("Drew Vulkan indexed vertex array (elements: {})", 0);
}
