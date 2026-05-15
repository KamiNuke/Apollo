#pragma once
#include <glm/glm.hpp>

namespace Apollo
{
    class Input
    {
    public:
        virtual ~Input() = default;

        inline static bool isKeyPressed(int keycode) { return s_instance->isKeyPressedImpl(keycode); }
        inline static bool isMouseButtonPressed(int button) { return s_instance->isMouseButtonPressedImpl(button); }
        inline static glm::vec2 getMousePos() { return s_instance->getMousePosImpl(); }

    protected:
        virtual bool isKeyPressedImpl(int keycode) = 0;
        virtual bool isMouseButtonPressedImpl(int button) = 0;
        virtual glm::vec2 getMousePosImpl() = 0;
    private:
        static Input* s_instance;
    };
}