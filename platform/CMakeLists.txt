# Platform dependent code

set(BUILD_NAME platform)

set(SOURCES
        generic/genericwindow.cpp
        generic/genericwindow.h
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

target_include_directories(${BUILD_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/core/src
)