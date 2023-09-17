#pragma once

#include <map>

#include "../Core/Logger.hpp"
#include "../Core/Memory.hpp"
#include "KeyCodes.hpp"

namespace Hydrogen {
template <class... Args>
class HydrogenEvent {
 public:
  HydrogenEvent() = default;
  ~HydrogenEvent() = default;

  void RegisterCallback(const std::function<void(Args...)>& fn) { m_Callbacks.push_back(fn); }

  void Invoke(Args... args) const {
    for (const auto& callback : m_Callbacks) {
      callback(args...);
    }
  }

 private:
  DynamicArray<std::function<void(Args...)>> m_Callbacks;
};
}  // namespace Hydrogen
