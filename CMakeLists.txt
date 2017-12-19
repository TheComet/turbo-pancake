cmake_minimum_required (VERSION 3.0)

project (turbo-pancake)

set (CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_LIST_DIR}/CMake/Modules")
set (CMAKE_CXX_STANDARD 11)

set (SOURCES
    "ArenaMap.cpp"
    "Camera.cpp"
    "Character.cpp"
    "CollisionUtil.cpp"
    "GameMan.cpp"
    "GSArena.cpp"
    "GSMainMenu.cpp"
    "GSMapEditor.cpp"
    "GSNoot.cpp"
    "GSOptions.cpp"
    "main.cpp"
    "Sound.cpp"
    "Texture.cpp"
    "UI.cpp")
set (HEADERS
    "ArenaMap.h"
    "Camera.h"
    "Character.h"
    "CollisionUtil.h"
    "GameMan.h"
    "Geometry.h"
    "Globals.h"
    "GSArena.h"
    "GSMainMenu.h"
    "GSMapEditor.h"
    "GSNoot.h"
    "GSOptions.h"
    "Particles.h"
    "Sound.h"
    "Stickers.h"
    "Texture.h"
    "UI.h"
    "Vector2.h")
add_executable (turbo-pancake ${SOURCES} ${HEADERS})

find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
include_directories (${SDL2_INCLUDE_DIR})
include_directories (${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries (turbo-pancake SDL2)
target_link_libraries (turbo-pancake SDL2_image)
target_link_libraries (turbo-pancake SDL2_mixer)
target_link_libraries (turbo-pancake SDL2_ttf)
