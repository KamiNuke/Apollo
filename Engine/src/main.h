#pragma once

#include <Logger/Log.h>
#include "Core/Application.h"

extern Apollo::Application* Apollo::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char* argv[])
{
    Apollo::Log::Init();

    Apollo::Application* app = Apollo::CreateApplication({argc, argv});
    app->Run();
    delete app;

    return 0;
}