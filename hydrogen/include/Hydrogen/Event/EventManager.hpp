#pragma once

#include "../Core/Memory.hpp"
#include "../Core/Logger.hpp"
#include "KeyCodes.hpp"

#include <map>

namespace Hydrogen {
class Event {
public:
    virtual ~Event() {
    }
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(const int& width, const int& height) : m_Width(width), m_Height(height) {
    }

    const int& GetWidth() const {
        return m_Width;
    }
    const int& GetHeight() const {
        return m_Height;
    }

private:
    const int m_Width;
    const int m_Height;
};

class WindowCloseEvent : public Event {};

class KeyPressEvent : public Event {
public:
    KeyPressEvent(const KeyCode& keycode) : m_KeyCode(keycode) {
    }

    const KeyCode& GetKeyCode() const {
        return m_KeyCode;
    }

private:
    const KeyCode m_KeyCode;
};

class KeyReleaseEvent : public Event {
public:
    KeyReleaseEvent(const KeyCode& keycode) : m_KeyCode(keycode) {
    }

    const KeyCode& GetKeyCode() const {
        return m_KeyCode;
    }

private:
    const KeyCode m_KeyCode;
};

class KeyRepeatEvent : public Event {
public:
    KeyRepeatEvent(const KeyCode& keycode) : m_KeyCode(keycode) {
    }

    const KeyCode& GetKeyCode() const {
        return m_KeyCode;
    }

private:
    const KeyCode m_KeyCode;
};

class MouseMoveEvent : public Event {
public:
    MouseMoveEvent(const int& X, const int& Y) : m_X(X), m_Y(Y) {
    }

    const int& GetX() const {
        return m_X;
    }
    const int& GetY() const {
        return m_Y;
    }

private:
    const int m_X;
    const int m_Y;
};

class MousePressEvent : public Event {
public:
    MousePressEvent(const KeyCode& keycode) : m_KeyCode(keycode) {
    }

    const KeyCode& GetKeyCode() const {
        return m_KeyCode;
    }

private:
    const KeyCode m_KeyCode;
};

class MouseReleaseEvent : public Event {
public:
    MouseReleaseEvent(const KeyCode& keycode) : m_KeyCode(keycode) {
    }

    const KeyCode& GetKeyCode() const {
        return m_KeyCode;
    }

private:
    const KeyCode m_KeyCode;
};

class MouseScrollEvent : public Event {
public:
    MouseScrollEvent(const int& value) : m_Value(value) {
    }

    const int& GetValue() const {
        return m_Value;
    }

private:
    const int m_Value;
};

using EventMap = std::multimap<const std::type_info*, const std::function<void(const Event&)>>;

class EventDispatcher {
public:
    template <typename T> static void Subscribe(const std::function<void(const Event&)>& fn) {
        m_EventMap.emplace(&typeid(T), fn);
    }
    static void Post(const Event& event);

private:
    static EventMap m_EventMap;
};
} // namespace Hydrogen
