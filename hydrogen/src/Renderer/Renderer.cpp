#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <tracy/Tracy.hpp>
#include <unordered_map>

using namespace Hydrogen;

ReferencePointer<Context> Renderer::s_Context;

Renderer::Renderer() {
    ZoneScoped;

    HY_LOG_INFO("Initialized renderer");
}
