#pragma once

#include <memory>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <string>

namespace Hydrogen {
template <typename T> using ReferencePointer = std::shared_ptr<T>;
template <typename T> using ReferencePointer = std::shared_ptr<T>;

template <typename T, typename... Args> constexpr ReferencePointer<T> NewReferencePointer(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T> using ScopePointer = std::unique_ptr<T>;
template <typename T> using ScopePointer = std::unique_ptr<T>;

template <typename T, typename... Args> constexpr ScopePointer<T> ScopeNewPointer(Args&&... args) {
    return std::unique_ptr<T>(std::forward<Args>(args)...);
}

template <typename T, std::size_t N> using StaticArray = std::array<T, N>;
template <typename T> using DynamicArray = std::vector<T>;
template <typename T, typename D> using Map = std::map<T, D>;
template <typename T, typename D> using UnorderedMap = std::unordered_map<T, D>;

using String = std::string;
} // namespace Hydrogen
