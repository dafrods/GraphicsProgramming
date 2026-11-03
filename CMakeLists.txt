cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(opengl LANGUAGES C CXX)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_executable(helloworld WIN32
    src/hellowindows.cpp
    src/glad.c
)

target_include_directories(helloworld
    PRIVATE
        include
        $ENV{HOME}/openGL/GLFW/include
        opengl32
)

target_link_libraries(helloworld PRIVATE
    $ENV{HOME}/openGL/GLFW/build-w64/src/libglfw3.a
    opengl32
    gdi32
    user32
    shell32
)