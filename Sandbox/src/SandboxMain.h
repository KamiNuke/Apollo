#pragma once

#include "Apollo.h"


class SandboxMain : public Apollo::Application
{
public:
    SandboxMain(Apollo::Window::Properties props);
    ~SandboxMain();

    void OnUpdate() override;
    void OnRender() override;

private:
};

Apollo::Application* CreateApplication();
