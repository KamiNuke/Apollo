# Core
set(BUILD_NAME core)

set(SOURCES
        ${IMGUI_SOURCES}
        ${GLAD_SOURCES}
        src/logger/log.cpp
        src/logger/log.h
        src/logger/log.cpp
        src/window.h
        src/defines.h
        src/application.cpp
        src/application.h
        src/main.h
        src/apollo.h
)

add_library(${BUILD_NAME} STATIC ${SOURCES})

target_link_libraries(${BUILD_NAME}
        PRIVATE
        SDL3::SDL3
        spdlog::spdlog
        platform
)

target_include_directories(${BUILD_NAME}
        PUBLIC
        src
        ${IMGUI_INCLUDE_DIRS}
        ${GLAD_INCLUDE_DIRS}
)

