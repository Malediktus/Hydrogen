#pragma once

#include "RendererAPI.hpp"
#include <vector>

namespace Hydrogen {
class RenderWindow {
public:
    virtual ~RenderWindow() = default;

    virtual void SetupOpenglContext(int majorVersion, int minorVersion) = 0;
    virtual void* GetWindowOpenGLProcAddress() = 0;

    static RendererAPI::API ChooseRenderingAPI(bool vulkanAvailable);
};
} // namespace Hydrogen
