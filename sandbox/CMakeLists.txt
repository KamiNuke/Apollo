set(SOURCES
        src/sandboxmain.cpp
        src/sandboxmain.h
)

set(BUILD_NAME sandbox)

add_executable(${BUILD_NAME} ${SOURCES})

target_sources(${BUILD_NAME} PRIVATE ${SOURCES})

target_link_libraries(${BUILD_NAME} core)