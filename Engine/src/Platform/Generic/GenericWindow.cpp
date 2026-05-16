#include "GenericWindow.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Logger/Log.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Apollo
{
    static bool s_SDLInitialized = false;

    Window* Window::Create(const Properties& props)
    {
        return new Platform::GenericWindow(props);
    }

    Platform::GenericWindow::GenericWindow(const Properties& props)
    {
        Init(props);
    }

    Platform::GenericWindow::~GenericWindow()
    {
        Shutdown();
    }

    void Platform::GenericWindow::OnUpdate()
    {
        m_context->SwapBuffer();

        SDL_Event event;
        while( SDL_PollEvent( &event ) != 0 )
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            switch (event.type)
            {
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    m_data.width = event.window.data1;
                    m_data.height = event.window.data2;

                    m_context->SetViewportSize(0, 0, event.window.data1, event.window.data2);

                    WindowResizeEvent resizeEvent(event.window.data1, event.window.data2);
                    m_data.eventCallback(resizeEvent);
                    break;
                }
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    WindowCloseEvent closeEvent;
                    m_data.eventCallback(closeEvent);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    KeyPressedEvent keyPressed(event.key.key, event.key.repeat);
                    m_data.eventCallback(keyPressed);
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    KeyReleasedEvent keyReleased(event.key.key);
                    m_data.eventCallback(keyReleased);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    MouseButtonPressedEvent mousePressed(event.button.button);
                    m_data.eventCallback(mousePressed);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    MouseButtonPressedEvent mouseReleased(event.button.button);
                    m_data.eventCallback(mouseReleased);
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION:
                {
                    MouseMovedEvent mouseMoved(event.motion.x, event.motion.y);
                    m_data.eventCallback(mouseMoved);
                    break;
                }
                case SDL_EVENT_MOUSE_WHEEL:
                {
                    MouseScrolledEvent mouseScrolled(event.wheel.x, event.wheel.y);
                    m_data.eventCallback(mouseScrolled);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    int Platform::GenericWindow::GetWidth() const
    {
        int w;
        SDL_GetWindowSize(m_window, &w, nullptr);
        return w;
    }
    
    int Platform::GenericWindow::GetHeight() const
    {
        int h;
        SDL_GetWindowSize(m_window, nullptr, &h);
        return h;
    }

    void Platform::GenericWindow::SetEventCallback(const EventCallbackFn& callback)
    {
        m_data.eventCallback = callback;
    }

    void Platform::GenericWindow::SetVsync(bool enabled)
    {
        if (enabled)
            SDL_GL_SetSwapInterval(1);
        else
            SDL_GL_SetSwapInterval(0);

        m_data.vSync = enabled;
    }

    bool Platform::GenericWindow::IsVsync()
    {
        return m_data.vSync;
    }

    void* Platform::GenericWindow::GetNativeWindow() const
    {
        return m_window;
    }

    void Platform::GenericWindow::ImGuiInit()
    {
        m_context->ImGuiInit();
    }

    void Platform::GenericWindow::ImGuiShutdown()
    {
        m_context->ImGuiShutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void Platform::GenericWindow::ImGuiBegin()
    {
        m_context->ImGuiBegin();
        ImGui_ImplSDL3_NewFrame();
    }

    void Platform::GenericWindow::ImGuiEnd()
    {
        m_context->ImGuiEnd();
    }

    void Platform::GenericWindow::Init(const Properties& props)
    {
        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        APOLLO_LOGGER_INFO("Initializing window {0} ({1}, {2})", props.title, props.width, props.height);

        if (!s_SDLInitialized)
        {
            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                APOLLO_LOGGER_CRITICAL("Failed to initialize SDL: {0}", SDL_GetError());
                assert("SDL_Init() Failed");
            }

            s_SDLInitialized = true;
        }

        constexpr SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
        m_window = SDL_CreateWindow(m_data.title.c_str(), m_data.width, m_data.height, flags);
        if (!m_window)
        {
            APOLLO_LOGGER_CRITICAL("Failed to Create window: {0}", SDL_GetError());
            assert("SDL_CreateWindow() Failed");
        }

        m_context = std::make_unique<OpenGLContext>(m_window);
        m_context->Init();

        SDL_ShowWindow(m_window);
        SetVsync(true);
    }

    void Platform::GenericWindow::Shutdown()
    {
        m_context->Shutdown();
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
} // Apollo