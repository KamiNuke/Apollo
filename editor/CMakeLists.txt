set(SOURCES

)

set(BUILD_NAME Apollo)

add_executable(${BUILD_NAME})

target_sources(${BUILD_NAME} PRIVATE ${SOURCES})

target_link_libraries(${BUILD_NAME} core)