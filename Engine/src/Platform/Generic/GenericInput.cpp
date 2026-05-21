#include "Input/Input.h"

#include <SDL3/SDL.h>

namespace Apollo
{
    bool Input::IsKeyPressed(int keycode)
    {
        const auto state = SDL_GetKeyboardState(nullptr);
        //const SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, nullptr);
        return state[keycode];
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        auto state = SDL_GetMouseState(nullptr, nullptr);
        return (state & SDL_BUTTON_MASK(button)) != 0;
    }

    glm::vec2 Input::GetMousePos()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }
} // Apollo