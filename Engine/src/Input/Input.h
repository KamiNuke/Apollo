#pragma once
#include <glm/glm.hpp>

#include "../Core/Base.h"

namespace Apollo
{
    class Input
    {
    protected:
        Input() = default;
    public:
        virtual ~Input() = default;

        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
        inline static glm::vec2 GetMousePos() { return s_instance->GetMousePosImpl(); }

        static Scope<Input> Create();
    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual glm::vec2 GetMousePosImpl() = 0;
    private:
        static Scope<Input> s_instance;
    };
}
