cmake_minimum_required(VERSION 4.0)
project(bullet_balance)

# raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG ${RAYLIB_VERSION}
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
set(RAYLIB_CPP_VERSION v5.5.0)
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG ${RAYLIB_CPP_VERSION}
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# dylib
set(DYLIB_VERSION v2.2.1)
find_package(dylib QUIET)
if (NOT dylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        dylib
        GIT_REPOSITORY  "https://github.com/martin-olivier/dylib"
        GIT_TAG         ${DYLIB_VERSION}
    )
    FetchContent_MakeAvailable(dylib)
endif()

# EnTT
set(ENTT_VERSION v3.15.0)
find_package(EnTT QUIET)
if (NOT EnTT_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        EnTT
        GIT_REPOSITORY "https://github.com/skypjack/entt"
        GIT_TAG ${ENTT_VERSION}
    )
    FetchContent_MakeAvailable(EnTT)
endif()

# janet
set(JANET_DIR vendor/janet)
file(GLOB JANET_SOURCES ${JANET_DIR}/*.c)

# raygui
set(RAYGUI_DIR vendor/raygui)
file(GLOB RAYGUI_SOURCES ${RAYGUI_DIR}/*.c)

# Main project
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCE_MAIN ${SOURCE_DIR}/main.cpp)
file(GLOB_RECURSE SOURCES
    ${SOURCE_DIR}/*.cpp
    ${SOURCE_DIR}/*.cc
    ${SOURCE_DIR}/*.c)

# static target
set(STATIC_SOURCES ${SOURCES})
set(STATIC_EXECUTABLE ${PROJECT_NAME})

add_executable(${STATIC_EXECUTABLE} ${SOURCES})
target_include_directories(
    ${STATIC_EXECUTABLE}
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${JANET_DIR}
    PUBLIC ${RAYGUI_DIR}
)

set_target_properties(${STATIC_EXECUTABLE} PROPERTIES CXX_STANDARD 20)
set_target_properties(${STATIC_EXECUTABLE} PROPERTIES C_STANDARD 99)

target_link_libraries(${STATIC_EXECUTABLE} PUBLIC raylib raylib_cpp EnTT)

# hotload targets
# library
set(HOTLOAD_LIBRARY_SOURCES ${SOURCES})
list(REMOVE_ITEM HOTLOAD_LIBRARY_SOURCES ${SOURCE_MAIN})
set(HOTLOAD_LIBRARY ${PROJECT_NAME}_lib)

add_library(${HOTLOAD_LIBRARY} SHARED ${HOTLOAD_LIBRARY_SOURCES})
target_include_directories(
    ${HOTLOAD_LIBRARY}
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${JANET_DIR}
    PUBLIC ${RAYGUI_DIR}
)
target_link_libraries(${HOTLOAD_LIBRARY} PUBLIC raylib raylib_cpp EnTT)
target_compile_definitions(${HOTLOAD_LIBRARY} PUBLIC HOTLOAD)

# executable
set(HOTLOAD_EXECUTABLE_SOURCES ${SOURCE_MAIN})
set(HOTLOAD_EXECUTABLE ${PROJECT_NAME}_hotload)

add_executable(${HOTLOAD_EXECUTABLE} ${HOTLOAD_EXECUTABLE_SOURCES})
target_include_directories(
    ${HOTLOAD_EXECUTABLE}
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${JANET_DIR}
    PUBLIC ${RAYGUI_DIR}
)
set_target_properties(${HOTLOAD_EXECUTABLE} PROPERTIES CXX_STANDARD 20)
set_target_properties(${HOTLOAD_EXECUTABLE} PROPERTIES C_STANDARD 99)
target_link_libraries(${HOTLOAD_EXECUTABLE} PUBLIC ${HOTLOAD_LIBRARY} dylib raylib raylib_cpp EnTT)
target_compile_definitions(${HOTLOAD_EXECUTABLE}
    PUBLIC HOTLOAD
    PUBLIC HOTLOAD_LIBRARY="${HOTLOAD_LIBRARY}")

# Web Configurations
if (${PLATFORM} MATCHES "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${STATIC_EXECUTABLE} PROPERTIES SUFFIX ".html")

    # Required linker flags for using Raylib with Emscripten
    target_link_options(${STATIC_EXECUTABLE} PRIVATE -sEXPORTED_FUNCTIONS=['_main','_malloc'] -sEXPORTED_RUNTIME_METHODS=ccall -sUSE_GLFW=3)
endif()

