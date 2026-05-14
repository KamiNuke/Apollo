#include "sandboxmain.h"

#include "../../thirdparty/imgui/imgui.h"
#include "event/keyevent.h"

class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test") {}

    void onUpdate() override
    {

    }

    void onImGuiRender() override
    {
        ImGui::Begin("TEST");
        ImGui::Text("test");
        ImGui::End();
    }

    void onEvent(Apollo::Event& event) override
    {

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