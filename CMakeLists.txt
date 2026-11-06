cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(opengl LANGUAGES C CXX)

# ADD DEPENDENCY REQUIREMENTS LATER
# Need atleast a graphics driver eg Mesa
# OpenGL 3.3 or higher



# Removes mingw-w64 dll dependency for windows builds
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_library(opengl_common STATIC
        src/glad.c
        src/opengl_util.cpp
        src/logger.cpp
        src/shader.cpp
)

target_include_directories(opengl_common PUBLIC
    include
    $ENV{HOME}/openGL/GLFW/include
)

if(WIN32)
target_link_libraries(opengl_common PUBLIC
    $ENV{HOME}/openGL/GLFW/build-w64/src/libglfw3.a
    $ENV{HOME}/openGL/proj/third-party/spdlog/build-w64/libspdlog.a 
    opengl32
    gdi32
    user32
    shell32
)
elseif(LINUX)
target_link_libraries(opengl_common PUBLIC
    $ENV{HOME}/openGL/GLFW/build-linux/src/libglfw3.a
    $ENV{HOME}/openGL/proj/third-party/spdlog/build/libspdlog.a 
    GL
)
endif()

# Common function for OpenGL executables
function(add_gl_program TARGET SOURCE)
    if(WIN32)
        add_executable(${TARGET} WIN32
        ${SOURCE}
    )
    elseif(LINUX)
        add_executable(${TARGET}
        ${SOURCE}
    )
    endif()

    target_link_libraries(${TARGET} PRIVATE opengl_common)
endfunction()

add_gl_program(shader_template src/shader_ex1.cpp)
add_gl_program(shader_template2 src/shader_ex2.cpp)