cmake_minimum_required(VERSION 3.10)
project(drumsynth)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JACK REQUIRED jack)
pkg_check_modules(SNDFILE REQUIRED sndfile)

include_directories(lib)

file(GLOB_RECURSE SOURCES "lib/*.cpp")
list(APPEND SOURCES main.cpp)

add_executable(drumsynth ${SOURCES})

target_link_libraries(drumsynth ${JACK_LIBRARIES} ${SNDFILE_LIBRARIES})
target_include_directories(drumsynth PRIVATE ${JACK_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
target_compile_options(drumsynth PRIVATE ${JACK_CFLAGS_OTHER} ${SNDFILE_CFLAGS_OTHER})