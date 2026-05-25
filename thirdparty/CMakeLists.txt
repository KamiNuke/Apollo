option(SDL_VENDORED "Use vendored libraries" ON)

if(SDL_VENDORED)
    add_subdirectory(sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

set(GLAD_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)
set(GLAD_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

set(IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.h
)

set(IMGUI_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

set(STB_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
)

set(STB_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/stb/
)

add_subdirectory(spdlog)
add_subdirectory(glm)
add_subdirectory(entt)
add_subdirectory(yaml-cpp)

add_library(stb STATIC ${STB_SOURCES})
target_include_directories(stb PUBLIC ${STB_INCLUDE_DIRS})

add_library(glad STATIC ${GLAD_SOURCES})
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIRS})

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIRS})
target_link_libraries(imgui PUBLIC SDL3::SDL3)
add_library(imgui::imgui ALIAS imgui)

add_subdirectory(ImGuiFileDialog EXCLUDE_FROM_ALL)

target_include_directories(ImGuiFileDialog PRIVATE ${IMGUI_INCLUDE_DIRS})
target_link_libraries(ImGuiFileDialog PRIVATE imgui)

add_subdirectory(ImGuizmo)
target_link_libraries(imguizmo PRIVATE imgui)

add_subdirectory(Box2D)