#pragma once

#include <vector>
#include "RendererAPI.hpp"

namespace Vortex {
class Window {
public:
    virtual ~Window() = default;

    virtual void SetupOpenglContext(int majorVersion, int minorVersion) = 0;
    virtual void* GetWindowOpenGLProcAddress() = 0;

    static VT_API RendererAPI::API ChooseRenderingAPI();
};
} // namespace Vortex
