cmake_minimum_required(VERSION 3.20)
project(FestivalGrounds VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG QUIET)
if(TARGET SDL2::SDL2)
    set(FG_SDL_TARGET SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
    set(FG_SDL_TARGET SDL2::SDL2-static)
else()
    # Fall back to the official SDL2 source release so a fresh Windows machine
    # only needs CMake and a C++ compiler. Keep it static so the game runs
    # without separately copying SDL2.dll next to the executable.
    include(FetchContent)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL2
        URL https://www.libsdl.org/release/SDL2-2.32.8.tar.gz
        URL_HASH SHA256=0ca83e9c9b31e18288c7ec811108e58bac1f1bb5ec6577ad386830eac51c787e
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(SDL2)
    if(TARGET SDL2::SDL2-static)
        set(FG_SDL_TARGET SDL2::SDL2-static)
    elseif(TARGET SDL2-static)
        set(FG_SDL_TARGET SDL2-static)
    elseif(TARGET SDL2::SDL2)
        set(FG_SDL_TARGET SDL2::SDL2)
    else()
        message(FATAL_ERROR "SDL2 was downloaded, but its CMake target was not created.")
    endif()
endif()

add_executable(festival_grounds
    src/main.cpp
    src/core/AssetManager.cpp
    src/world/World.cpp
    src/pathfinding/Pathfinder.cpp
    src/simulation/Simulation.cpp
    src/economy/Economy.cpp
    src/rendering/Renderer.cpp
    src/ui/Input.cpp
)
target_include_directories(festival_grounds PRIVATE src)
target_link_libraries(festival_grounds PRIVATE ${FG_SDL_TARGET})
if(WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(festival_grounds PRIVATE SDL2::SDL2main)
elseif(WIN32 AND TARGET SDL2main)
    target_link_libraries(festival_grounds PRIVATE SDL2main)
endif()
if(MSVC)
    target_compile_options(festival_grounds PRIVATE /W4 /permissive-)
else()
    target_compile_options(festival_grounds PRIVATE -Wall -Wextra -Wpedantic)
endif()
