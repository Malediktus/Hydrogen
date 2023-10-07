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
  virtual bool GetKeyDown(enum class KeyCode key) const = 0;
  virtual bool GetKey(enum class KeyCode key) const = 0;
  virtual bool GetKeyUp(enum class KeyCode key) const = 0;
  virtual bool GetMouseKeyDown(enum class KeyCode key) const = 0;
  virtual bool GetMouseKey(enum class KeyCode key) const = 0;
  virtual bool GetMouseKeyUp(enum class KeyCode key) const = 0;
  virtual uint32_t GetMouseX() const = 0;
  virtual uint32_t GetMouseY() const = 0;
  virtual Vector2 GetMousePos() const = 0;

  virtual void UpdateEvents() = 0;
  virtual void Render() = 0;

  const HydrogenEvent<uint32_t, uint32_t> GetResizeEvent() { return m_OnViewportResizeEvent; }
  const HydrogenEvent<const String&> GetTitleChangeEvent() { return m_OnTitleChangeEvent; }
  const HydrogenEvent<> GetWindowCloseEvent() { return m_OnWindowCloseEvent; }
  const HydrogenEvent<enum class KeyCode> GetKeyDownEvent() { return m_OnKeyDownEvent; }
  const HydrogenEvent<enum class KeyCode> GetKeyEvent() { return m_OnKeyEvent; }
  const HydrogenEvent<enum class KeyCode> GetKeyUpEvent() { return m_OnKeyUpEvent; }
  const HydrogenEvent<enum class KeyCode> GetMouseKeyDownEvent() { return m_OnMouseKeyDownEvent; }
  const HydrogenEvent<enum class KeyCode> GetMouseKeyEvent() { return m_OnMouseKeyEvent; }
  const HydrogenEvent<enum class KeyCode> GetMouseKeyUpEvent() { return m_OnMouseKeyUpEvent; }
  const HydrogenEvent<uint32_t, uint32_t> GetMouseMoveEvent() { return m_OnMouseMoveEvent; }

  static ReferencePointer<Window> Create(const String& title, uint32_t width, uint32_t height);

 protected:
  static Window* s_MainWindow;
  HydrogenEvent<uint32_t, uint32_t> m_OnViewportResizeEvent;
  HydrogenEvent<const String&> m_OnTitleChangeEvent;
  HydrogenEvent<> m_OnWindowCloseEvent;
  HydrogenEvent<enum class KeyCode> m_OnKeyDownEvent;
  HydrogenEvent<enum class KeyCode> m_OnKeyEvent;
  HydrogenEvent<enum class KeyCode> m_OnKeyUpEvent;
  HydrogenEvent<enum class KeyCode> m_OnMouseKeyDownEvent;
  HydrogenEvent<enum class KeyCode> m_OnMouseKeyEvent;
  HydrogenEvent<enum class KeyCode> m_OnMouseKeyUpEvent;
  HydrogenEvent<uint32_t, uint32_t> m_OnMouseMoveEvent;
  // TODO: Mouse wheel
  // TODO: Controller
};
}  // namespace Hydrogen
