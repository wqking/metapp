project(dlib)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC_LIB "../../../src/*.cpp")

add_library(dlib SHARED
	dlib.cpp
	${SRC_LIB}
)

set_target_properties(dlib PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME dlib)
