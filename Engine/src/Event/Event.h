#pragma once
#include <sstream>
#include <functional>
#include <string>

namespace Apollo
{
    // TODO: Implement type-safety

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0x00,
        EventCategoryApplication = 0x01,
        EventCategoryInput = 0x02,
        EventCategoryMouse = 0x03,
        EventCategoryKeyboard = 0x04,
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual EventType GetEventType() const = 0;
        [[nodiscard]] virtual const char* GetName() const = 0;
        [[nodiscard]] virtual int GetCategoryFlags() const = 0;
        [[nodiscard]] virtual std::string ToString() const { return GetName(); }

        [[nodiscard]] inline bool isInCategory(const EventCategory category) const
        {
            return GetCategoryFlags() & category;
        }

        bool handled = false;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_event(event) {}

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_event.GetEventType() == T::GetStaticType())
            {
                m_event.handled |= func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }
    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

    inline std::string format_as(const Event& e) {
        return e.ToString();
    }
} // Apollo