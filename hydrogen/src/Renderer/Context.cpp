#include <Hydrogen/Platform/Vulkan/VulkanContext.hpp>
#include <Hydrogen/Renderer/Context.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Assert.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Context> Context::Create(const ReferencePointer<Hydrogen::RenderWindow>& mainWindow) {
  ZoneScoped;

  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan:
      return NewReferencePointer<Vulkan::VulkanContext>(mainWindow);
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
