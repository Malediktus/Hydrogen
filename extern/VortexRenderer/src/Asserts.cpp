#include <Vortex/Core.hpp>
#include <spdlog/spdlog.h>
#include <cstdlib>

void Vortex::ReportAssertionFailure(const char* filename, uint64_t line, bool check, const std::string& msg) {
#ifdef VT_DEBUG
    if (check)
        spdlog::critical("Assertion failure of type sanity check in file {}, line {}: {}\n", filename, line, msg);
    else
        spdlog::critical("Assertion failure of type error in file {}, line {}: {}\n", filename, line, msg);
#else
    // No need to check for sanity checks, because they dont exist in release builds
    spdlog::critical("Vortex Renderer failure: {}", msg);
#endif
    exit(1);
}
