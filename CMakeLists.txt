cmake_minimum_required(VERSION 4.1)
project(MAZEGAME)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(MAZEGAME main.cpp
        Headers/shader.h
        Headers/camera.h
        Headers/objects.h
        World/objects.cpp
        World/collisions.cpp
        "External Headers/stb_image.cpp"
        World/textures.cpp
        Headers/mazegen.h
        Headers/mazegen.h
        World/mazeGen.cpp
        Headers/input.h
        Player/input.cpp
        Player/input.cpp
        Interface/debug.cpp
        Headers/debug.h
        "External Headers/stb_image.cpp"
        "External Headers/imgui.cpp"
        "External Headers/imgui_draw.cpp"
        "External Headers/imgui_tables.cpp"
        "External Headers/imgui_widgets.cpp"
        "External Headers/imgui_demo.cpp"
        "External Headers/imgui_impl_glfw.cpp"
        "External Headers/imgui_impl_opengl3.cpp"
        "External Headers/imgui_internal.h"
)

target_link_libraries(MAZEGAME PRIVATE
        glfw
        glad::glad
)

file(GLOB SHADER_FILES "*.glsl")
foreach(SHADER ${SHADER_FILES})
    configure_file(${SHADER} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()