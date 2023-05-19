#pragma once

#include "../../Renderer/Context.hpp"

namespace Hydrogen::OpenGL {
class OpenGLContext : public Context {
public:
    OpenGLContext(const Reference<RenderWindow>& window, ProjectInformation) : m_Window(window) {
    }
    virtual ~OpenGLContext() = default;

    virtual void Init() override;
    virtual void Destroy() override {
    }

    virtual Reference<RenderWindow> GetWindow() override {
        return m_Window;
    }

private:
    Reference<RenderWindow> m_Window;
};
} // namespace Hydrogen::OpenGL
