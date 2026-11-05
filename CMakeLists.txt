cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(opengl LANGUAGES C CXX)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

# Common function for OpenGL executables
function(add_gl_program TARGET SOURCE)
    if(WIN32)
        add_executable(${TARGET} WIN32
        ${SOURCE}
        src/opengl_util.cpp
        src/glad.c
        src/logger.cpp
    )
    else()
        add_executable(${TARGET}
        ${SOURCE}
        src/glad.c
        src/opengl_util.cpp
        src/logger.cpp
    )
    endif()
    
    target_include_directories(${TARGET} PRIVATE
        include
        $ENV{HOME}/openGL/GLFW/include
    )

    if(WIN32)
    target_link_libraries(${TARGET} PRIVATE
        $ENV{HOME}/openGL/GLFW/build-w64/src/libglfw3.a
        $ENV{HOME}/openGL/proj/third-party/spdlog/build-w64/libspdlog.a
        opengl32
        gdi32
        user32
        shell32
    )
    elseif(LINUX)
    target_link_libraries(${TARGET} PRIVATE
        $ENV{HOME}/openGL/GLFW/build-linux/src/libglfw3.a
        $ENV{HOME}/openGL/proj/third-party/spdlog/build/libspdlog.a
        GL
    )
    endif()
endfunction()

add_gl_program(shader src/shader.cpp)