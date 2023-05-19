#pragma once

#include "Memory.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "../Scene/Scene.hpp"
#include "../Event/EventManager.hpp"
#include "../Renderer/Context.hpp"
#include "../Renderer/Renderer.hpp"

#include <string>
#include <glm/glm.hpp>

namespace Hydrogen {
class Application {
public:
    Application();
    virtual ~Application() = default;
    void Run();

    virtual void OnSetup() = 0;
    virtual void OnInit() = 0;
    virtual void OnShutdown() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnEvent(const Event& event) = 0;

protected:
    Reference<Logger> Console;
    Reference<Window> AppWindow;
    Reference<Scene> CurrentScene;
    struct _ApplicationInfo {
        std::string Name;
        glm::vec3 Version;
        glm::vec2 WindowSize;
    } ApplicationInfo;

private:
    Reference<Context> m_RenderContext;
    Reference<Renderer> m_WindowRenderer;
};

extern Reference<Application> CreateApplication();
} // namespace Hydrogen
