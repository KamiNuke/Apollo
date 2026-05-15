#pragma once
#include "Event.h"

namespace Apollo
{
    class KeyEvent : public Event
    {
    public:
        inline unsigned int getKeyCode() const { return m_keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(unsigned int keyCode)
            : m_keyCode(keyCode) {}

        unsigned int m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(unsigned int keyCode, bool repeat)
            : KeyEvent(keyCode),
              m_repeat(repeat) {}

        [[nodiscard]] inline bool getRepeat() const { return m_repeat; }

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keyCode << " repeat:" << m_repeat;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        bool m_repeat;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(unsigned int keyCode)
            : KeyEvent(keyCode) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

} // Apollo
