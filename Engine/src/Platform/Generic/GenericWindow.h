#pragma once
#include "glad/glad.h"

#include "Core/Window.h"
#include <SDL3/SDL.h>
#include <memory>

#include "Renderer/GraphicsContext.h"

namespace Apollo::Platform
{
    /*
     * Uses SDL3 to Create generic window
     */
    class GenericWindow : public Window
    {
    public:
        GenericWindow(const Properties& props);
        ~GenericWindow() override;

        void OnUpdate() override;

        [[nodiscard]] int GetWidth() const override;
        [[nodiscard]] int GetHeight() const override;

        void SetEventCallback(const EventCallbackFn& callback) override;
        void SetVsync(bool enabled) override;
        bool IsVsync() override;

        void* GetNativeWindow() const override;

        void ImGuiInit() override;
        void ImGuiShutdown() override;
        void ImGuiBegin() override;
        void ImGuiEnd() override;

    private:
        void Init(const Properties& props);
        void Shutdown();

    private:
        SDL_Window* m_window = nullptr;
        std::unique_ptr<GraphicsContext> m_context;

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
