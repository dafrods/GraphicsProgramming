cmake_minimum_required(VERSION 3.23)

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(opengl LANGUAGES C CXX)

# ADD DEPENDENCY REQUIREMENTS LATER
# Need atleast a graphics driver eg Mesa
# OpenGL 3.3 or higher

# option(WIN32_CONSOLE "Builds executable to be used in console for Windows" OFF)

# ************************************************************************
# Third party Libraries
# ************************************************************************

# Manually Installed
add_subdirectory(third-party/glfw)
add_subdirectory(third-party/glm)
add_subdirectory(third-party/spdlog)


# ************************************************************************

# Source - https://stackoverflow.com/a/65133324
# Posted by tdashroy, modified by community. See post 'Timeline' for change history
# Retrieved 2026-11-09, License - CC BY-SA 4.0

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)

# Removes mingw-w64 dll dependency for windows builds
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_library(opengl_common STATIC
        src/common/glad.c
        src/common/opengl_util.cpp
        src/common/logger.cpp
        src/common/shader.cpp
        src/common/camera.cpp
)

target_include_directories(opengl_common PUBLIC
    include
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

    # message("-- [debug] WIN32_CONSOLE: ${WIN32_CONSOLE}")
    message("-- [debug] SOURCE: ${SOURCE}")

    if(WIN32)
        if(WIN32_CONSOLE)
            message("-- [info] Target Win32 Console App")
            add_executable(${TARGET}
            ${SOURCE}
            )

            set_target_properties(${TARGET} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/console"
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
    add_dependencies(${TARGET} copy_assets)

endfunction()

# Convenience Function for the project
function(create_executable TARGET TYPE)
    if(${TYPE} STREQUAL EXERCISE)
        add_gl_program(${TARGET} "src/exercises/${TARGET}.cpp")
    elseif(${TYPE} STREQUAL EXAMPLE)
        add_gl_program(${TARGET} "src/${TARGET}.cpp")
    elseif(${TYPE} STREQUAL CONSOLE)
        add_gl_program(${TARGET} "src/console/${TARGET}.cpp" ON)
    endif()
endfunction()

# *********************************************************
# Programs
# ********************************************************* 

# Expects C++ source files

# Add files from the exercise folder
set(Exercise
    texture-ex-input
    texture-ex-texfil
    shader_ex2
    ex3
    ex
    ex3alt
    ex2
    shader_ex1
    coordinate-ex1
    coordinate-ex2
    coordinate-ex3
)

# Add source files from the root src dir
set(Example
    triangle
    linalg
    hellowindows
    texture_opengl
    square
    input
    texture
    texture-extend
    hello_windows
    shader_pulse
    shader_tricolor
    coordinate
    coordinate-extend
    coordinate-extended
    camera
    camera-extend
    camera-extended
)

# Add source files from the console folder
set(Console
    main
    matrixops
)

foreach(EXE IN LISTS Exercise)
    create_executable(${EXE} EXERCISE)
endforeach()

foreach(EXE IN LISTS Example)
    create_executable(${EXE} EXAMPLE)
endforeach()

foreach(EXE IN LISTS Console)
    create_executable(${EXE} CONSOLE)
endforeach()



