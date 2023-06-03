#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "RenderWindow.hpp"

#include <memory>

namespace Hydrogen {
struct ProjectInformation {
    String ProjectName;
    Vector3 ProjectVersion;
};

class Context {
public:
    ~Context() = default;

    virtual ReferencePointer<RenderWindow> GetWindow() = 0;

    static ReferencePointer<Context> Create(const ReferencePointer<RenderWindow>& window, ProjectInformation clientInfo, ProjectInformation engineInfo);
};
} // namespace Hydrogen
