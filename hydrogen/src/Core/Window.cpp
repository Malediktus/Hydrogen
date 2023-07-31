#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Core/Platform.hpp>
#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Platform/MacOS/MacOSWindow.hpp>
#include <Hydrogen/Platform/Windows/WindowsWindow.hpp>

namespace Hydrogen {
ReferencePointer<Window> Window::Create(const String& title, uint32_t width, uint32_t height) {
#if defined HY_PLATFORM_MACOS
  return NewReferencePointer<MacOSWindow>(title, width, height);
#elif defined HY_PLATFORM_WINDOWS
  return NewReferencePointer<WindowsWindow>(title, width, height);
#else
  HY_INVOKE_ERROR("Invalid platform");
  return nullptr;
#endif
}
}  // namespace Hydrogen
