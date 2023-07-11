#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <tracy/Tracy.hpp>
#include <unordered_map>

using namespace Hydrogen;

ReferencePointer<Context> Renderer::s_Context;
ReferencePointer<RenderDevice> Renderer::s_RenderDevice;
ReferencePointer<SwapChain> Renderer::s_SwapChain;

Renderer::Renderer() {
  ZoneScoped;

  HY_LOG_INFO("Initialized renderer");
}
