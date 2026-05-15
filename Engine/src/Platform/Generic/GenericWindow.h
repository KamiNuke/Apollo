#pragma once
#include "glad/glad.h"

#include "Core/Window.h"
#include <SDL3/SDL.h>
#include <memory>

namespace Apollo::Platform
{
    /*
     * Uses SDL3 to create generic window
     */
    class GenericWindow : public Window
    {
    public:
        GenericWindow(const Properties& props);
        ~GenericWindow() override;

        void onUpdate() override;

        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;

        void setEventCallback(const EventCallbackFn& callback) override;
        void setVsync(bool enabled) override;
        bool isVsync() override;

        void* getNativeWindow() const override;

        void imGuiInit() override;
        void imGuiShutdown() override;
        void imGuiBegin() override;
        void imGuiEnd() override;

    private:
        void init(const Properties& props);
        void shutdown();

    private:
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_gl_context = nullptr;

        struct WindowData
        {
            std::string title;
            int width, height;
            bool vSync;

            EventCallbackFn eventCallback;
        };
        WindowData m_data;
    };
} // Apollo
