#pragma once

#include "../Event/EventManager.hpp"
#include "../Math/Math.hpp"
#include "../Renderer/Context.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Scene/Scene.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "Window.hpp"

namespace Hydrogen {
class Application {
 public:
  Application();
  virtual ~Application() = default;
  void Run();

  virtual void OnSetup() = 0;
  virtual void OnInit() = 0;
  virtual void OnShutdown() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnImGuiDraw() = 0;
  virtual void OnEvent(const Event& event) = 0;

 protected:
  ReferencePointer<Logger> Console;
  ReferencePointer<Window> AppWindow;
  ReferencePointer<Scene> CurrentScene;
  struct _ApplicationInfo {
    String Name;
    Vector3 Version;
    Vector2 WindowSize;
  } ApplicationInfo;

 private:
  bool m_Initialized = false;
  void OnResize(const Event& event);

  ReferencePointer<Context> m_RenderContext;
};

extern ReferencePointer<Application> CreateApplication();
}  // namespace Hydrogen
