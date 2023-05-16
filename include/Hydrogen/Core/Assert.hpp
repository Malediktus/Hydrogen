#pragma once

#include "Platform.hpp"
#include <string>

#ifdef HY_DEBUG
    #define HY_ASSERT_CHECK(expr, msg)                                                                                                                                             \
        if (expr)                                                                                                                                                                  \
            ;                                                                                                                                                                      \
        else                                                                                                                                                                       \
            Hydrogen::ReportAssertionFailure(__FILE__, __LINE__, true, msg)
    #define HY_ASSERT(expr, msg)                                                                                                                                                   \
        if (expr)                                                                                                                                                                  \
            ;                                                                                                                                                                      \
        else                                                                                                                                                                       \
            Hydrogen::ReportAssertionFailure(__FILE__, __LINE__, false, msg)
#else
    #define HY_ASSERT_CHECK(expr, msg)
    #define HY_ASSERT(expr, msg)                                                                                                                                                   \
        if (expr)                                                                                                                                                                  \
            ;                                                                                                                                                                      \
        else                                                                                                                                                                       \
            Hydrogen::ReportAssertionFailure(__FILE__, __LINE__, false, msg)
#endif

#define HY_INVOKE_ERROR(msg) Hydrogen::ReportAssertionFailure(__FILE__, __LINE__, false, msg)

namespace Hydrogen {
#ifdef HY_COMPILER_MSVC
void ReportAssertionFailure(const char* filename, size_t line, bool check, const std::string& msg);
#else
__attribute__((noreturn)) void ReportAssertionFailure(const char* filename, size_t line, bool check, const std::string& msg);
#endif
} // namespace Hydrogen
