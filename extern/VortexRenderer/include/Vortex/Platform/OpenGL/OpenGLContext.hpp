#pragma once

#include "../../Context.hpp"

namespace Vortex::OpenGL {
class OpenGLContext : public Context {
public:
    OpenGLContext(const std::shared_ptr<Vortex::Window>& window, ProjectInformation) : m_Window(window) {
    }
    virtual ~OpenGLContext() = default;

    virtual void Init() override;
    virtual void Destroy() override {
    }

    virtual std::shared_ptr<Vortex::Window> GetWindow() override {
        return m_Window;
    }

private:
    std::shared_ptr<Vortex::Window> m_Window;
};
} // namespace Vortex::OpenGL
