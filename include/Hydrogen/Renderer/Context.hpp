#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "RenderWindow.hpp"

#include <memory>

namespace Hydrogen {
struct ProjectInformation {
    std::string ProjectName;
    glm::vec3 ProjectVersion;
};

class Context {
public:
    ~Context() = default;

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    virtual Reference<RenderWindow> GetWindow() = 0;

    static Reference<Context> Create(const Reference<RenderWindow>& window, ProjectInformation projectInfo);
};
} // namespace Hydrogen
