#pragma once
#include "Core/Input.h"

namespace Apollo
{
    class GenericInput : public Input
    {
    protected:
        bool IsKeyPressedImpl(int keycode) override;
        bool IsMouseButtonPressedImpl(int button) override;
        glm::vec2 GetMousePosImpl() override;
    };
} // Apollo
