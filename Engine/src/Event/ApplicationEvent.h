#pragma once
#include "Event.h"

namespace Apollo
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(int width, int height)
            : m_width(width), m_height(height) {}

        [[nodiscard]] int getWidth() const { return m_width; }
        [[nodiscard]] int getHeight() const { return m_height; }

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        int m_width, m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:

    };
} // Apollo
