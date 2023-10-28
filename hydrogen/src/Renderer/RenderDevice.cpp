#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Renderer/RenderDevice.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<RenderDevice> RenderDevice::Create(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanRenderDevice>(deviceRateFunction);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
