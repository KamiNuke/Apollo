#include "Input.h"

#include "Platform/Generic/GenericInput.h"

namespace Apollo
{
    Scope<Input> Input::s_instance = Input::Create();

    Scope<Input> Input::Create()
    {
        return CreateScope<GenericInput>();
    }
}
