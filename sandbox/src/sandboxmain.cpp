#include "sandboxmain.h"

namespace Apollo
{
    SandboxMain::SandboxMain(Window::Properties props)
        : Application(props)
    {
    }

    SandboxMain::~SandboxMain()
    {
    }

    void SandboxMain::onUpdate()
    {
    }

    void SandboxMain::onRender()
    {
    }

    Application* createApplication()
    {
        Window::Properties props;
        props.title = "Apollo Editor";

        return new SandboxMain(props);
    }
} // Apollo