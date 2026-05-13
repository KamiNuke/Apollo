#pragma once
#include <string>

#include "event/event.h"

namespace Apollo
{
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        struct Properties
        {
            std::string title;
            int width;
            int height;

            Properties(const std::string& _title = "Apollo",
                        const int _width = 1280,
                        const int _height = 720)
                : title(_title), width(_width), height(_height)
            {
            }
        };

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVsync(bool enabled) = 0;
        virtual bool isVsync() = 0;

        static Window* create(const Properties& props = Properties());
    private:

    };
}
