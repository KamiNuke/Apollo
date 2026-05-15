#pragma once
#include <memory>

#include "LayerStack.h"
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

        void run();

        void onEvent(Event& e);
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        [[nodiscard]] inline Window& getWindow() const { return *m_window; }

        /*
         * Basically entry function for user apps. Define in order to use
         */
        Application* createApplication();

        static inline Application& get() { return *s_instance; };
    private:
        bool onWindowClose(WindowCloseEvent& e);

    private:
        std::unique_ptr<Window> m_window;
        static Application* s_instance;

        ImGuiLayer* m_imGuiLayer;
        LayerStack m_layerStack;

        bool m_isRunning = true;
    };
} // Apollo
