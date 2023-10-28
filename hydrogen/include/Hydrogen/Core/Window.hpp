#pragma once

#include <glm/glm.hpp>

#include "../Events/EventSystem.hpp"
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
  virtual bool GetKeyDown(enum KeyCode key) const = 0;
  virtual bool GetKey(enum KeyCode key) const = 0;
  virtual bool GetKeyUp(enum KeyCode key) const = 0;
  virtual bool GetMouseKeyDown(enum KeyCode key) const = 0;
  virtual bool GetMouseKey(enum KeyCode key) const = 0;
  virtual bool GetMouseKeyUp(enum KeyCode key) const = 0;
  virtual uint32_t GetMouseX() const = 0;
  virtual uint32_t GetMouseY() const = 0;
  virtual Vector2 GetMousePos() const = 0;

  virtual void UpdateEvents() = 0;
  virtual void Render() = 0;

  const HydrogenEvent<uint32_t, uint32_t> GetResizeEvent() { return m_OnViewportResizeEvent; }
  const HydrogenEvent<const String&> GetTitleChangeEvent() { return m_OnTitleChangeEvent; }
  const HydrogenEvent<> GetWindowCloseEvent() { return m_OnWindowCloseEvent; }
  const HydrogenEvent<enum KeyCode> GetKeyDownEvent() { return m_OnKeyDownEvent; }
  const HydrogenEvent<enum KeyCode> GetKeyEvent() { return m_OnKeyEvent; }
  const HydrogenEvent<enum KeyCode> GetKeyUpEvent() { return m_OnKeyUpEvent; }
  const HydrogenEvent<enum KeyCode> GetMouseKeyDownEvent() { return m_OnMouseKeyDownEvent; }
  const HydrogenEvent<enum KeyCode> GetMouseKeyEvent() { return m_OnMouseKeyEvent; }
  const HydrogenEvent<enum KeyCode> GetMouseKeyUpEvent() { return m_OnMouseKeyUpEvent; }
  const HydrogenEvent<uint32_t, uint32_t> GetMouseMoveEvent() { return m_OnMouseMoveEvent; }

  static ReferencePointer<Window> Create(const String& title, uint32_t width, uint32_t height);

 protected:
  static Window* s_MainWindow;
  HydrogenEvent<uint32_t, uint32_t> m_OnViewportResizeEvent;
  HydrogenEvent<const String&> m_OnTitleChangeEvent;
  HydrogenEvent<> m_OnWindowCloseEvent;
  HydrogenEvent<enum KeyCode> m_OnKeyDownEvent;
  HydrogenEvent<enum KeyCode> m_OnKeyEvent;
  HydrogenEvent<enum KeyCode> m_OnKeyUpEvent;
  HydrogenEvent<enum KeyCode> m_OnMouseKeyDownEvent;
  HydrogenEvent<enum KeyCode> m_OnMouseKeyEvent;
  HydrogenEvent<enum KeyCode> m_OnMouseKeyUpEvent;
  HydrogenEvent<uint32_t, uint32_t> m_OnMouseMoveEvent;
  // TODO: Mouse wheel
  // TODO: Controller
};
}  // namespace Hydrogen
