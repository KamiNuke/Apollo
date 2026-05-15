# Core
set(BUILD_NAME Engine)

set(SOURCES
        ${IMGUI_SOURCES}
        ${GLAD_SOURCES}
        src/Logger/Log.cpp
        src/Logger/Log.h
        src/Logger/Log.cpp
        src/Core/Window.h
        src/defines.h
        src/Core/Application.cpp
        src/Core/Application.h
        src/main.h
        src/Apollo.h
        src/Event/Event.h
        src/Event/ApplicationEvent.h
        src/Event/MouseEvent.h
        src/Event/KeyEvent.h
        src/Core/Layer.cpp
        src/Core/Layer.h
        src/Core/LayerStack.cpp
        src/Core/LayerStack.h
        src/ImGui/ImGuiLayer.cpp
        src/ImGui/ImGuiLayer.h
        src/Core/Input.h
        src/keycodes.h
        src/mousecodes.h
        src/Platform/Generic/GenericWindow.cpp
        src/Platform/Generic/GenericWindow.h
        src/Platform/Generic/GenericInput.cpp
        src/Platform/Generic/GenericInput.h
)

add_library(${BUILD_NAME} STATIC ${SOURCES})

target_link_libraries(${BUILD_NAME}
        PRIVATE
        SDL3::SDL3
        spdlog::spdlog
)

target_include_directories(${BUILD_NAME}
        PUBLIC
        src
        ${GLAD_INCLUDE_DIRS}
        ${IMGUI_INCLUDE_DIRS}
)

