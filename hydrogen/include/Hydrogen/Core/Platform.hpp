#pragma once

#include "../Core/Memory.hpp"

// Platform detection
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define HY_PLATFORM_WINDOWS
#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
#define HY_PLATFORM_WINDOWS_STORE
#endif
#if defined(_XBOX_ONE) || defined(_DURANGO)
#define HY_PLATFORM_XBOX_ONE
#endif
#elif defined(__unix__) || defined(__APPLE__)
#define HY_PLATFORM_UNIX
#if defined(__APPLE__)
#define HY_PLATFORM_APPLE
#include <TargetConditionals.h>
#if defined(TARGET_OS_EMBEDDED)
#define HY_PLATFORM_APPLE_EMBEDDED
#endif
#if defined(TARGET_IPHONE_SIMULATOR)
#define HY_PLATFORM_IOS_SIMULATOR
#endif
#if defined(TARGET_OS_IPHONE)
#define HY_PLATFORM_IOS
#endif
#if defined(TARGET_OS_MAC)
#define HY_PLATFORM_MACOS
#endif
#endif
#elif defined(__native_client__)
#define HY_PLATFORM_NACL
#endif

#if defined(__EMSCRIPTEN__)
#define HY_PLATFORM_WEB
#endif

#if defined(__ANDROID__)
#define HY_PLATFORM_ANDROID
#endif

#if defined(__linux__)
#define HY_PLATFORM_LINUX
#endif

#if defined(__DragonFly__)
#define HY_PLATFORM_DRAGON_FLY_BSD
#endif

#if defined(__FreeBSD__)
#define HY_PLATFORM_FREE_BSD
#endif

#if defined(__NetBSD__)
#define HY_PLATFORM_NET_BSD
#endif

#if defined(__OpenBSD__)
#define HY_PLATFORM_OPEN_BSD
#endif

#if defined(__ros__)
#define HY_PLATFORM_AKAROS
#endif

#if defined(__sun)
#define HY_PLATFORM_SOLARIS
#endif

#if defined(__psp2__) || defined(__PS4__)
#define HY_PLATFORM_PLAYSTATION
#endif

#if defined(__NX__)
#define HY_PLATFORM_NINTENDO
#endif

// Compiler detection
#if defined(_MSC_VER)
#define HY_COMPILER_MSVC
#elif defined(__clang__)
#define HY_COMPILER_CLANG
#elif defined(__GNUC__)
#define HY_COMPILER_GCC
#elif defined(__INTEL_COMPILER)
#define HY_COMPILER_INTEL
#elif defined(__CC_ARM) || defined(__ARMCC__)
#define HY_COMPILER_ARM
#elif defined(__NVCC__)
#define HY_COMPILER_NVCC
#endif

// Architecture detection
#if defined(__i386__)
#define HY_ARCH_X86
#elif defined(__x86_64__)
#define HY_ARCH_X86_64
#elif defined(__arm__) || defined(__thumb__) || defined(__aarch64__)
#define HY_ARCH_ARM
#endif

#if defined(__arm64__) || defined(__aarch64__)
#define HY_ARCH_ARM64
#endif

#if defined(__mips__) || defined(__mips) || defined(__MIPS__)
#define HY_ARCH_MIPS
#endif

#if defined(__riscv) || defined(__riscv__)
#define HY_ARCH_RISCV
#endif

#if defined(__sparc__) || defined(__sparc)
#define HY_ARCH_SPARC
#endif

#if defined(__wasm__)
#define HY_ARCH_WASM
#endif

namespace Hydrogen {
enum class Platform { Windows, WindowsPC, XBoxOne, Unix, Apple, MacOS, IOS, Web, Android, Linux, DragonFlyBSD, FreeBSD, NetBSD, OpenBSD, AkarOS, Solaris, Playstation, Nintendo };

String PlatformToString(Platform platform);
Platform GetCurrentPlatform();
inline String GetCurrentPlatformName();
}  // namespace Hydrogen
