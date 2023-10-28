#pragma once

#include "Platform.hpp"

#if defined HY_PLATFORM_WINDOWS
#include "../Platform/Windows/WindowsEntry.hpp"
#elif defined HY_PLATFORM_MACOS
#include "../Platform/MacOS/MacOSEntry.hpp"
#endif
