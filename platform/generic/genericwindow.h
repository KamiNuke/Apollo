#pragma once
#include "window.h"
#include <SDL3/SDL.h>

namespace Apollo::Platform
{
    class GenericWindow : public Window
    {
    public:
        GenericWindow(const Properties& props);

        ~GenericWindow() override;

        void update() override;

        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;
    private:
        void init();
        void shutdown();

    private:
        SDL_Window* m_window;
        SDL_GLContext m_gl_context;
        Properties m_props;
    };
} // Apollo
