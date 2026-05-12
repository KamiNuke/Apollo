#pragma once

#include <apollo.h>

namespace Apollo
{
    class SandboxMain : public Application
    {
    public:
        SandboxMain(Window::Properties props);

        ~SandboxMain();

        void onUpdate() override;
        void onRender() override;

    private:

    };

    Application* createApplication();
} // Apollo
