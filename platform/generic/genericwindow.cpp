#include "genericwindow.h"

#include "event/applicationevent.h"
#include "event/keyevent.h"
#include "event/mouseevent.h"
#include "logger/log.h"

namespace Apollo
{
    static bool s_SDLInitialized = false;

    Window* Window::create(const Properties& props)
    {
        return new Platform::GenericWindow(props);
    }

    Platform::GenericWindow::GenericWindow(const Properties& props)
    {
        init(props);
    }

    Platform::GenericWindow::~GenericWindow()
    {
        shutdown();
    }

    void Platform::GenericWindow::onUpdate()
    {
        SDL_GL_SwapWindow(m_window);

        SDL_Event event;
        while( SDL_PollEvent( &event ) != 0 )
        {
            switch (event.type)
            {
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    m_data.width = event.window.data1;
                    m_data.height = event.window.data2;

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

    int Platform::GenericWindow::getWidth() const
    {
        int w;
        SDL_GetWindowSize(m_window, &w, nullptr);
        return w;
    }
    
    int Platform::GenericWindow::getHeight() const
    {
        int h;
        SDL_GetWindowSize(m_window, nullptr, &h);
        return h;
    }

    void Platform::GenericWindow::setEventCallback(const EventCallbackFn& callback)
    {
        m_data.eventCallback = callback;
    }

    void Platform::GenericWindow::setVsync(bool enabled)
    {
        if (enabled)
            SDL_GL_SetSwapInterval(1);
        else
            SDL_GL_SetSwapInterval(0);

        m_data.vSync = enabled;
    }

    bool Platform::GenericWindow::isVsync()
    {
        return m_data.vSync;
    }

    void* Platform::GenericWindow::getNativeWindow() const
    {
        return m_window;
    }

    void Platform::GenericWindow::init(const Properties& props)
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

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        constexpr SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
        m_window = SDL_CreateWindow(m_data.title.c_str(), m_data.width, m_data.height, flags);
        if (!m_window)
        {
            APOLLO_LOGGER_CRITICAL("Failed to create window: {0}", SDL_GetError());
            assert("SDL_CreateWindow() Failed");
        }

        m_gl_context = SDL_GL_CreateContext(m_window);
        if (!m_gl_context)
        {
            APOLLO_LOGGER_CRITICAL("Failed to create context: {0}", SDL_GetError());
            assert("SDL_GL_CreateContext() failed");
        }

        SDL_GL_MakeCurrent(m_window, m_gl_context);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        {
            assert("Failed to initialize Glad");
        }

        SDL_ShowWindow(m_window);
        setVsync(true);
    }

    void Platform::GenericWindow::shutdown()
    {
        SDL_GL_DestroyContext(m_gl_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
} // Apollo