#include "Application.h"

#include <memory>

#include "Logger/Log.h"
#include "defines.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "SDL3/SDL_timer.h"

namespace Apollo
{
    Application* Application::s_instance = nullptr;

    Application::Application(const Window::Properties& props)
    {
        assert(!s_instance && "Application already exists");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        m_window->SetVsync(true);

        m_imGuiLayer = new ImGuiLayer();
        PushOverlay(m_imGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_isRunning)
        {
            float time = static_cast<float>(SDL_GetTicks()) / 1000;
            Timestep timestep = time - m_lastFrameTime;
            m_lastFrameTime = time;


            for (Layer* layer : m_layerStack)
                layer->OnUpdate(timestep);

            m_imGuiLayer->Begin();
            for (Layer* layer : m_layerStack)
                layer->OnImGuiRender();
            m_imGuiLayer->End();

            m_window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        //APOLLO_LOGGER_TRACE("{0}", e);

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
        {
            if (e.handled)
                break;
            (*it)->OnEvent(e);
        }

    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_isRunning = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            return false;
        }

        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
} // ApolloEvent& event