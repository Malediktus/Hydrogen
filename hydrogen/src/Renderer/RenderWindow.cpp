#include <Hydrogen/Renderer/RenderWindow.hpp>

using namespace Hydrogen;

RendererAPI::API RenderWindow::ChooseRenderingAPI(bool vulkanAvailable) {
    auto api = RendererAPI::API::OpenGL;
    // if (vulkanAvailable)
    //     api = RendererAPI::API::Vulkan;

    RendererAPI::SetAPI(api);
    return api;
}
