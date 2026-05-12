CMAKE_MINIMUM_REQUIRED(VERSION 4.1)
PROJECT(Engine VERSION 0.0.1)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform detection
if(CMAKE_SYSTEM_NAME MATCHES Windows)
    add_definitions(-DPLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME MATCHES Linux)
    add_definitions(-DPLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME MATCHES Darwin)
    add_definitions(-DPLATFORM_APPLE)
else ()
    message(FATAL_ERROR "UNKNOWN PLATFORM")
endif()

add_subdirectory(thirdparty)
add_subdirectory(core)
add_subdirectory(sandbox)
#add_subdirectory(editor)