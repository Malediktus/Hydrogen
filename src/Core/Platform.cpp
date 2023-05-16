#include <Hydrogen/Core/Platform.hpp>
#include <Hydrogen/Core/Logger.hpp>

using namespace Hydrogen;

std::string Hydrogen::PlatformToString(Platform platform) {
    switch (platform) {
    case Platform::Windows:
        return "Windows";
        break;
    case Platform::WindowsPC:
        return "WindowsPC";
        break;
    case Platform::XBoxOne:
        return "XBoxOne";
        break;
    case Platform::Unix:
        return "Unix";
        break;
    case Platform::Apple:
        return "Apple";
        break;
    case Platform::MacOS:
        return "MacOS";
        break;
    case Platform::IOS:
        return "IOS";
        break;
    case Platform::Web:
        return "Web";
        break;
    case Platform::Android:
        return "Android";
        break;
    case Platform::Linux:
        return "Linux";
        break;
    case Platform::DragonFlyBSD:
        return "DragonFlyBSD";
        break;
    case Platform::FreeBSD:
        return "FreeBSD";
        break;
    case Platform::NetBSD:
        return "NetBSD";
        break;
    case Platform::OpenBSD:
        return "OpenBSD";
        break;
    case Platform::AkarOS:
        return "AkarOS";
        break;
    case Platform::Solaris:
        return "Solaris";
        break;
    case Platform::Playstation:
        return "Playstation";
        break;
    case Platform::Nintendo:
        return "Nintendo";
        break;
    default:
        HY_INVOKE_ERROR("Invalid platform");
    }
}

Platform Hydrogen::GetCurrentPlatform() {
#if defined HY_PLATFORM_WINDOWS_STORE
    return Platform::WindowsPC;
#elif defined HY_PLATFORM_XBOX_ONE
    return Platform::XBoxOne;
#elif defined HY_PLATFORM_MACOS
    return Platform::MacOS;
#elif defined HY_PLATFORM_IOS
    return Platform::IOS;
#elif defined HY_PLATFORM_WEB
    return Platform::Web;
#elif defined HY_PLATFORM_ANDROID
    return Platform::Android;
#elif defined HY_PLATFORM_LINUX
    return Platform::Linux;
#elif defined HY_PLATFORM_DRAGON_FLY_BSD
    return Platform::DragonFlyBSD;
#elif defined HY_PLATFORM_FREE_BSD
    return Platform::FreeBSD;
#elif defined HY_PLATFORM_NET_BSD
    return Platform::NetBSD;
#elif defined HY_PLATFORM_OPEN_BSD
    return Platform::OpenBSD;
#elif defined HY_PLATFORM_AKAROS
    return Platform::AkarOS;
#elif defined HY_PLATFORM_SOLARIS
    return Platform::Solaris;
#elif defined HY_PLATFORM_PLAYSTATION
    return Platform::Playstation;
#elif defined HY_PLATFORM_NINTENDO
    return Platform::Nintendo;
#endif
    HY_INVOKE_ERROR("Invalid platform");
}

inline std::string Hydrogen::GetCurrentPlatformName() {
    return PlatformToString(GetCurrentPlatform());
}
