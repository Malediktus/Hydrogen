#pragma once

#include <memory>
#include <Vortex/Vortex.hpp>

namespace Hydrogen {
template <typename T> using Reference = std::shared_ptr<T>;
template <typename T> using Reference = std::shared_ptr<T>;

template <typename T, typename... Args> constexpr Reference<T> NewReference(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace Hydrogen
