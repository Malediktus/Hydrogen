#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Core/Logger.hpp>

using namespace Hydrogen;

void Hydrogen::ReportAssertionFailure(const char* filename, size_t line, bool check, const std::string& msg) {
#ifdef HY_DEBUG
    if (check) {
        HY_LOG_FATAL("Assertion failure of type sanity check in file {}, line {}: {}\n", filename, line, msg);
    } else {
        HY_LOG_FATAL("Assertion failure of type error in file {}, line {}: {}\n", filename, line, msg);
    }
#else
    // No need to check for sanity checks, because they dont exist in release builds
    HY_LOG_FATAL("Vortex Renderer failure: {}", msg);
#endif
    exit(1);
}
