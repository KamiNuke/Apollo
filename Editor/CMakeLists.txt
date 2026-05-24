set(SOURCES
        src/EditorApp.cpp
        src/EditorLayer.h
        src/EditorLayer.cpp
        src/Panels/SceneHierarchyPanel.cpp
        src/Panels/SceneHierarchyPanel.h
        src/Panels/ContentBrowserPanel.cpp
        src/Panels/ContentBrowserPanel.h
)

set(BUILD_NAME Apollo)

add_executable(${BUILD_NAME})

target_sources(${BUILD_NAME} PRIVATE ${SOURCES})

target_link_libraries(${BUILD_NAME} Engine)