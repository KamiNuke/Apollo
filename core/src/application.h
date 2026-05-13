#pragma once
#include <memory>

#include "window.h"
#include "event/applicationevent.h"

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

        /*
         * Basically entry function for user apps. Define in order to use
         */
        Application* createApplication();

        static Application& get();
    private:
        bool onWindowClose(WindowCloseEvent& e);

    private:
        std::unique_ptr<Window> m_window;
        static Application* s_instance;

        bool m_isRunning = true;
    };
} // Apollo
