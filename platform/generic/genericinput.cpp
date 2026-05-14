#include "genericinput.h"

#include <SDL3/SDL.h>

#include "application.h"

namespace Apollo
{
    Input* Input::s_instance = new GenericInput();

    bool GenericInput::isKeyPressedImpl(int keycode)
    {
        const auto state = SDL_GetKeyboardState(nullptr);
        //const SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, nullptr);
        return state[keycode];
    }

    bool GenericInput::isMouseButtonPressedImpl(int button)
    {
        auto state = SDL_GetMouseState(nullptr, nullptr);
        return (state & SDL_BUTTON_MASK(button)) != 0;
    }

    glm::vec2 GenericInput::getMousePosImpl()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }
} // Apollo