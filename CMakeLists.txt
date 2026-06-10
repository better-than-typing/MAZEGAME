cmake_minimum_required(VERSION 4.1)
project(MAZEGAME)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(MAZEGAME main.cpp
        Headers/shader.h
        Headers/camera.h
        Headers/objects.h
        objects.cpp
        collisions.cpp

)

target_link_libraries(MAZEGAME PRIVATE
        glfw
        glad::glad
)

file(GLOB SHADER_FILES "*.glsl")
foreach(SHADER ${SHADER_FILES})
    configure_file(${SHADER} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()