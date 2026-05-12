
#include "genericwindow.h"

#include "logger/log.h"

namespace Apollo
{
    
    Window* Window::create(const Properties& props)
    {
        return new Platform::GenericWindow(props);
    }

    Platform::GenericWindow::GenericWindow(const Properties& props)
        : m_props(props)
    {
        init();
    }

    Platform::GenericWindow::~GenericWindow()
    {
        shutdown();
    }

    void Platform::GenericWindow::update()
    {
        SDL_GL_SwapWindow(m_window);

        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_EVENT_QUIT )
            {
                abort();
            }
        }

    }

    int Platform::GenericWindow::getWidth() const
    {
        int w;
        SDL_GetWindowSize(m_window, &w, nullptr);
        APOLLO_LOGGER_INFO("Window width: {0}", w);
        return w;
    }
    
    int Platform::GenericWindow::getHeight() const
    {
        int h;
        SDL_GetWindowSize(m_window, nullptr, &h);
        APOLLO_LOGGER_INFO("Window height: {0}", h);
        return h;
    }

    void Platform::GenericWindow::init()
    {
        APOLLO_LOGGER_INFO("Initializing window");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            APOLLO_LOGGER_CRITICAL("Failed to initialize SDL: {0}", SDL_GetError());
            assert("SDL_Init() Failed");
        }

        constexpr SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
        m_window = SDL_CreateWindow(m_props.title.c_str(), m_props.width, m_props.height, flags);
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
        SDL_GL_SetSwapInterval(0);
        SDL_ShowWindow(m_window);
    }

    void Platform::GenericWindow::shutdown()
    {
        SDL_GL_DestroyContext(m_gl_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
} // Apollo