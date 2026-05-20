#include "Window.h"

#include "Platform/Generic/GenericWindow.h"

namespace Apollo
{
    Scope<Window> Window::Create(const Properties& props)
    {
        return CreateScope<Platform::GenericWindow>(props);
    }
}
