#pragma once

#include <logger/log.h>
#include "application.h"

namespace Apollo
{
    extern Application* createApplication();
}

int main(int argc, char* argv[])
{
    Apollo::Log::init();

    Apollo::Application* app = Apollo::createApplication();
    app->run();
    delete app;

    return 0;
}