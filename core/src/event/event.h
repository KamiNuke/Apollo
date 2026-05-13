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

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
                                virtual EventType getEventType() const override { return getStaticType(); }\
                                virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

    class Event
    {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual EventType getEventType() const = 0;
        [[nodiscard]] virtual const char* getName() const = 0;
        [[nodiscard]] virtual int getCategoryFlags() const = 0;
        [[nodiscard]] virtual std::string toString() const { return getName(); }

        [[nodiscard]] inline bool isInCategory(const EventCategory category) const
        {
            return getCategoryFlags() & category;
        }
    protected:
        bool m_handled = false;
    };

    class EventDispatcher
    {
        template <typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event)
            : m_event(event) {}

        // template<typename T, typename F>
        // bool dispatch(const F& func)
        // {
        //     if (m_event.getEventType() == T::getStaticType())
        //     {
        //         m_event.m_handled |= func(static_cast<T&>(&m_event));
        //         return true;
        //     }
        //     return false;
        // }

        template<typename T>
        bool dispatch(EventFn<T> func)
        {
            if (m_event.getEventType() == T::getStaticType())
            {
                m_event.m_handled = func(*(T*)&m_event);
                return true;
            }
            return false;
        }
    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.toString();
    }

    inline std::string format_as(const Event& e) {
        return e.toString();
    }
} // Apollo