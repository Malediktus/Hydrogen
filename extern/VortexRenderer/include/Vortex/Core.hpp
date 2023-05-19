#pragma once

#include <string>

#ifdef VT_BUILD_SHARED
    #ifdef VT_EXPORT_API
    // Exports
    #ifdef _MSC_VER
        #define VT_API __declspec(dllexport)
    #else
        #define VT_API __attribute__((visibility("default")))
    #endif
#else
    // Imports
    #ifdef _MSC_VER
        #define VT_API __declspec(dllimport)
    #else
        #define VT_API
    #endif
#endif
#else
    #define VT_API
#endif

#ifdef VT_DEBUG
    #define VT_ASSERT_CHECK(expr, msg)                                                                                                                                             \
        if (expr)                                                                                                                                                                  \
            ;                                                                                                                                                                      \
        else                                                                                                                                                                       \
            Vortex::ReportAssertionFailure(__FILE__, __LINE__, true, msg)
    #define VT_ASSERT(expr, msg)                                                                                                                                                   \
        if (expr)                                                                                                                                                                  \
            ;                                                                                                                                                                      \
        else                                                                                                                                                                       \
            Vortex::ReportAssertionFailure(__FILE__, __LINE__, false, msg)
#else
    #define VT_ASSERT_CHECK(expr, msg)
    #define VT_ASSERT(expr, msg)                                                                                                                                                   \
        if (expr)                                                                                                                                                                  \
            ;                                                                                                                                                                      \
        else                                                                                                                                                                       \
            Vortex::ReportAssertionFailure(__FILE__, __LINE__, false, msg)
#endif

namespace Vortex {
#ifdef _MSC_VER
    void ReportAssertionFailure(const char* filename, uint64_t line, bool check, const std::string& msg);
#else
    __attribute__((noreturn)) void ReportAssertionFailure(const char* filename, uint64_t line, bool check, const std::string& msg);
#endif
}
