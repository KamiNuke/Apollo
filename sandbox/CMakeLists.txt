set(SOURCES
        src/main.cpp
)

set(BUILD_NAME sandbox)

add_executable(${BUILD_NAME})

target_sources(${BUILD_NAME} PRIVATE ${SOURCES})

target_link_libraries(${BUILD_NAME} core)