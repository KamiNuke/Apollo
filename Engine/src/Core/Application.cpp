#include "Application.h"

#include <memory>
#include <X11/Xproto.h>

#include "Logger/Log.h"
#include "Base.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Utils/PlatformUtils.h"

namespace Apollo
{
    Application* Application::s_instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_specification(specification)
    {
        APOLLO_ASSERT(!s_instance, "Application already exists");
        s_instance = this;

        m_window = Window::Create(specification.name);
        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        //m_window->SetVsync(false);

        Renderer::Init();

        m_imGuiLayer = new ImGuiLayer();
        PushOverlay(m_imGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

    void Application::Run()
    {
        while (m_isRunning)
        {
            float time = Time::GetTime();
            Timestep ts = time - m_lastFrameTime;
            m_lastFrameTime = time;

            if (!m_isMinimized)
            {
                for (Layer* layer : m_layerStack)
                    layer->OnUpdate(ts);
            }

            m_imGuiLayer->Begin();
            for (Layer* layer : m_layerStack)
                layer->OnImGuiRender();
            m_imGuiLayer->End();

            m_window->OnUpdate();
        }
    }

    void Application::Close()
    {
        m_isRunning = false;
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
            m_isMinimized = true;
            return false;
        }
        m_isMinimized = false;

        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
} // ApolloEvent& event