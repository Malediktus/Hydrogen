#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Math/Math.hpp>
#include <tracy/Tracy.hpp>

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
