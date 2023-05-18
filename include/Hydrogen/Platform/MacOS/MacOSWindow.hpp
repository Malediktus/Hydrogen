#pragma once

#include "../../Core/Window.hpp"
#include <GLFW/glfw3.h>
#include <string>

namespace Hydrogen {
class MacOSWindow : public Window {
public:
    MacOSWindow(const std::string& title, uint32_t width, uint32_t height);
    ~MacOSWindow();

    void SetTitle(const std::string& title) override;
    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;
    glm::vec2 GetViewportSize() const override;

    bool GetWindowClose() const override;
    bool GetKeyDown(KeyCode key) const override;
    bool GetKey(KeyCode key) const override;
    bool GetKeyUp(KeyCode key) const override;
    bool GetMouseKeyDown(KeyCode key) const override;
    bool GetMouseKey(KeyCode key) const override;
    bool GetMouseKeyUp(KeyCode key) const override;
    uint32_t GetMouseX() const override;
    uint32_t GetMouseY() const override;
    glm::vec2 GetMousePos() const override;

    void UpdateEvents() override;
    void Render() override;

    void SetupOpenglContext() override;

private:
    GLFWwindow* m_Window;
};
} // namespace Hydrogen
