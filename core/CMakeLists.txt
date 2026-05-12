# Core
set(BUILD_NAME core)

set(SOURCES
        ${IMGUI_SOURCES}
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
        ${IMGUI_INCLUDE_DIRS}
)

