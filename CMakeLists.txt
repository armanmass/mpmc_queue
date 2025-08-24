cmake_minimum_required(VERSION 4.1)
project(MPMCQueue LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(
    main_exe
    ${SOURCES}
)

target_include_directories(
    main_exe
    PRIVATE
        include
)
