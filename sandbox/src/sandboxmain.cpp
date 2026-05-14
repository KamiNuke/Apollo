#include "sandboxmain.h"

class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test") {}

    void onUpdate() override
    {
        LOGGER_INFO("TestLayer::onUpdate()");
    }

    void onEvent(Apollo::Event& event) override
    {
        LOGGER_TRACE("TestLayer::onEvent({0})", event);
    }
};

SandboxMain::SandboxMain(Apollo::Window::Properties props)
    : Application(props)
{
    pushLayer(new TestLayer());
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