#include "application.h"

#include "glad/glad.h"
#include "logger/log.h"
#include "SDL3/SDL_video.h"
#include "event/applicationevent.h"

namespace Apollo
{
    Application* Application::s_instance = nullptr;

    Application::Application(const Window::Properties& props)
    {
        m_window = std::unique_ptr<Window>(Window::create());

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
        m_isRunning = false;

        while (m_isRunning)
        {
            m_window->update();

            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    Application& Application::get()
    {
        return *s_instance;
    }
} // Apollo