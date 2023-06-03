#pragma once

#include "../../Renderer/Context.hpp"

namespace Hydrogen::OpenGL {
class OpenGLContext : public Context {
public:
    OpenGLContext(const ReferencePointer<RenderWindow>& window);
    virtual ~OpenGLContext() = default;

    virtual void Init(ProjectInformation clientInfo, ProjectInformation engineInfo) override;
    virtual ReferencePointer<RenderWindow> GetWindow() override {
        return m_Window;
    }

private:
    ReferencePointer<RenderWindow> m_Window;
};
} // namespace Hydrogen::OpenGL
