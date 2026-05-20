#pragma once
#include <memory>

#include "Base.h"
#include "LayerStack.h"
#include "Timestep.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace Apollo
{
    class Application
    {
    public:
        explicit Application(const Window::Properties& props);

        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        [[nodiscard]] inline Window& GetWindow() const { return *m_window; }

        /*
         * Basically entry function for user apps. Define in order to use
         */
        Application* CreateApplication();

        static inline Application& Get() { return *s_instance; };
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        Scope<Window> m_window;
        static Application* s_instance;

        ImGuiLayer* m_imGuiLayer;
        LayerStack m_layerStack;

        bool m_isRunning = true;
        bool m_isMinimized = false;

        float m_lastFrameTime = 0.0f;
    };
} // Apollo
