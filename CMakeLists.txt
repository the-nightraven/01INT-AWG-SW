cmake_minimum_required(VERSION 3.30)
project(01INT_AWG_SW LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

if(WIN32 AND NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_link_options(-mwindows)
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

# SDL3 Dependencies
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git main)
endif()

find_package(SDL3_image QUIET)
if(NOT SDL3_image_FOUND)
    include_dependency(SDL3_image https://github.com/libsdl-org/SDL_image.git main)
endif()

find_package(SDL3_ttf QUIET)
if(NOT SDL3_ttf_FOUND)
    include_dependency(SDL3_ttf https://github.com/libsdl-org/SDL_ttf.git main)
endif()

# ==============================================================================
# 1. Engine Static Library (awg_engine)
# ==============================================================================
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "engine/src/*.cpp"
    "engine/include/*.hpp"
)

# Exclude debug sources from Release builds
if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    list(FILTER ENGINE_SOURCES EXCLUDE REGEX "engine/(src|include/awg)/debug/.*")
endif()

add_library(awg_engine STATIC ${ENGINE_SOURCES})

target_include_directories(awg_engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/engine/include"
)

target_link_libraries(awg_engine PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(awg_engine PUBLIC DEBUG=1)
endif()

# ==============================================================================
# 2. Example Game Executable (01INT_AWG_SW)
# ==============================================================================
file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "example/*.cpp"
    "example/*.hpp"
)

add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/example"
)

target_link_libraries(${PROJECT_NAME} PRIVATE awg_engine)

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