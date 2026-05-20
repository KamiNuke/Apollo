set(SOURCES
        src/SandboxMain.cpp
        src/SandboxMain.h
        src/Sandbox2D.cpp
        src/Sandbox2D.h
)

set(BUILD_NAME sandbox)

add_executable(${BUILD_NAME} ${SOURCES})

target_sources(${BUILD_NAME} PRIVATE ${SOURCES})

target_link_libraries(${BUILD_NAME} Engine)