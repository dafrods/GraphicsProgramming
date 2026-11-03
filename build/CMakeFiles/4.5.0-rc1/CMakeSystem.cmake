set(CMAKE_HOST_SYSTEM "Linux-6.8.0-57-generic")
set(CMAKE_HOST_SYSTEM_NAME "Linux")
set(CMAKE_HOST_SYSTEM_VERSION "6.8.0-57-generic")
set(CMAKE_HOST_SYSTEM_PROCESSOR "x86_64")

set(_CMAKE_INPUT_TOOLCHAIN_FILE [==[/home/green/toolchains/mingw-w64.cmake]==])
set(_CMAKE_SYSTEM_TOOLCHAIN_FILE "/home/green/toolchains/mingw-w64.cmake")
include("${_CMAKE_SYSTEM_TOOLCHAIN_FILE}")

set(CMAKE_SYSTEM "Windows")
set(CMAKE_SYSTEM_NAME "Windows")
set(CMAKE_SYSTEM_VERSION "")
set(CMAKE_SYSTEM_PROCESSOR "")

set(CMAKE_CROSSCOMPILING "TRUE")

set(CMAKE_SYSTEM_LOADED 1)
