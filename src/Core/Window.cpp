#include <Hydrogen/Core/Window.hpp>
#include <Hydrogen/Core/Platform.hpp>
#include <Hydrogen/Platform/MacOS/MacOSWindow.hpp>

namespace Hydrogen {
Reference<Window> Window::Create(const std::string& title, uint32_t width, uint32_t height) {
#ifdef HY_PLATFORM_MACOS
    return NewReference<MacOSWindow>(title, width, height);
#endif
    // TODO: Assert
    return nullptr;
}
} // namespace Hydrogen
