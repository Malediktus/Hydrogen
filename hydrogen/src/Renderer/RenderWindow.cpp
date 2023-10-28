#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Renderer/RenderWindow.hpp>

using namespace Hydrogen;

RendererAPI::API RenderWindow::ChooseRenderingAPI(bool vulkanAvailable) {
  HY_ASSERT(vulkanAvailable, "Vulkan API is required");

  auto api = RendererAPI::API::Vulkan;
  RendererAPI::SetAPI(api);
  return api;
}
