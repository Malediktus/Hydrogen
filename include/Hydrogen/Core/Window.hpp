#pragma once

#include "Memory.hpp"
#include "../Event/KeyCodes.hpp"

#include <glm/glm.hpp>

namespace Hydrogen {
class Window {
public:
    virtual ~Window() = default;

    virtual void SetTitle(const std::string& title) = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual glm::vec2 GetViewportSize() const = 0;

    virtual bool GetWindowClose() const = 0;
    virtual bool GetKeyDown(KeyCode key) const = 0;
    virtual bool GetKey(KeyCode key) const = 0;
    virtual bool GetKeyUp(KeyCode key) const = 0;
    virtual bool GetMouseKeyDown(KeyCode key) const = 0;
    virtual bool GetMouseKey(KeyCode key) const = 0;
    virtual bool GetMouseKeyUp(KeyCode key) const = 0;
    virtual uint32_t GetMouseX() const = 0;
    virtual uint32_t GetMouseY() const = 0;
    virtual glm::vec2 GetMousePos() const = 0;

    static Reference<Window> Create(const std::string& title, uint32_t width, uint32_t height);
};
} // namespace Hydrogen
