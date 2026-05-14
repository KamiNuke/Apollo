# Core
set(BUILD_NAME core)

set(SOURCES
        src/logger/log.cpp
        src/logger/log.h
        src/logger/log.cpp
        src/window.h
        src/defines.h
        src/application.cpp
        src/application.h
        src/main.h
        src/apollo.h
        src/event/event.h
        src/event/applicationevent.h
        src/event/mouseevent.h
        src/event/keyevent.h
        src/layer.cpp
        src/layer.h
        src/layerstack.cpp
        src/layerstack.h
        src/imgui/imguilayer.cpp
        src/imgui/imguilayer.h
        src/input.h
        src/keycodes.h
        src/mousecodes.h
)

add_library(${BUILD_NAME} STATIC ${SOURCES})

target_link_libraries(${BUILD_NAME}
        PRIVATE
        spdlog::spdlog
        platform
)

target_include_directories(${BUILD_NAME}
        PUBLIC
        src
)

