cmake_minimum_required(VERSION 3.30)
project(01INT_AWG_SW)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
    set(CMAKE_CXX_FLAGS "-Wall -std=c++17 ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -mwindows ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(SDL2_PATH "C:/mingw64/x86_64-w64-mingw32")
set(SDL2_ttf_PATH "C:/mingw64/x86_64-w64-mingw32")
list(APPEND CMAKE_PREFIX_PATH ${SDL2_ttf_PATH})


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

find_package(Freetype REQUIRED)

# add SDL2 support
find_package(SDL2 QUIET)
if (NOT SDL2_FOUND)
    message(STATUS "Getting SDL2 from Github")
    include_dependency(SDL2 https://github.com/libsdl-org/SDL.git release-2.30.8)
else()
    message(STATUS "Using local SDL2")
endif()

# Add SDL2_image support
find_package(SDL2_image QUIET)
if (NOT SDL2_image_FOUND)
    message(STATUS "Getting SDL2_image from Github")
    include_dependency(SDL2_image https://github.com/libsdl-org/SDL_image.git release-2.8.2)
else()
    message(STATUS "Using local SDL2_image")
endif()

message(STATUS "SDL2_image Include Directory: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_image Libraries: ${SDL2_IMAGE_LIBRARIES}")


# add SDL2_ttf support
find_package(SDL2_ttf QUIET)
if (NOT SDL2_ttf_FOUND)
    message(STATUS "Getting SDL2_ttf from Github")
    include_dependency(SDL2_ttf https://github.com/libsdl-org/SDL_ttf.git release-2.22.0)
else()
    message(STATUS "Using local SDL2_ttf")
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    include_directories(${SDL2_INCLUDE_DIR})
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    include_directories(${SDL2_ttf_INCLUDE_DIR})
    include_directories(libs/debug)
    include_directories(libs/event_reader)
    include_directories(libs/font_cache)
    include_directories(libs/logger)
    include_directories(libs/monitor)
    include_directories(libs/physics)
    include_directories(libs/renderer)
    include_directories(libs/typedefs)
    include_directories(libs/updater)
    include_directories(libs/window)
    include_directories(src/app)
    include_directories(src/player)
    include_directories(src)
    include_directories(libs)
    include_directories(libs/ui_elements)
    include_directories(libs/tiny_xml)

    add_executable(01INT_AWG_SW
        libs/debug/debugger.cpp
        libs/debug/debugger.h
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
        src/app/app.cpp
        src/app/app.h
        src/player/player.cpp
        src/player/player.h
        main.cpp
        libs/includes.h
        libs/ui_elements/touchable/touchable.cpp
        libs/ui_elements/touchable/touchable.h
        libs/tiny_xml/tinyxml2.cpp
        libs/tiny_xml/tinyxml2.h
        libs/ui_elements/scene/scene.cpp
        libs/ui_elements/scene/scene.h
        src/game_scenes/menu/start_menu/start_menu.cpp
        src/game_scenes/menu/start_menu/start_menu.h)
else()
    include_directories(${FREETYPE_INCLUDE_DIRS})
    include_directories(${SDL2_INCLUDE_DIR})
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    include_directories(${SDL2_ttf_INCLUDE_DIR})
    include_directories(libs/event_reader)
    include_directories(libs/font_cache)
    include_directories(libs/logger)
    include_directories(libs/monitor)
    include_directories(libs/physics)
    include_directories(libs/renderer)
    include_directories(libs/typedefs)
    include_directories(libs/updater)
    include_directories(libs/window)
    include_directories(src/app)
    include_directories(src/player)
    include_directories(src)
    include_directories(libs)
    include_directories(libs/ui_elements)
    include_directories(libs/tiny_xml)

    add_executable(01INT_AWG_SW
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
            src/app/app.cpp
            src/app/app.h
            src/player/player.cpp
            src/player/player.h
            main.cpp
            libs/includes.h
            libs/ui_elements/touchable/touchable.cpp
            libs/ui_elements/touchable/touchable.h
            libs/tiny_xml/tinyxml2.cpp
            libs/tiny_xml/tinyxml2.h
            libs/ui_elements/scene/scene.cpp
            libs/ui_elements/scene/scene.h
            src/game_scenes/menu/start_menu/start_menu.cpp
            src/game_scenes/menu/start_menu/start_menu.h)
endif()

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        Freetype::Freetype
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
)

if (WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2_image::SDL2_image>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2_ttf::SDL2_ttf>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            VERBATIM
    )
endif()
