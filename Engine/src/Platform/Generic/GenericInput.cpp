#include "GenericInput.h"

#include <SDL3/SDL.h>
#include "Logger/Log.h"

namespace Apollo
{
    bool GenericInput::IsKeyPressedImpl(int keycode)
    {
        const auto state = SDL_GetKeyboardState(nullptr);
        //const SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, nullptr);
        return state[keycode];
    }

    bool GenericInput::IsMouseButtonPressedImpl(int button)
    {
        auto state = SDL_GetMouseState(nullptr, nullptr);
        return (state & SDL_BUTTON_MASK(button)) != 0;
    }

    glm::vec2 GenericInput::GetMousePosImpl()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }
} // Apollo