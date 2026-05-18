#include "SDL3/SDL_timer.h"
#include "Utils/PlatformUtils.h"

namespace Apollo
{
    float Time::GetTime()
    {
        return static_cast<float>(SDL_GetTicks()) / 1000.0f;
    }
}
