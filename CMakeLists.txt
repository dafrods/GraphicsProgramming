cmake_minimum_required(VERSION 3.23)

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(opengl LANGUAGES C CXX)

# ADD DEPENDENCY REQUIREMENTS LATER
# Need atleast a graphics driver eg Mesa
# OpenGL 3.3 or higher

# Third party Libraries
add_subdirectory(third-party/glfw)
add_subdirectory(third-party/glm)
add_subdirectory(third-party/spdlog)


# Removes mingw-w64 dll dependency for windows builds
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_library(opengl_common STATIC
        src/common/glad.c
        src/common/opengl_util.cpp
        src/common/logger.cpp
        src/common/shader.cpp
)

target_include_directories(opengl_common PUBLIC
    include
    $ENV{HOME}/openGL/GLFW/include
)

target_link_libraries(opengl_common PUBLIC
    glfw
    spdlog
    glm
)

# Needed by GLAD

if(WIN32)
target_link_libraries(opengl_common PRIVATE
    opengl32
    gdi32
    user32
    shell32
)
elseif(UNIX)
target_link_libraries(opengl_common PRIVATE
    GL
)
endif()

# Common function for OpenGL executables
function(add_gl_program TARGET SOURCE)
    if(ARGC GREATER 2)
        set(WIN32_CONSOLE "${ARGV2}")
    else()
        set(WIN32_CONSOLE OFF)
    endif()

    message("-- [debug] WIN32_CONSOLE: ${WIN32_CONSOLE}")

    if(WIN32)
        if(WIN32_CONSOLE)
            message("-- [info] Target Win32 Console App")
            add_executable(${TARGET}
            ${SOURCE}
            )
        else()
            message("-- [info] Target Win32 GUI App")

            add_executable(${TARGET} WIN32
                ${SOURCE}
            )
        endif()
    elseif(UNIX)
        message("-- [info] Target UNIX App")
        add_executable(${TARGET}
        ${SOURCE}
    )
    endif()

    target_link_libraries(${TARGET} PRIVATE opengl_common)
endfunction()

# add_gl_program(matrixops src/console/matrixops.cpp ON)
add_gl_program(linalg src/linalg.cpp)
# add_gl_program(input_test src/input.cpp)
# add_gl_program(texture src/texture.cpp)
# add_gl_program(texture-extend src/texture-extend.cpp)
# add_gl_program(texture-ex-texfil src/texture-ex-texfil.cpp)
# add_gl_program(texture-ex-input src/texture-ex-input.cpp)
# add_gl_program(texture_opengl src/texture_opengl.cpp)
# add_gl_program(shader_template src/shader_ex1.cpp)
# add_gl_program(shader_template2 src/shader_ex2.cpp)

# file(GLOB SOURCEFILES src/*.cpp)

# foreach(file in ${SOURCEFILES})
# message("${file}")
# endforeach()