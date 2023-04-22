#pragma once

#include "Memory.hpp"
#include "Logger.hpp"
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

protected:
    Reference<Logger> Console;
    struct _ApplicationInfo {
        std::string Name;
        glm::vec3 Version;
    } ApplicationInfo;
};

extern Reference<Application> CreateApplication();
} // namespace Hydrogen
