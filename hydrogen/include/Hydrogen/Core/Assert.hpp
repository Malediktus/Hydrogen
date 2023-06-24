#pragma once

#include "Platform.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

#ifdef HY_DEBUG
    #define HY_ASSERT_CHECK(expr, ...)                                                                                                                                             \
        if (expr) {                                                                                                                                                                \
        } else {                                                                                                                                                                   \
            HY_LOG_FATAL("Assertion error in " __FILE__ ": '" #expr "' is not zero | " __VA_ARGS__);                                                                               \
            exit(0);                                                                                                                                                               \
        }
#else
    #define HY_ASSERT_CHECK(expr, ...)
#endif

#define HY_ASSERT(expr, ...)                                                                                                                                                       \
    if (expr) {                                                                                                                                                                    \
    } else {                                                                                                                                                                       \
        HY_LOG_FATAL("Hydrogen error: " __VA_ARGS__);                                                                                                                              \
        exit(0);                                                                                                                                                                   \
    }

#define HY_INVOKE_ERROR(...)                                                                                                                                                       \
    HY_LOG_FATAL("Hydrogen error: " __VA_ARGS__);                                                                                                                                  \
    exit(0);
