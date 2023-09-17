#pragma once

#include <glm/glm.hpp>

#include "../Events/KeyCodes.hpp"
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

  const HydrogenEvent<uint32_t, uint32_t> GetResizeEvent() { return m_OnViewportResizeEvent; }
  const HydrogenEvent<const String&> GetTitleChangeEvent() { return m_OnTitleChangeEvent; }
  const HydrogenEvent<> GetWindowCloseEvent() { return m_OnWindowCloseEvent; }
  const HydrogenEvent<KeyCode> GetKeyDownEvent() { return m_OnKeyDownEvent; }
  const HydrogenEvent<KeyCode> GetKeyEvent() { return m_OnKeyEvent; }
  const HydrogenEvent<KeyCode> GetKeyUpEvent() { return m_OnKeyUpEvent; }
  const HydrogenEvent<KeyCode> GetMouseKeyDownEvent() { return m_OnMouseKeyDownEvent; }
  const HydrogenEvent<KeyCode> GetMouseKeyEvent() { return m_OnMouseKeyEvent; }
  const HydrogenEvent<KeyCode> GetMouseKeyUpEvent() { return m_OnMouseKeyUpEvent; }
  const HydrogenEvent<uint32_t, uint32_t> GetMouseMoveEvent() { return m_OnMouseMoveEvent; }

  static ReferencePointer<Window> Create(const String& title, uint32_t width, uint32_t height);

 protected:
  static Window* s_MainWindow;
  HydrogenEvent<uint32_t, uint32_t> m_OnViewportResizeEvent;
  HydrogenEvent<const String&> m_OnTitleChangeEvent;
  HydrogenEvent<> m_OnWindowCloseEvent;
  HydrogenEvent<KeyCode> m_OnKeyDownEvent;
  HydrogenEvent<KeyCode> m_OnKeyEvent;
  HydrogenEvent<KeyCode> m_OnKeyUpEvent;
  HydrogenEvent<KeyCode> m_OnMouseKeyDownEvent;
  HydrogenEvent<KeyCode> m_OnMouseKeyEvent;
  HydrogenEvent<KeyCode> m_OnMouseKeyUpEvent;
  HydrogenEvent<uint32_t, uint32_t> m_OnMouseMoveEvent;
  // TODO: Mouse wheel
  // TODO: Controller
};
}  // namespace Hydrogen
