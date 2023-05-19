#include <Hydrogen/Renderer/RenderWindow.hpp>

using namespace Hydrogen;

RendererAPI::API RenderWindow::ChooseRenderingAPI() {
    auto api = RendererAPI::API::OpenGL;

    RendererAPI::SetAPI(api);
    return api;
}
