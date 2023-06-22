#pragma once

#include "Memory.hpp"
#include "Logger.hpp"
#include <random>

namespace Hydrogen {
class UUID {
public:
    UUID() {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        // Generate a random 64-bit integer
        m_Value = dis(gen);

        // Set version (4) and variant (10xxxxxx)
        m_Value &= ~(0xf000'0000'0000'0000ULL); // Clear version bits
        m_Value |= 0x4000'0000'0000'0000ULL;    // Set version 4
        m_Value &= ~(0xc000'0000'0000'0000ULL); // Clear variant bits
        m_Value |= 0x8000'0000'0000'0000ULL;    // Set variant 10

        HY_LOG_DEBUG("Created new UUID: {}!", m_Value);
    }

    UUID(const uint64_t val) {
        m_Value = val;
    }

    operator uint64_t() const {
        return m_Value;
    }

    uint64_t GetValue() {
        return m_Value;
    }

private:
    uint64_t m_Value;
};
} // namespace Hydrogen
