#pragma once
#include "Renderer/GraphicsContext.h"
#include "SDL3/SDL_video.h"

namespace Apollo
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        explicit OpenGLContext(SDL_Window* window);

        void Init() override;
        void Shutdown() override;
        void SwapBuffer() override;

        void ImGuiInit() override;
        void ImGuiShutdown() override;
        void ImGuiBegin() override;
        void ImGuiEnd() override;

    private:
        SDL_Window* m_window;
        SDL_GLContext m_gl_context;
    };
} // Apollo
