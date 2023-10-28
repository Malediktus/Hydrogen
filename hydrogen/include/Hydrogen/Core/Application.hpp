#pragma once

#include "../Math/Math.hpp"
#include "../Renderer/RenderDevice.hpp"
#include "Memory.hpp"

namespace Hydrogen {
class Logger;
class Window;
class Scene;

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

 protected:
  ReferencePointer<Logger> Console;
  ReferencePointer<Window> AppWindow;
  ScopePointer<Scene> CurrentScene;
  ReferencePointer<RenderDevice> MainRenderDevice;
  struct _ApplicationInfo {
    String Name;
    Vector3 Version;
    Vector2 WindowSize;
  } ApplicationInfo;

 private:
  bool m_Initialized = false;
};

extern ReferencePointer<Application> CreateApplication();
}  // namespace Hydrogen
