#pragma once
#include <glm/glm.hpp>

#include "Base.h"

namespace Apollo
{
    class Input
    {
    public:
        virtual ~Input() = default;

        inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
        inline static glm::vec2 GetMousePos() { return s_instance->GetMousePosImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual glm::vec2 GetMousePosImpl() = 0;
    private:
        static Scope<Input> s_instance;
    };
}
