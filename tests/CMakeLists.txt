project(metapptest)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

set(THIRDPARTY_PATH ../../thirdparty)

include_directories(../include)

if(MSVC)
	add_definitions(/W4)
else()
	add_definitions(-Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_subdirectory(unittest)
add_subdirectory(docsrc)
add_subdirectory(dlib)
add_subdirectory(benchmark)
