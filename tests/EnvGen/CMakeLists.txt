cmake_minimum_required(VERSION 3.10)
project(EnvGenTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(plot_envelope plot_envelope.cpp ../../lib/EnvGen.cpp)
target_include_directories(plot_envelope PRIVATE ../../lib)