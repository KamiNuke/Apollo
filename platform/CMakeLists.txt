# Platform dependent code

set(BUILD_NAME platform)

set(SOURCES
        generic/genericwindow.cpp
        generic/genericwindow.h
        ${GLAD_SOURCES}
        generic/genericinput.cpp
        generic/genericinput.h
)

# Platform detection
#if(CMAKE_SYSTEM_NAME MATCHES Windows)
#    set(SOURCES
#
#)
#elseif (CMAKE_SYSTEM_NAME MATCHES Linux)
#    set(SOURCES
#
#)
#elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
#    set(SOURCES
#
#   )
#else ()
#    message(FATAL_ERROR "UNKNOWN PLATFORM")
#endif()

add_library(${BUILD_NAME} STATIC ${SOURCES})

target_link_libraries(${BUILD_NAME} PRIVATE
        SDL3::SDL3)

target_include_directories(${BUILD_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/core/src
        ${GLAD_INCLUDE_DIRS}
)