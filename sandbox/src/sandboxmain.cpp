#include "sandboxmain.h"

#include "event/keyevent.h"

class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test") {}

    void onUpdate() override
    {

    }

    void onEvent(Apollo::Event& event) override
    {
       
    }
};

SandboxMain::SandboxMain(Apollo::Window::Properties props)
    : Application(props)
{
    pushLayer(new TestLayer());
    pushOverlay(new Apollo::ImguiLayer());
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

namespace Apollo
{
    Application* createApplication()
    {
        Apollo::Window::Properties props;
        props.title = "Sandbox";

        return new SandboxMain(props);
    }
}