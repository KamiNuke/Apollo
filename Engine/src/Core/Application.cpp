#include "Application.h"

#include "Logger/Log.h"
#include "glad/glad.h"
#include "defines.h"
#include "Input.h"

namespace Apollo
{

    Application* Application::s_instance = nullptr;

    Application::Application(const Window::Properties& props)
    {
        assert(!s_instance && "Application already exists");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallback(BIND_EVENT_FN(onEvent));

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while (m_isRunning)
        {
            glClearColor(0,0,0,1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_layerStack)
                layer->onUpdate();

            m_imGuiLayer->begin();
            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();
            m_imGuiLayer->end();

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
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* overlay)
    {
        m_layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_isRunning = false;
        return true;
    }
} // ApolloEvent& event