#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Core/Platform.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <Hydrogen/Platform/MacOS/MacOSWindow.hpp>

namespace Hydrogen {
ReferencePointer<Window> Window::Create(const String& title, uint32_t width, uint32_t height) {
#ifdef HY_PLATFORM_MACOS
    return NewReferencePointer<MacOSWindow>(title, width, height);
#endif
    HY_INVOKE_ERROR("Invalid platform");
    return nullptr;
}
} // namespace Hydrogen
