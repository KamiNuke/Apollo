#include "OpenGLContext.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "glad/glad.h"
#include "Logger/Log.h"

namespace Apollo
{
    OpenGLContext::OpenGLContext(SDL_Window* window)
        : m_window(window), m_gl_context(nullptr)
    {
    }

    void OpenGLContext::Init()
    {
        m_gl_context = SDL_GL_CreateContext(m_window);
        if (!m_gl_context)
        {
            APOLLO_LOGGER_CRITICAL("Failed to create context: {0}", SDL_GetError());
            assert("SDL_GL_CreateContext() failed");
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_GL_MakeCurrent(m_window, m_gl_context);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        {
            assert("Failed to initialize Glad");
        }

        APOLLO_LOGGER_INFO("OpenGL info:");
        APOLLO_LOGGER_INFO("    Vendor: {0}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
        APOLLO_LOGGER_INFO("    Renderer: {0}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        APOLLO_LOGGER_INFO("    Version: {0}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    }

    void OpenGLContext::Shutdown()
    {
        SDL_GL_DestroyContext(m_gl_context);
    }

    void OpenGLContext::SwapBuffer()
    {
        SDL_GL_SwapWindow(m_window);
    }

    void OpenGLContext::SetViewportSize(int x, int y, int w, int h)
    {
        glViewport(x, y, w, h);
    }

    void OpenGLContext::ImGuiInit()
    {
        ImGui_ImplSDL3_InitForOpenGL(m_window, m_gl_context);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void OpenGLContext::ImGuiShutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    void OpenGLContext::ImGuiBegin()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void OpenGLContext::ImGuiEnd()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
} // Apollo