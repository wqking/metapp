project(dlib)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

add_library(dlib SHARED dlib.cpp)
set_target_properties(dlib PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME dlib)
