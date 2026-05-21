#pragma once
#include <glm/glm.hpp>

#include "../Core/Base.h"

namespace Apollo
{
    class Input
    {
    public:

        static bool IsKeyPressed(int keycode);
        static bool IsMouseButtonPressed(int button);
        static glm::vec2 GetMousePos();

    };
}
