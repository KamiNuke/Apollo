#pragma once
#include <string>

#include "Event/Event.h"

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

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual int GetWidth() const = 0;
        [[nodiscard]] virtual int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVsync(bool enabled) = 0;
        virtual bool IsVsync() = 0;

        virtual void* GetNativeWindow() const = 0;

        // ImGui stuff
        virtual void ImGuiInit() = 0;
        virtual void ImGuiShutdown() = 0;
        virtual void ImGuiBegin() = 0;
        virtual void ImGuiEnd() = 0;

        static Window* Create(const Properties& props = Properties());
    private:

    };
}
