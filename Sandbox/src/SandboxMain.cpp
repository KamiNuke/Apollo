#include "SandboxMain.h"

#include "imgui.h"
#include "Event/KeyEvent.h"

class TestLayer : public Apollo::Layer
{
public:
    TestLayer()
        : Layer("Test") {}

    void OnUpdate() override
    {

    }

    void OnImGuiRender() override
    {
        ImGui::Begin("TEST");
        ImGui::Text("test");
        ImGui::End();
    }

    void OnEvent(Apollo::Event& event) override
    {

    }
};

SandboxMain::SandboxMain(Apollo::Window::Properties props)
    : Application(props)
{
    PushLayer(new TestLayer());
}

SandboxMain::~SandboxMain()
{
}

void SandboxMain::OnUpdate()
{
}

void SandboxMain::OnRender()
{
}

namespace Apollo
{
    Application* CreateApplication()
    {
        Apollo::Window::Properties props;
        props.title = "Sandbox";

        return new SandboxMain(props);
    }
}