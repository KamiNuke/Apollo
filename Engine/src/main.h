#pragma once

#include <Logger/Log.h>
#include "Core/Application.h"

namespace Apollo
{
    extern Application* CreateApplication();
}

int main(int argc, char* argv[])
{
    Apollo::Log::Init();

    Apollo::Application* app = Apollo::CreateApplication();
    app->Run();
    delete app;

    return 0;
}