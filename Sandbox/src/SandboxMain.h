#pragma once

#include "Apollo.h"


class SandboxMain : public Apollo::Application
{
public:
    SandboxMain(Apollo::Window::Properties props);
    ~SandboxMain();

    void onUpdate() override;
    void onRender() override;

private:
};

Apollo::Application* createApplication();
