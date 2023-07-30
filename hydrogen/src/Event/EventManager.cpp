#include <Hydrogen/Event/EventManager.hpp>

using namespace Hydrogen;

EventMap EventDispatcher::m_EventMap;

void EventDispatcher::Post(const Event& event) {
  auto range = m_EventMap.equal_range(&typeid(event));
  for (auto it = range.first; it != range.second; it++) it->second(event);
}
