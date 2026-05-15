#pragma once
#include "Event.h"

namespace Apollo
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y)
            : m_mouseX(x), m_mouseY(y) {}

        [[nodiscard]] float GetX() const { return m_mouseX; }
        [[nodiscard]] float GetY() const { return m_mouseY; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_mouseX, m_mouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float offsetX, const float offsetY)
            : m_offsetX(offsetX), m_offsetY(offsetY) {}

        [[nodiscard]] float GetOffsetX() const { return m_offsetX; }
        [[nodiscard]] float GetOffsetY() const { return m_offsetY; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetOffsetX() << ", " << GetOffsetY();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_offsetX, m_offsetY;
    };

    class MouseButtonEvent : public Event
    {
    public:
        [[nodiscard]] int GetMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int button)
            : m_button(button) {}

        int m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

} // Apollo
