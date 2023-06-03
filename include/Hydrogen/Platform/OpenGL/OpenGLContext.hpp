#pragma once

#include "../../Renderer/Context.hpp"

namespace Hydrogen::OpenGL {
class OpenGLContext : public Context {
public:
    OpenGLContext(const ReferencePointer<RenderWindow>& window, ProjectInformation clientInfo, ProjectInformation engineInfo);
    virtual ~OpenGLContext() = default;

    virtual ReferencePointer<RenderWindow> GetWindow() override {
        return m_Window;
    }

private:
    ReferencePointer<RenderWindow> m_Window;
};
} // namespace Hydrogen::OpenGL
