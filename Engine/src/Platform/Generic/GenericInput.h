#pragma once
#include "Core/Input.h"

namespace Apollo
{
    class GenericInput : public Input
    {
    protected:
        bool isKeyPressedImpl(int keycode) override;
        bool isMouseButtonPressedImpl(int button) override;
        glm::vec2 getMousePosImpl() override;
    };
} // Apollo
