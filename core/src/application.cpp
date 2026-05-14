#include "application.h"

#include "glad/glad.h"
#include "logger/log.h"
#include "SDL3/SDL_video.h"

namespace Apollo
{
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

    Application* Application::s_instance = nullptr;

    Application::Application(const Window::Properties& props)
    {
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallback(BIND_EVENT_FN(onEvent));

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            APOLLO_LOGGER_CRITICAL("Failed to initialize GLAD");
            abort();
        }
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while (m_isRunning)
        {
            glClearColor(1, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_layerStack)
                layer->onUpdate();

            m_window->onUpdate();
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

        //APOLLO_LOGGER_TRACE("{0}", e);

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
        {
            if (e.handled)
                break;
            (*it)->onEvent(e);
        }
    }

    void Application::pushLayer(Layer* layer)
    {
        m_layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer* overlay)
    {
        m_layerStack.pushOverlay(overlay);
    }

    Application& Application::get()
    {
        return *s_instance;
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_isRunning = false;
        return true;
    }
} // ApolloEvent& event