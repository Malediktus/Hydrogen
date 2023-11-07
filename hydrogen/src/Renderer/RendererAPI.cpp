#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Renderer/RendererAPI.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<RendererAPI> RendererAPI::s_Instance = nullptr;
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

ReferencePointer<RendererAPI> RendererAPI::Create(const ReferencePointer<class RenderWindow>& window) {
  ZoneScoped;
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::Vulkan: {
      auto reference = NewReferencePointer<Vulkan::VulkanRendererAPI>(window);
      RendererAPI::s_Instance = reference;
      return reference;
    }
    default:
      HY_ASSERT_CHECK(false,
                      "Invalid renderer API value returned from "
                      "Renderer::GetRendererAPI()");
  }
  return nullptr;
}
