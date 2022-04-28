set(TARGET_TEST unittest)

file(GLOB_RECURSE SRC_TEST "./*.cpp")
file(GLOB_RECURSE HEADERS "../../include/metapp/*.h")
file(GLOB_RECURSE SRC_LIB "../../src/*.cpp")

add_executable(
	${TARGET_TEST}
	${SRC_TEST}
	${HEADERS}
	${SRC_LIB}
)

target_include_directories(
	${TARGET_TEST}
	PUBLIC
	./
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_TEST} Threads::Threads)

set_target_properties(${TARGET_TEST} PROPERTIES CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
if(coverage)
	set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
endif()
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS "/bigobj /EHsc")
endif()

add_test(NAME ${TARGET_TEST} COMMAND ${TARGET_TEST})
