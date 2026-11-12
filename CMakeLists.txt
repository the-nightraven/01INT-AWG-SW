cmake_minimum_required(VERSION 3.30)
project(01INT_AWG_SW LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable compiler warnings cross-platform
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Platform-specific compile/link options
if(WIN32)
    if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
        # Hide console window on Windows GUI release builds
        add_link_options(-mwindows)
    endif()
endif()

include(FetchContent)

function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

# Find or Fetch SDL3
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found locally. Fetching main branch from GitHub...")
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git main)
else()
    message(STATUS "Using local SDL3")
endif()

# Find or Fetch SDL3_image
find_package(SDL3_image QUIET)
if(NOT SDL3_image_FOUND)
    message(STATUS "SDL3_image not found locally. Fetching main branch from GitHub...")
    include_dependency(SDL3_image https://github.com/libsdl-org/SDL_image.git main)
else()
    message(STATUS "Using local SDL3_image")
endif()

# Find or Fetch SDL3_ttf
find_package(SDL3_ttf QUIET)
if(NOT SDL3_ttf_FOUND)
    message(STATUS "SDL3_ttf not found locally. Fetching main branch from GitHub...")
    include_dependency(SDL3_ttf https://github.com/libsdl-org/SDL_ttf.git main)
else()
    message(STATUS "Using local SDL3_ttf")
endif()

# Define common source files (shared between Debug and Release)
set(COMMON_SOURCES
    libs/event_reader/reader.cpp
    libs/event_reader/reader.h
    libs/font_cache/font_cache.cpp
    libs/font_cache/font_cache.h
    libs/logger/logger.cpp
    libs/logger/logger.h
    libs/monitor/engine_monitor.cpp
    libs/monitor/engine_monitor.h
    libs/physics/collisions/collisions.cpp
    libs/physics/collisions/collisions.h
    libs/physics/rigidbody/rigidbody.cpp
    libs/physics/rigidbody/rigidbody.h
    libs/renderer/renderer.cpp
    libs/renderer/renderer.h
    libs/typedefs/typedefs.h
    libs/updater/updater.cpp
    libs/updater/updater.h
    libs/window/window.cpp
    libs/window/window.h
    libs/ui_elements/touchable/touchable.cpp
    libs/ui_elements/touchable/touchable.h
    libs/tiny_xml/tinyxml2.cpp
    libs/tiny_xml/tinyxml2.h
    libs/ui_elements/scene/scene.cpp
    libs/ui_elements/scene/scene.h
    src/app/app.cpp
    src/app/app.h
    src/player/player.cpp
    src/player/player.h
    src/game_scenes/menu/start_menu/start_menu.cpp
    src/game_scenes/menu/start_menu/start_menu.h
    libs/includes.h
    main.cpp
)

# Debug-only sources
set(DEBUG_SOURCES
    libs/debug/debugger.cpp
    libs/debug/debugger.h
)

# Create Executable
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${DEBUG_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES})
endif()

# Target Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/app
    src/player
    libs
    libs/event_reader
    libs/font_cache
    libs/logger
    libs/monitor
    libs/physics
    libs/renderer
    libs/typedefs
    libs/updater
    libs/window
    libs/ui_elements
    libs/tiny_xml
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_include_directories(${PROJECT_NAME} PRIVATE libs/debug)
endif()

# Link SDL3 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# Copy runtime DLLs next to executable on Windows builds
if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_image::SDL3_image>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )
endif()