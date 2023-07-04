#pragma once

#include <glm/glm.hpp>

#include "../Event/KeyCodes.hpp"
#include "../Math/Math.hpp"
#include "../Renderer/RenderWindow.hpp"
#include "Memory.hpp"

namespace Hydrogen {
class Window : public RenderWindow {
 public:
  virtual ~Window() = default;

  virtual void SetTitle(const String& title) = 0;
  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  virtual bool GetWindowClose() const = 0;
  virtual bool GetKeyDown(KeyCode key) const = 0;
  virtual bool GetKey(KeyCode key) const = 0;
  virtual bool GetKeyUp(KeyCode key) const = 0;
  virtual bool GetMouseKeyDown(KeyCode key) const = 0;
  virtual bool GetMouseKey(KeyCode key) const = 0;
  virtual bool GetMouseKeyUp(KeyCode key) const = 0;
  virtual uint32_t GetMouseX() const = 0;
  virtual uint32_t GetMouseY() const = 0;
  virtual Vector2 GetMousePos() const = 0;

  virtual void UpdateEvents() = 0;
  virtual void Render() = 0;

  static ReferencePointer<Window> Create(const String& title, uint32_t width,
                                         uint32_t height);
};
}  // namespace Hydrogen
