project(metapppppkgdemo)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

set(TARGET pkgdemo)

set(SRC
	main.cpp
)

add_executable(
	${TARGET}
	${SRC}
)

find_package(metapp CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE metapp::metapp)

